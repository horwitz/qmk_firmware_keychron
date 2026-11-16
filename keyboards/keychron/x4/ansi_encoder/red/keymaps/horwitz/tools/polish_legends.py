# Copyright 2026 Jeremy Horwitz
# SPDX-License-Identifier: GPL-2.0-or-later

"""Polish the keymap YAML that `keymap parse` produced, before `keymap draw` turns it into the picture.

Usage: polish_legends.py SOURCE.yaml DESTINATION.yaml KEYMAP.c (run by draw_layers.ps1 with the environment's Python).

It changes only legends and layer titles: friendlier names for this keymap's own keys, Option and Cmd on the Mac layers, what
turning the knob does, and the Fn key styled as a held key. It does not change which key is where.

What is read from the keymap, so that it follows the keymap: every key, and the encoder's turn actions (from the encoder_map
in KEYMAP.c, because QMK's keymap-to-JSON tool leaves the encoder map out).

What is written here, as naming conventions and not as facts about the keymap: the friendly legend table (COMMON), what the
modifiers are called on each platform (PLATFORM, chosen by the layer's WIN_ or MAC_ prefix), the layer titles (TITLES), what a
pair of encoder keycodes is called (ENCODER_PAIRS), and the knob key's position in the layout (KNOB, a hardware fact).
"""
import re
import sys

import yaml

SRC, DST, KEYMAP_C = sys.argv[1], sys.argv[2], sys.argv[3]

# Legends that apply on every layer: what `keymap parse` printed -> what the picture should say.
COMMON = {
    "BRID": "Screen -", "BRIU": "Screen +",
    "BL DOWN": "Light -", "BL UP": "Light +", "BL TOGG": "Light on/off", "BL STEP": "Effect",
    "MPRV": "Prev", "MPLY": "Play", "MNXT": "Next", "VOLD": "Vol -", "VOLU": "Vol +",
    "NK TOGG": "NKRO", "MODE WIN": "Win mode", "MODE MAC": "Mac mode", "SHOW VER": "Version", "GU TOGG": "Win lock",
    "MCTL": "Mission Ctrl", "LPAD": "Launch pad", "Gui+TAB": "Task view", "Gui+E": "Files",
    "ESC": "Esc", "BSPC": "Bksp", "TAB": "Tab", "CAPS": "Caps", "ENT": "Enter", "SPC": "Space", "DEL": "Del",
    "LSFT": "Shift", "RSFT": "Shift", "LCTL": "Ctrl", "RCTL": "Ctrl",
    "MUTE": "Mute", "END": "End", "HOME": "Home", "PGUP": "PgUp", "PGDN": "PgDn",
    "UP": "↑", "DOWN": "↓", "LEFT": "←", "RGHT": "→",
}
# What the modifiers are called on each platform, chosen by the layer's prefix (WIN_BASE, WIN_FN, MAC_BASE, ...).
PLATFORM = {
    "WIN": {"LWIN": "Win", "LALT": "Alt", "RALT": "Alt"},
    "MAC": {"LALT": "Option", "LGUI": "Cmd", "RGUI": "Cmd"},
}
# What a layer's title says. (The tool underlines a key whose label equals a layer's title, to mark layer-switch keys, so a
# title must never equal a key label such as "Mac mode".)
TITLES = {
    "WIN_BASE": "Windows (base layer)",
    "WIN_FN": "Windows (with Fn held)",
    "MAC_BASE": "Mac (base layer)",
    "MAC_FN": "Mac (with Fn held)",
}
# What a (counterclockwise, clockwise) pair of encoder keycodes is called under the knob's label; any other pair is shown as
# its two keycodes.
ENCODER_PAIRS = {
    ("KC_VOLD", "KC_VOLU"): "Vol",
    ("BL_DOWN", "BL_UP"): "Light",
    ("KC_BRID", "KC_BRIU"): "Screen",
    ("KC_MPRV", "KC_MNXT"): "Track",
}
KNOB = 14  # the key that is the encoder's press: the 15th key of the first row of the layout


def encoder_turns(path):
    """Layer name -> what turning the encoder does there, read from the encoder_map in the keymap source."""
    text = open(path, encoding="utf-8").read()
    turns = {}
    pattern = r"\[(\w+)\]\s*=\s*\{\s*ENCODER_CCW_CW\(\s*(\w+)\s*,\s*(\w+)\s*\)\s*\}"
    for layer, ccw, cw in re.findall(pattern, text):
        turns[layer] = ENCODER_PAIRS.get((ccw, cw)) or "%s / %s" % (ccw.removeprefix("KC_"), cw.removeprefix("KC_"))
    return turns


def legend(key):
    # a key is a plain string or a dict; a transparent key is {type: trans} with no "t"
    return key.get("t", "") if isinstance(key, dict) else key


def with_legend(key, text):
    if isinstance(key, dict):
        new = dict(key)
        new["t"] = text
        return new
    return text


TURNS = encoder_turns(KEYMAP_C)
data = yaml.safe_load(open(SRC, encoding="utf-8"))
out = {}
for name, keys in data["layers"].items():
    per = PLATFORM.get(name.split("_")[0], {})
    new_keys = []
    for i, key in enumerate(keys):
        text = legend(key)
        if isinstance(text, str) and text.startswith("MO("):
            key = {"t": "Fn", "type": "held"}          # the Fn key, drawn like a held key
        elif isinstance(text, str) and text in per:
            key = with_legend(key, per[text])
        elif isinstance(text, str) and text in COMMON:
            key = with_legend(key, COMMON[text])
        if i == KNOB:                                   # say what turning the knob does on this layer, from the encoder map
            if name in TURNS:
                key = {"t": legend(key), "h": "turn: " + TURNS[name]}
            else:
                print("warning: no encoder_map entry could be read for layer %s, so its knob has no turn label" % name,
                      file=sys.stderr)
        new_keys.append(key)
    out[TITLES.get(name, name)] = new_keys
data["layers"] = out
with open(DST, "w", encoding="utf-8") as f:
    yaml.safe_dump(data, f, sort_keys=False, allow_unicode=True, default_flow_style=None, width=200)
print("polished %d layers -> %s (encoder turns read for %s)" % (len(out), DST, ", ".join(sorted(TURNS)) or "no layers"))
