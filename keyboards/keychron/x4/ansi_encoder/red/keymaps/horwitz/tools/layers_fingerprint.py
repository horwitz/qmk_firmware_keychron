# Copyright 2026 Jeremy Horwitz
# SPDX-License-Identifier: GPL-2.0-or-later

"""Tells whether the picture of the layers (horwitz_layers.svg) is current.

The picture is generated, so nothing makes it follow the keymap by itself. This script gives it a fingerprint: a hash of
everything the picture is generated from, stored in a comment at the end of the SVG when draw_layers.ps1 writes it. A check
recomputes the hash and fails when it differs, i.e., when the keymap, the board's key positions or the generator's own
legends and settings changed after the picture was drawn.

Usage:
    layers_fingerprint.py stamp SVG [--root DIR]   normalize SVG's line endings to LF and (re)write its fingerprint comment
    layers_fingerprint.py check SVG [--root DIR]   exit 0 if the fingerprint matches, 1 if not (and say what to do)

--root is the repository root; by default it is found from this file's location. Only the standard library is used, so CI can
run this with the container's python3. The hash ignores carriage returns, so it is the same on Windows and on Linux.

The check proves the picture was regenerated after the last change to its inputs. It does not prove the picture is right.
"""
import argparse
import hashlib
import pathlib
import re
import sys

ALGORITHM = "v1"  # change this if the way the hash is computed ever changes (every picture then needs a new stamp)
MARK = "layers-fingerprint"
KEYMAP = "keyboards/keychron/x4/ansi_encoder/red/keymaps/horwitz"
# What the picture is drawn from, relative to the repository root. The generator's own legends and settings are inputs too.
INPUTS = [
    "keyboards/keychron/x4/info.json",
    "keyboards/keychron/x4/ansi_encoder/red/info.json",
    f"{KEYMAP}/keymap.c",
    f"{KEYMAP}/keymaps.h",
    f"{KEYMAP}/layers.h",
    f"{KEYMAP}/ctrlkeycodes.h",
    f"{KEYMAP}/tools/polish_legends.py",
    f"{KEYMAP}/tools/keymap_drawer.yaml",
]
STAMP = re.compile(rb"\n*<!-- " + MARK.encode() + rb": ([0-9a-f]{64}) -->\s*$")


def default_root():
    # this file is <root>/keyboards/keychron/x4/ansi_encoder/red/keymaps/horwitz/tools/layers_fingerprint.py
    return pathlib.Path(__file__).resolve().parents[8]


def compute(root):
    digest = hashlib.sha256()
    digest.update(ALGORITHM.encode() + b"\n")
    for rel in INPUTS:
        path = root / rel
        if not path.is_file():
            sys.exit(f"{MARK}: an input of the picture is missing: {rel}")
        digest.update(rel.encode() + b"\n")
        digest.update(path.read_bytes().replace(b"\r", b""))
        digest.update(b"\n")
    return digest.hexdigest()


def normalized(svg):
    # LF line endings, and without any earlier fingerprint comment
    data = svg.read_bytes().replace(b"\r\n", b"\n").replace(b"\r", b"\n")
    return STAMP.sub(b"", data).rstrip(b"\n") + b"\n"


def stamp(svg, root):
    value = compute(root)
    svg.write_bytes(normalized(svg) + b"<!-- " + MARK.encode() + b": " + value.encode() + b" -->\n")
    print(f"stamped {svg.name} with {value[:12]}...")


def check(svg, root):
    found = STAMP.search(svg.read_bytes().replace(b"\r", b""))
    expected = compute(root)
    if found and found.group(1).decode() == expected:
        print(f"{svg.name} is current ({expected[:12]}...).")
        return 0
    why = "has no fingerprint" if not found else "was drawn from different inputs than the ones in this checkout"
    print(
        f"The picture of the layers is out of date: {svg.name} {why}.\n"
        "It is generated from keymap.c, keymaps.h, layers.h, ctrlkeycodes.h, the board's info.json files and the files in\n"
        "tools/ (the legends and settings). To regenerate it, in PowerShell from the keymap folder run\n"
        "    ./tools/draw_layers.ps1\n"
        "(it needs QMK MSYS and Python 3.12 to 3.14; see the README), then commit the new horwitz_layers.svg.",
        file=sys.stderr,
    )
    return 1


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("action", choices=["stamp", "check"])
    parser.add_argument("svg", type=pathlib.Path)
    parser.add_argument("--root", type=pathlib.Path, default=None)
    args = parser.parse_args()
    root = (args.root or default_root()).resolve()
    if args.action == "stamp":
        stamp(args.svg, root)
        return 0
    return check(args.svg, root)


if __name__ == "__main__":
    sys.exit(main())
