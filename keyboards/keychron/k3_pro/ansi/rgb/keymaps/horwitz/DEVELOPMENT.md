# Development (horwitz keymap)

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

## Flashing the keyboard

```sh
qmk flash -kb keychron/k3_pro/ansi/rgb -km horwitz
```

his compiles and then waits for a DFU device; while it waits, trigger the bootloader:

1. unplug USB from K3 Pro
2. hold Esc on K3 Pro
3. plug USB back in
4. release Esc

the flash will complete automatically and the keyboard will reboot

## Debug builds

Pass `DEBUG=1` to enable console output and `uprintf` statements:

```sh
qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz DEBUG=1
```

This automatically sets `CONSOLE_ENABLE = yes` and `#define DEBUG 1`. View output in QMK Toolbox's console.

## Linting & static analysis

All commands run from the repo root.

### clang-tidy

Prerequisite: `brew install llvm` (binary at `/opt/homebrew/opt/llvm/bin/clang-tidy`).
Requires `compile_commands.json` — see the "generate the compilation database" step above.

```sh
/opt/homebrew/opt/llvm/bin/clang-tidy \
  --header-filter='keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/.*' \
  -p . \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/ccp.c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/gcp.c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/fnhi.c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/keymap.c
```

`--header-filter` is required; without it, upstream QMK headers flood the output.
`keymap.c` is not in `compile_commands.json` (QMK inlines it), but clang-tidy still
analyzes it when listed explicitly.

### cppcheck

Prerequisite: `brew install cppcheck`.

```sh
cppcheck \
  --enable=all \
  --suppress=missingIncludeSystem \
  --suppress=missingInclude \
  --inline-suppr \
  --language=c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/keymap.c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/ccp.c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/gcp.c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/fnhi.c \
  2>&1
```

Known false positives to ignore:
- `unusedFunction` — QMK framework callbacks are called by the framework, not from within
  the keymap source, so cppcheck cannot see their call sites.
- `constParameterCallback` on `dance_grayscale` — the signature must match `tap_dance_fn_t`;
  it cannot be changed.

### -Wconversion

Add temporarily to `rules.mk`, compile, then remove:

```sh
# 1. Add to rules.mk:
echo 'EXTRAFLAGS += -Wconversion' >> keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/rules.mk

# 2. Compile and filter to horwitz warnings only:
qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz 2>&1 | grep 'warning:' | grep horwitz

# 3. Remove the added line from rules.mk when done.
```

## Formatting

### qmk lint

Checks license headers, `#pragma once` vs include guards, and other QMK-specific style rules:

```sh
qmk lint -kb keychron/k3_pro/ansi/rgb -km horwitz
```

The horwitz files intentionally use `#ifndef` include guards (not `#pragma once`)--this is a known, deferred issue.

### clang-format

Prerequisite: `brew install llvm` (binary at `/opt/homebrew/opt/llvm/bin/clang-format`).

```sh
# Check (no changes written):
/opt/homebrew/opt/llvm/bin/clang-format --dry-run --Werror \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/*.c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/*.h

# Apply:
/opt/homebrew/opt/llvm/bin/clang-format -i \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/*.c \
  keyboards/keychron/k3_pro/ansi/rgb/keymaps/horwitz/*.h
```

Note: the repo's `.clang-format` style may conflict with the keymap's formatting
conventions (e.g., aligned columns in `LAYOUT_*` macros). Review diffs carefully.
