# Copyright 2026 Jeremy Horwitz
# SPDX-License-Identifier: GPL-2.0-or-later

<#
.SYNOPSIS
    Regenerates horwitz_layers.svg, the picture of this keymap's layers.

.DESCRIPTION
    The picture is generated from keymap.c, so it does not follow the keymap by itself: run this script after changing
    keymap.c or keymaps.h. It is written to the keymaps folder, next to this keymap's folder and not inside it, because
    `qmk lint` rejects any file inside a keymap folder that QMK's top-level .gitignore would ignore, and that file ignores
    every *.svg. The steps are:
      1. QMK's own tools (run through QMK MSYS) write the keymap as JSON and the keyboard's key positions as JSON.
      2. keymap-drawer ("keymap parse") turns the keymap JSON into its YAML form.
      3. polish_legends.py (next to this script) gives the legends friendlier names and reads the encoder's turn actions
         from keymap.c.
      4. keymap-drawer ("keymap draw") draws the SVG.
      5. layers_fingerprint.py (next to this script) writes line endings as LF and stamps the SVG with a hash of everything
         it was drawn from. CI recomputes that hash and fails when it differs, so a picture that was not regenerated after
         the keymap changed is caught.
    The first run creates a Python environment for keymap-drawer outside the repository (-VenvDir) and installs a pinned
    version of it from ready-made packages only, so nothing is compiled.

    Needs QMK MSYS (C:\QMK_MSYS by default), the `py` launcher, and a Python from 3.12 to 3.14. Python 3.15 is not supported
    yet: keymap-drawer needs tree-sitter 0.25, which has no ready-made package for 3.15.

.PARAMETER PythonVersion
    The Python to create the environment with, as the `py` launcher names it (default 3.14).

.PARAMETER QmkMsys
    Where QMK MSYS is installed.

.PARAMETER VenvDir
    Where the environment for keymap-drawer lives (default: keymap-drawer-venv in %LOCALAPPDATA%).
#>
param(
    [string]$PythonVersion = '3.14',
    [string]$QmkMsys = 'C:\QMK_MSYS',
    [string]$VenvDir = (Join-Path $env:LOCALAPPDATA 'keymap-drawer-venv')
)

$ErrorActionPreference = 'Stop'

$KeymapDrawerVersion = '0.23.0'
$Keyboard = 'keychron/x4/ansi_encoder/red'
$Keymap = 'horwitz'
$LayoutName = 'LAYOUT_82_ansi'
$LayerNames = @('WIN_BASE', 'WIN_FN', 'MAC_BASE', 'MAC_FN')   # the order of keymaps[] in keymap.c

$toolsDir = $PSScriptRoot
$keymapDir = Split-Path $toolsDir -Parent
$repo = (& git -C $toolsDir rev-parse --show-toplevel).Trim()

function ConvertTo-MsysPath([string]$Path) {
    $full = [IO.Path]::GetFullPath($Path)
    return '/' + $full.Substring(0, 1).ToLower() + $full.Substring(2).Replace('\', '/')
}

function Assert-Success([string]$What) {
    if ($LASTEXITCODE -ne 0) { throw "$What failed (exit code $LASTEXITCODE)." }
}

# 1. The environment for keymap-drawer.
$venvPython = Join-Path $VenvDir 'Scripts\python.exe'
$keymapExe = Join-Path $VenvDir 'Scripts\keymap.exe'
if (-not (Test-Path $venvPython)) {
    Write-Host "Creating the environment in $VenvDir ..."
    & py "-$PythonVersion" -m venv $VenvDir
    Assert-Success "Creating the environment with Python $PythonVersion (is it installed? see 'py -0p')"
}
$have = & $venvPython -m pip show keymap-drawer 2>$null | Where-Object { $_ -like 'Version:*' }
if ($have -ne "Version: $KeymapDrawerVersion") {
    Write-Host "Installing keymap-drawer $KeymapDrawerVersion (ready-made packages only) ..."
    & $venvPython -m pip install --only-binary :all: --disable-pip-version-check "keymap-drawer==$KeymapDrawerVersion"
    Assert-Success 'Installing keymap-drawer'
}

$work = Join-Path ([IO.Path]::GetTempPath()) ('layers-' + [Guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $work | Out-Null
try {
    # 2. The keymap and the key positions, from QMK's own tools.
    $keymapJson = Join-Path $work 'keymap.json'
    $infoJson = Join-Path $work 'info.json'
    $bash = Join-Path $QmkMsys 'usr\bin\bash.exe'
    $env:MSYSTEM = 'MINGW64'
    $env:CHERE_INVOKING = '1'
    $commands = "cd '$(ConvertTo-MsysPath $repo)'" +
        " && qmk c2json -kb $Keyboard -km $Keymap '$(ConvertTo-MsysPath (Join-Path $keymapDir 'keymap.c'))' > '$(ConvertTo-MsysPath $keymapJson)'" +
        " && qmk info -kb $Keyboard -f json > '$(ConvertTo-MsysPath $infoJson)'"
    & $bash -lc $commands | Out-Null
    Assert-Success 'qmk c2json / qmk info (through QMK MSYS)'
    foreach ($f in $keymapJson, $infoJson) {
        if (-not (Test-Path $f) -or (Get-Item $f).Length -eq 0) { throw "QMK wrote no output to $f." }
    }

    # 3. Parse, polish the legends, draw.
    $config = Join-Path $toolsDir 'keymap_drawer.yaml'
    $rawYaml = Join-Path $work 'raw.yaml'
    $polishedYaml = Join-Path $work 'polished.yaml'
    $svg = Join-Path $work 'layers.svg'
    & $keymapExe -c $config parse -q $keymapJson -l $LayerNames -o $rawYaml
    Assert-Success 'keymap parse'
    & $venvPython (Join-Path $toolsDir 'polish_legends.py') $rawYaml $polishedYaml (Join-Path $keymapDir 'keymap.c')
    Assert-Success 'polish_legends.py'
    & $keymapExe -c $config draw -j $infoJson -l $LayoutName $polishedYaml -o $svg
    Assert-Success 'keymap draw'

    # 4. Stamp the picture with the fingerprint of its inputs (and write it with LF line endings).
    & $venvPython (Join-Path $toolsDir 'layers_fingerprint.py') stamp $svg --root $repo
    Assert-Success 'layers_fingerprint.py stamp'

    $target = Join-Path (Split-Path $keymapDir -Parent) 'horwitz_layers.svg'
    Copy-Item $svg $target -Force
    Write-Host ("Wrote {0} ({1:N0} bytes)." -f $target, (Get-Item $target).Length)
}
finally {
    Remove-Item $work -Recurse -Force -ErrorAction SilentlyContinue
}
