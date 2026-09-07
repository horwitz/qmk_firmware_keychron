# Dev Setup (horwitz keymap)

## Prerequisites

- Homebrew
- `qmk` CLI (`brew install qmk/qmk/qmk`)
- ARM toolchain (`brew install arm-gcc-bin@10`)
- VS Code (https://code.visualstudio.com)

## First-time setup

### 1. Fix qmk Python dependencies

```sh
/opt/homebrew/Cellar/qmk/1.1.8/libexec/bin/python -m pip install -r requirements.txt
```

(Run from the repo root. If the qmk version has changed, adjust the path accordingly.)

### 2. Install the VS Code `code` CLI

Open VS Code, then: `⌘⇧P` → **Shell Command: Install 'code' command in PATH**

### 3. Install the clangd extension

```sh
code --install-extension llvm-vs-code-extensions.vscode-clangd
```

### 4. Disable conflicting Microsoft C/C++ extensions (workspace-only)

In VS Code Extensions (`⌘⇧X`), find each of the following and choose **Disable (Workspace)**:
- C/C++ (`ms-vscode.cpptools`)
- C/C++ Extension Pack (`ms-vscode.cpptools-extension-pack`)
- C/C++ Themes (`ms-vscode.cpptools-themes`)

### 5. Open the workspace

```sh
code /path/to/qmk_firmware
```

When prompted, choose **Open Workspace** (to load `qmk_firmware.code-workspace`).
Dismiss any "Configure C++ IntelliSense using Makefiles?" prompts with **No**.

### 6. Build and generate the compilation database

`qmk generate-compilation-database` wipes `.build/` as part of its run, so the
order matters: **compile → generate → compile**.

```sh
qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz
qmk generate-compilation-database -kb keychron/k3_pro/ansi/rgb -km horwitz
qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz
```

Then in VS Code: `⌘⇧P` → **Reload Window**

clangd will index briefly (watch the status bar at the bottom for `clangd: indexing`),
then symbol navigation (F12 / Go to Definition, hover tooltips) will be fully functional.

## Ongoing: after changing rules.mk or adding source files

Repeat the three-command sequence and reload:

```sh
qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz
qmk generate-compilation-database -kb keychron/k3_pro/ansi/rgb -km horwitz
qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz
```

`⌘⇧P` → **Reload Window**

## Ongoing: normal compile (no rules.mk changes)

```sh
qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz
```

## Debug builds

Pass `DEBUG=1` to enable console output and `uprintf` statements:

```sh
qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz DEBUG=1
```

This automatically sets `CONSOLE_ENABLE = yes` and `#define DEBUG 1`. View output in QMK Toolbox's console.
