#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _NUM,
    _EXT,
    _SYM,
    _FN,
};

enum custom_keycodes {
    CTRL_CMD_Q = SAFE_RANGE, // This keycode locks macOS
    CLEAR_OSM, // This keycode cancels one-shot-modifiers
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_BASE] = LAYOUT_split_3x5_3(
            KC_Q, KC_W, KC_E, KC_R, KC_T,               KC_Z, KC_U, KC_I, KC_O, KC_P,
            KC_A, KC_S, KC_D, KC_F, KC_G,               KC_H, KC_J, KC_K, KC_L, LALT(KC_U),
            KC_Y, KC_X, KC_C, KC_V, KC_B,               KC_N, KC_M, KC_COMM, KC_DOT, LCTL(KC_E),
                        MO(1), MO(2), KC_LSFT,      KC_SPC, MO(3), MO(4)),

	[_NUM] = LAYOUT_split_3x5_3(
            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                      KC_EQL, KC_7, KC_8, KC_9, KC_PLUS,
            OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), KC_NO,      KC_ASTR, KC_4, KC_5, KC_6, KC_MINS,
            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                      KC_0, KC_1, KC_2, KC_3, KC_SLSH,
                        KC_TRNS, KC_TRNS, KC_NO,                                KC_NO, KC_TRNS, KC_NO),

	[_EXT] = LAYOUT_split_3x5_3(
            CTRL_CMD_Q, LCTL(KC_B) , SGUI(KC_C), CLEAR_OSM, LALT(KC_SPC),                   LGUI(KC_LEFT), KC_PGDN, KC_PGUP, KC_INS, LGUI(KC_RGHT),
            OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), LGUI(KC_G),     KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_DEL,
            LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), KC_ENT,                     KC_ESC, KC_BSPC, KC_TAB, QK_REP, KC_NO,
                        KC_NO, KC_TRNS, KC_NO,                                  KC_NO, KC_TRNS, KC_NO),

	[_SYM] = LAYOUT_split_3x5_3(
            KC_EXLM, KC_QUES, LSA(KC_2), KC_AMPR, KC_GRV,                           KC_NO, KC_AT, KC_DLR, KC_HASH, KC_NO,
            OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), KC_DQUO,    KC_LPRN, KC_LCBR, KC_LBRC, KC_PIPE, KC_COLN,
            KC_PERC, KC_TILD, KC_NO, KC_NO, KC_QUOT,                                KC_RPRN, KC_RCBR, KC_RBRC, KC_BSLS, KC_SCLN,
                        KC_NO, KC_NO, KC_NO,                                    KC_NO, KC_TRNS, KC_TRNS),

	[_FN] = LAYOUT_split_3x5_3(
            KC_MUTE, KC_MPRV, KC_MPLY, KC_MNXT, KC_NO,                              KC_NO, KC_F7, KC_F8, KC_F9, KC_F12,
            OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), KC_NO,      KC_NO, KC_F4, KC_F5, KC_F6, KC_F11,
            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                      KC_NO, KC_F1, KC_F2, KC_F3, KC_F10,
                                KC_NO, KC_NO, KC_NO,                            KC_NO, KC_NO, KC_TRNS)
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CTRL_CMD_Q:
            if (record->event.pressed) {
                // When the key is pressed
                register_code(KC_LCTL);
                register_code(KC_LGUI);
                register_code(KC_Q);
            } else {
                // When the key is released
                unregister_code(KC_LCTL);
                unregister_code(KC_LGUI);
                unregister_code(KC_Q);
            }
            return false; // Skip all further processing of this key
        case CLEAR_OSM:
            if (record->event.pressed) {
                clear_oneshot_mods();
                rgblight_setrgb(0, 0, 0);
            }
            return false;
        default:
            return true; // Process all other keycodes normally
    }
}

// Override matrix_scan_user to check one-shot modifier state and modify lighting accordingly
void matrix_scan_user(void) {
    if (get_oneshot_mods()) {
        rgblight_sethsv_at(HSV_ORANGE, 0); // Only left side can be controlled, I do not know why
    } else {
        rgblight_setrgb(0, 0, 0);
    }

}

// Initialization function
void keyboard_post_init_user(void) {
    rgblight_setrgb(0, 0, 0);  // Ensure RGB is off initially
}
