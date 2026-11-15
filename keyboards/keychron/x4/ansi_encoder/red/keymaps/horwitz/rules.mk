# The stock `default` keymap's only rule; the encoder's per-layer actions come from encoder_map in keymap.c.
ENCODER_MAP_ENABLE = yes

# Picks Windows or Mac mode from the host's USB behavior when the keyboard is connected (see horwitz.c).
OS_DETECTION_ENABLE = yes

SRC += horwitz.c

# Debug output: build with DEBUG=1 (e.g., qmk compile -kb keychron/x4/ansi_encoder/red -km horwitz -e DEBUG=1) to switch on
# the console and the DEBUG define used in horwitz.c; read the output in QMK Toolbox. A normal build is unchanged. DEBUG is
# deliberately not given a default here, because the build system also uses it as the compiler's debug-info level.
ifeq ($(strip $(DEBUG)),1)
    CONSOLE_ENABLE = yes
    OPT_DEFS += -DDEBUG=1
endif
