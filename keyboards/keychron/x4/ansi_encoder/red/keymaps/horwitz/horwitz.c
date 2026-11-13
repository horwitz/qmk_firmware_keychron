// Behavior of the horwitz keymap for the Lemokey X4 (named as on the K3, where the shared behavior is horwitz.c):
// the keys that choose between Windows mode and Mac mode, and the automatic choice of the mode when the keyboard is
// connected to a host.

#include QMK_KEYBOARD_H
#include "layers.h"
#include "ctrlkeycodes.h"
#include "os_detection.h"
#include "usb_device_state.h"

// x4.c keeps the state of its GUI ("Win") lock in a private variable, which it also saves to the EEPROM user datablock
// (eeconfig_update_user_datablock) on every toggle and reloads at boot. We rely on that to read the lock state.
_Static_assert(EECONFIG_USER_DATA_SIZE == 1, "x4.c's lock state is expected to be the only byte of the user datablock");

// The lock disables the GUI keys, which are Command on the Mac layer, and it would stay on after switching modes: Command
// would be dead, with no lock key on the Mac layer to turn it off. So selecting Mac mode first releases it.
//
// Two states have to be cleared: x4.c's private one (which drives the lock LED) and QMK's keymap_config.no_gui (which
// actually drops the GUI keycodes). A GU_TOGG press normally changes both, but they are checked separately here, so that
// neither is toggled the wrong way if they ever disagree.
static void release_gui_lock(void) {
    uint8_t x4_lock[EECONFIG_USER_DATA_SIZE] = {0};
    eeconfig_read_user_datablock(x4_lock);
    if (x4_lock[0]) {
        // x4.c's process_record_kb toggles its own state, the LED and the saved copy on GU_TOGG. It does not toggle
        // keymap_config.no_gui (QMK's own handling of GU_TOGG, which we bypass here, does that).
        keyrecord_t record = {.event = MAKE_KEYEVENT(0, 0, true)};
        process_record_kb(GU_TOGG, &record);
    }
    if (keymap_config.no_gui) {
        keymap_config.no_gui = false;
        eeconfig_update_keymap(keymap_config.raw);
    }
}

// Starts the blink that confirms a mode key press (defined with the LED code below; does nothing without an LED matrix).
static void start_mode_blink(uint8_t base_layer);

// Automatic mode selection (task 15). The EEPROM holds the mode the owner last chose with a mode key; keyboard_init()
// restores it at every boot (quantum/bootmagic/magic.c). Each time the keyboard is connected to a host, QMK's OS detection
// (quantum/os_detection.c) guesses the host's OS from its USB setup requests, which takes a few hundred milliseconds, and
// if it says Windows or macOS that mode is applied WITHOUT saving it, so the saved mode stays the owner's own choice and is
// what is used whenever detection cannot tell (unsure, Linux or iOS). A mode key pressed meanwhile or afterwards wins until
// the next connection.
//
// "Armed" means a connection has started and its detected OS has not been applied yet. It starts true because a boot is a
// connection. This base has no callback for a finished detection, so housekeeping_task_user() polls.
static volatile bool autodetect_armed = true;

// Switches to a mode, optionally saving it. Selecting Mac first releases the GUI lock, for any way of getting there.
static void set_mode(uint8_t base_layer, bool save) {
    if (base_layer == MAC_BASE) {
        release_gui_lock();
    }
    if (save) {
        set_single_persistent_default_layer(base_layer);
    } else {
        default_layer_set((layer_state_t)1 << base_layer);
    }
}

// A mode key press: saved, blinked, and it cancels a pending automatic choice.
static void select_mode(uint8_t base_layer) {
    autodetect_armed = false;
    set_mode(base_layer, true);
    start_mode_blink(base_layer);
}

// A new connection starts when the USB state goes to INIT (a boot, a bus reset, being unconfigured). QMK's detection
// counters add up and nothing in this base ever clears them, so clear them here; stale counts would break its exact-count
// patterns (e.g., macOS is recognized only at exactly 5 requests) after a re-enumeration without a power cycle.
void notify_usb_device_state_change_user(enum usb_device_state state) {
    if (state == USB_DEVICE_STATE_INIT) {
        erase_wlength_data();
        autodetect_armed = true;
    }
}

void housekeeping_task_user(void) {
    if (!autodetect_armed) {
        return;
    }
    switch (detected_host_os()) {
        case OS_WINDOWS:
            autodetect_armed = false;
            set_mode(WIN_BASE, false);
            break;
        case OS_MACOS:
            autodetect_armed = false;
            set_mode(MAC_BASE, false);
            break;
        default:
            break; // not known (yet), or not an OS with a mode of its own: keep the saved mode
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MODE_WIN:
            if (record->event.pressed) {
                select_mode(WIN_BASE);
            }
            return false;
        case MODE_MAC:
            if (record->event.pressed) {
                select_mode(MAC_BASE);
            }
            return false;
        default:
            return true;
    }
}

#ifdef LED_MATRIX_ENABLE
// Returns the index of the LED under the key that holds `keycode` on the Windows Fn layer (NO_LED if there is none).
//
// The LED indices are found from the keymap instead of being hard-coded: scan the layer's matrix for the key, then ask
// QMK's LED layout (g_led_config.matrix_co, built from the led_matrix "layout" list in info.json) which LED sits under
// that matrix position. If a mode key is ever moved in keymaps.h, its LED follows it. (The mode keys are at the same
// positions on both Fn layers, so scanning one of them is enough.)
static uint8_t led_under_key(uint16_t keycode) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (keymap_key_to_keycode(WIN_FN, (keypos_t){.row = row, .col = col}) == keycode) {
                return g_led_config.matrix_co[row][col];
            }
        }
    }
    return NO_LED;
}

// The blink: when a mode key is pressed, its LED pulses dark BLINK_PULSES times, the same for both modes (which mode was
// chosen is shown by the mode display itself: the active mode's key is lit and the other is dark). The pattern starts and
// ends lit, so every pulse is a visible change whichever state the key was in before (a pattern that started dark would
// show nothing when switching from the mode whose key was already dark). Lit and dark both show against any backlight:
// the dark steps stand out when the backlight is on, the lit steps when it is off. The blink is drawn after the mode
// display and overrides it, so it runs to the end even if Fn is released meanwhile, and it works whatever the backlight
// state. Pressing a mode key again (the same one included) restarts it.
#    define BLINK_STEP_MS 120
#    define BLINK_PULSES 3

static uint32_t blink_start_time = 0;
static bool     blink_running    = false;
static uint8_t  blink_layer      = WIN_BASE; // the base layer (mode) whose key's LED blinks

static void start_mode_blink(uint8_t base_layer) {
    blink_layer      = base_layer;
    blink_running    = true;
    blink_start_time = timer_read32();
}

// Applies the running blink, if any, to the pressed mode key's LED (`led`, which is NO_LED if that key has none).
static void apply_blink(uint8_t led) {
    if (!blink_running) {
        return;
    }
    // Steps alternate lit, dark, lit, ...: 2 * BLINK_PULSES + 1 steps in all, starting and ending lit.
    uint32_t step = timer_elapsed32(blink_start_time) / BLINK_STEP_MS;
    if (step > 2u * BLINK_PULSES) {
        blink_running = false; // done; the normal drawing takes over from the next frame
        return;
    }
    if (led != NO_LED) {
        led_matrix_set_value(led, (step % 2 == 0) ? 0xFF : 0);
    }
}

// While a Fn layer is active (Fn is held), show the current mode on the two mode keys: the active mode's key is driven to
// full brightness and the other goes dark. With the backlight on every key is lit, so the dark key stands out; with it
// off or dimmed, the bright key does. When Fn is released, both keys return to the normal backlight.
//
// x4.c calls this first from its led_matrix_indicators_kb, then sets its own indicators (Caps Lock, the Win lock); those
// use other LEDs.
bool led_matrix_indicators_user(void) {
    // The LED lookup is done once, on the first call, when the keymap and the LED layout are certainly ready.
    static bool    looked_up = false;
    static uint8_t win_led   = NO_LED;
    static uint8_t mac_led   = NO_LED;
    if (!looked_up) {
        win_led   = led_under_key(MODE_WIN);
        mac_led   = led_under_key(MODE_MAC);
        looked_up = true;
    }

    if (layer_state_cmp(layer_state, WIN_FN) || layer_state_cmp(layer_state, MAC_FN)) {
        // The mode is the base layer: the (persistent) default layer, which the mode keys set.
        bool mac_mode = layer_state_cmp(default_layer_state, MAC_BASE);
        if (win_led != NO_LED) {
            led_matrix_set_value(win_led, mac_mode ? 0 : 0xFF);
        }
        if (mac_led != NO_LED) {
            led_matrix_set_value(mac_led, mac_mode ? 0xFF : 0);
        }
    } else if (!led_matrix_get_flags()) {
        // Fn is not held and the backlight is off. Keychron's BL_TOGG (x4.c) turns the backlight off by setting the flags
        // to LED_FLAG_NONE and clearing every LED once; no effect repaints the LEDs after that. So the full-brightness
        // value set above while Fn was held would stay on after Fn is released. Clear both keys here to prevent that.
        //
        // This mirrors what Keychron's x4.c does for the Win-lock LED: it sets that LED to 0 on every frame when the lock
        // is off and `!led_matrix_get_flags()`, the same check used here.
        //
        // ASSUMPTION: "flags == none" means the LEDs are off. That is how x4.c's BL_TOGG defines "backlight off" (it
        // toggles between LED_FLAG_ALL and LED_FLAG_NONE, and no keymap exposes other values); we do not check anything
        // else (for example, a brightness of 0 or a different effect mode). With the backlight on, the effect repaints
        // every key each frame, so nothing stays lit anyway.
        if (win_led != NO_LED) {
            led_matrix_set_value(win_led, 0);
        }
        if (mac_led != NO_LED) {
            led_matrix_set_value(mac_led, 0);
        }
    }

    apply_blink(blink_layer == MAC_BASE ? mac_led : win_led);
    return true;
}
#else
static void start_mode_blink(uint8_t base_layer) {
    (void)base_layer;
}
#endif // LED_MATRIX_ENABLE
