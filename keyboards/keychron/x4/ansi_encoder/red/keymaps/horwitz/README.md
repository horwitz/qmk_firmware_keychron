# horwitz keymap for the Lemokey X4

A Windows layout and a Mac layout in one firmware, with the keyboard choosing between them. Build target:
`keychron/x4/ansi_encoder/red`, keymap `horwitz`. (For the project's working notes, decisions and environment, see
`CLAUDE.md` in the repository root.)

## Modes

The keyboard is always in one of two **modes**. They differ only in the bottom row (and in three Fn keys, below).

| Bottom-row key | Windows mode | Mac mode |
|---|---|---|
| Right of Left Ctrl | Windows key | Option |
| Left of the space bar | Alt | Command |
| Right of the space bar | Alt | Command |

Everything else, including the F-row and the encoder, is the same in both.

| Layer | Number | Used in |
|---|---|---|
| `WIN_BASE` | 0 | Windows mode |
| `WIN_FN` | 1 | Windows mode, while Fn is held |
| `MAC_BASE` | 2 | Mac mode |
| `MAC_FN` | 3 | Mac mode, while Fn is held |

## Choosing the mode

- **Automatically.** Each time the keyboard is connected to a computer (or plugged in), it guesses the computer's
  operating system from how it talks over USB. A Windows computer gets Windows mode and a Mac gets Mac mode. This takes
  a fraction of a second.
- **By hand.** **Fn + `\`** selects Windows mode and **Fn + `/`** selects Mac mode. A manual choice wins until the next
  connection, and it is also saved in the keyboard's memory.
- **The saved mode is the fallback.** When the guess does not give Windows or Mac (e.g., a Linux computer, or a power-only
  charger that never talks to the keyboard), the keyboard keeps the saved mode, which is the one you last chose by hand.
  The automatic choice is never saved, so it never overwrites your own.
- **The guess can be wrong.** It is a heuristic (QMK reports a Mac as "iOS", which this keymap treats as a Mac). If it
  gets it wrong, choose by hand.

Selecting Mac mode also turns off the Win lock (below), so Command can never be left disabled.

## Feedback

- **The blink.** The key of the active mode (`\` for Windows, `/` for Mac) pulses dark three times when you press a mode key
  and at every connection once the mode is settled, whether or not the mode changed. It works with the backlight on,
  dimmed or off.
- **The Fn display.** While Fn is held, only the keys that do something on the Fn layer are lit, at full brightness, and
  every other key is dark (with the backlight on, dimmed or off). Both mode keys are lit, and **the active mode's key
  breathes** (fades slowly), so the key that moves is the mode you are in. That is 20 keys in Windows mode and 19 in Mac
  mode. The display is read from the keymap, so a key added to a Fn layer lights up on its own.
- **Sleep.** The lighting turns off when the computer sleeps and comes back when it wakes.

## The Fn layer

Hold **Fn** and press:

| Key | Action |
|---|---|
| F1 / F2 | Screen brightness down / up |
| F3 | Windows: Task View (`Win+Tab`). Mac: Mission Control |
| F4 | Windows: File Explorer (`Win+E`). Mac: Launchpad |
| F5 / F6 | Backlight dimmer / brighter |
| F7 / F8 / F9 | Previous track / play-pause / next track |
| F10 / F11 / F12 | Mute / volume down / volume up |
| Tab | Backlight on/off |
| Q | Next lighting effect |
| W / S | Backlight brighter / dimmer |
| N | Toggle N-key rollover |
| `\` | Windows mode |
| `/` | Mac mode |
| Left Win key (the one next to Left Ctrl) | Windows only: toggle the **Win lock**, which disables the Windows key (the lock LED stays lit while it is on); not available in Mac mode |
| Encoder: turn / press | Backlight brightness down / up; backlight on/off (without Fn: volume down / up; mute) |

## Building, flashing and debugging

On Windows these run in the QMK MSYS shell (see `CLAUDE.md`, "Environment: Windows + QMK MSYS", for how to start it).

    qmk compile -kb keychron/x4/ansi_encoder/red -km horwitz
    qmk flash   -kb keychron/x4/ansi_encoder/red -km horwitz

To flash, put the board in its bootloader first: unplug it, hold **Esc**, and plug it in while holding. After a flash it
restarts by itself.

**Debug output** is built in only on request: add `-e DEBUG=1` to either command. That switches on the console, and the
keymap then prints a line for each mode key press and each automatic choice. Read it with `qmk console -t`, or in the
separate **HID Console** window of QMK Toolbox (on Windows the main log pane does not show it). A normal build has none of
this, and the DEBUG build is larger (about 74 KB against 68 KB). Flash a normal build again when you are done.

## Files

| File | Purpose |
|---|---|
| `keymap.c` | The base layers and the encoder map |
| `keymaps.h` | The two Fn layers and the Windows shortcuts |
| `layers.h` | The layer numbers |
| `ctrlkeycodes.h` | The two custom keycodes for choosing the mode |
| `horwitz.c` | The behavior: choosing the mode, the Win-lock release, the blink, and the Fn display |
| `rules.mk` | Build options (the encoder map, OS detection and the `DEBUG=1` switch) |

There is no VIA/Launcher support on purpose: the keymap in the repository is the only source of truth, and it changes by
reflashing.
