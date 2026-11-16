# Copyright 2026 Jeremy Horwitz
# SPDX-License-Identifier: GPL-2.0-or-later

"""Polish the keymap YAML that `keymap parse` produced, before `keymap draw` turns it into the picture.

Usage: polish_legends.py SOURCE.yaml DESTINATION.yaml (run by draw_layers.ps1 with the environment's Python).

It changes only legends and layer titles: friendlier names for this keymap's own keys, Option and Cmd on the Mac layer, the
knob's turn action, and the Fn key styled as a held key. It does not change which key is where.

Two things here are written by hand, because QMK's keymap-to-JSON tool does not carry them, and they must be updated if the
keymap changes in those places:
  * PER_LAYER: the modifier names that differ between the Windows and the Mac base layers.
  * KNOB and the "turn:" text: what turning the encoder does on each kind of layer (the encoder map is not in the JSON).
"""
import sys

import yaml

SRC, DST = sys.argv[1], sys.argv[2]

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
# Per layer: the keys whose meaning differs between the two modes.
PER_LAYER = {
    "WIN_BASE": {"LWIN": "Win", "LALT": "Alt", "RALT": "Alt"},
    "MAC_BASE": {"LALT": "Option", "LGUI": "Cmd", "RGUI": "Cmd"},
}
# What a layer's title says. (The tool underlines a key whose label equals a layer's title, to mark layer-switch keys, so a
# title must never equal a key label such as "Mac mode".)
TITLES = {
    "WIN_BASE": "Windows (base layer)",
    "WIN_FN": "Windows (with Fn held)",
    "MAC_BASE": "Mac (base layer)",
    "MAC_FN": "Mac (with Fn held)",
}
KNOB = 14  # the key that is the encoder's press: the 15th key of the first row


def legend(key):
    # a key is a plain string or a dict; a transparent key is {type: trans} with no "t"
    return key.get("t", "") if isinstance(key, dict) else key


def with_legend(key, text):
    if isinstance(key, dict):
        new = dict(key)
        new["t"] = text
        return new
    return text


data = yaml.safe_load(open(SRC, encoding="utf-8"))
out = {}
for name, keys in data["layers"].items():
    per = PER_LAYER.get(name, {})
    new_keys = []
    for i, key in enumerate(keys):
        text = legend(key)
        if isinstance(text, str) and text.startswith("MO("):
            key = {"t": "Fn", "type": "held"}          # the Fn key, drawn like a held key
        elif isinstance(text, str) and text in per:
            key = with_legend(key, per[text])
        elif isinstance(text, str) and text in COMMON:
            key = with_legend(key, COMMON[text])
        if i == KNOB:                                   # say what turning the knob does on this layer
            fn_layer = name.endswith("_FN")
            key = {"t": "Light on/off" if fn_layer else (legend(key) or "Mute"),
                   "h": "turn: Light" if fn_layer else "turn: Vol"}
        new_keys.append(key)
    out[TITLES.get(name, name)] = new_keys
data["layers"] = out
with open(DST, "w", encoding="utf-8") as f:
    yaml.safe_dump(data, f, sort_keys=False, allow_unicode=True, default_flow_style=None, width=200)
print("polished %d layers -> %s" % (len(out), DST))
