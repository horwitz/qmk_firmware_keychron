TAP_DANCE_ENABLE = yes

# To enable debug output: compile with DEBUG=1 (e.g., qmk compile ... DEBUG=1)
# Must also have CONSOLE_ENABLE = yes, which is set automatically below.
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CONSOLE_ENABLE = yes
    OPT_DEFS += -DDEBUG=1
endif

SRC += layout.c
SRC += fnhi.c
SRC += gcp_color.c
SRC += gcp.c
SRC += ccp_color.c
SRC += ccp.c
