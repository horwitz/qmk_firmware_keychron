TAP_DANCE_ENABLE = yes

# To enable debug output: compile with DEBUG=1 (e.g., qmk compile ... DEBUG=1)
# Must also have CONSOLE_ENABLE = yes, which is set automatically below.
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CONSOLE_ENABLE = yes
    OPT_DEFS += -DDEBUG=1
endif

SRC += $(USER_PATH)/layout.c
SRC += $(USER_PATH)/fnhi_color.c
SRC += $(USER_PATH)/fnhi.c
SRC += $(USER_PATH)/gcp_color.c
SRC += $(USER_PATH)/gcp.c
SRC += $(USER_PATH)/ccp_color.c
SRC += $(USER_PATH)/ccp.c
SRC += $(USER_PATH)/horwitz.c

# keymaps[] lives in a shared file instead of each keyboard's keymap.c. QMK #includes this file into
# quantum/keymap_introspection.c (see builddefs/build_keyboard.mk), so it must NOT be added to SRC.
INTROSPECTION_KEYMAP_C = $(USER_PATH)/horwitz_keymap.c
