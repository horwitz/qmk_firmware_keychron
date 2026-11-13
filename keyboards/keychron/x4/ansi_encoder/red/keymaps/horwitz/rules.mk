# The stock `default` keymap's only rule; the encoder's per-layer actions come from encoder_map in keymap.c.
ENCODER_MAP_ENABLE = yes

# Picks Windows or Mac mode from the host's USB behavior when the keyboard is connected (see horwitz.c).
OS_DETECTION_ENABLE = yes

SRC += horwitz.c
