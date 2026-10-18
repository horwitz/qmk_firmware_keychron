# --------------------------------------------------------------------------------
# Keep this file, even if it is empty, as a marker that this folder contains tests
# --------------------------------------------------------------------------------

VPATH += users/horwitz

SRC += ccp_color.c
SRC += gcp_color.c
SRC += fnhi_color.c

# Catch out-of-range float -> integer conversions (undefined behavior), e.g., a negative double cast to uint8_t.
# Without this, such a bug can pass here yet misbehave on the keyboard: the host happened to wrap -43 to 213, while
# the ARM firmware clamps it to 0 (see rgb_to_hsv in ccp_color.c). -fno-sanitize-recover makes it fail the test.
# On Linux (e.g., CI), the error message shows "Lg" in place of the offending value: the sanitizer formats it with
# snprintf("%Lg"), which resolves to QMK's own printf (quantum/logging/print.mk aliases the standard names and disables
# floating-point formatting). The test name (the "[ RUN ]" line above), file:line:column, and type are still correct;
# to see the value itself, run `make test:horwitz` on macOS, where the sanitizer uses the system snprintf.
SANITIZE_FLAGS = -fsanitize=float-cast-overflow -fno-sanitize-recover=float-cast-overflow
CFLAGS += $(SANITIZE_FLAGS)
CXXFLAGS += $(SANITIZE_FLAGS)
LDFLAGS += $(SANITIZE_FLAGS)
