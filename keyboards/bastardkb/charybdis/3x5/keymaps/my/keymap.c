/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_FUNCTION_NUMERAL = 1,
    LAYER_SYMBOLS = 2,
    LAYER_NAVIGATION = 3,
    LAYER_POINTER = 4,
    LAYER_MEDIA = 5,
    LAYER_GAME_WASD = 6,
    LAYER_GAME_QWERT = 7,
    LAYER_SWITCH = 8,
};

// Automatically enable sniping-mode on the pointer layer.
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define TAB_SWT LT(LAYER_SWITCH, KC_TAB)
#define SPC_FUN LT(LAYER_FUNCTION_NUMERAL, KC_SPC)
#define ENT_NAV LT(LAYER_NAVIGATION, KC_ENT)
#define DEL_MED LT(LAYER_MEDIA, KC_DEL)
#define BSP_SYM LT(LAYER_SYMBOLS, KC_BSPC)
#define ESC_SWT LT(LAYER_SWITCH, KC_ESC)
#define _L_PTR(KC) LT(LAYER_POINTER, KC)

#define MIC_MUTE G(A(KC_K)) // Mute microphone, Windows 11 shortcut

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

/** Convenience row shorthands. */
// #define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
// #define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
// #define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI

// 0 BASE
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │  Q  │  W  │  E  │  R  │  T  │                        │  Y  │  U  │  I  │  O  │  P  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │  A  │  S  │  D  │  F  │  G  │                        │  H  │  J  │  K  │  L  │  ;  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │  Z  │  X  │  C  │  V  │  B  │                        │  N  │  M  │  ,  │  .  │  /  │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ TAB │ SPC │ ENT │      │ DEL │ BSP │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_BASE \
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, \
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                   TAB_SWT, SPC_FUN, ENT_NAV,       DEL_MED, BSP_SYM

// 1 FUNCTION + NUMERAL
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │ F1  │ F2  │ F3  │ F4  │     │                        │  +  │  7  │  8  │  9  │  *  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ F5  │ F6  │ F7  │ F8  │     │                        │  -  │  4  │  5  │  6  │  /  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ F9  │ F10 │ F11 │ F12 │     │                        │  0  │  1  │  2  │  3  │  .  │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ --- │ --- │ --- │      │ --- │ --- │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_FUNCTION_NUMERAL \
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   XXXXXXX,       KC_PPLS, KC_7,  KC_8,  KC_9,  KC_PAST, \
    KC_F5,   KC_F6,   KC_F7,   KC_F8,   XXXXXXX,       KC_PMNS, KC_4,  KC_5,  KC_6,  KC_PSLS, \
    KC_F9,   KC_F10,  KC_F11,  KC_F12,  XXXXXXX,       KC_0,    KC_1,  KC_2,  KC_3,  KC_DOT,  \
                      _______, _______, _______,       _______, _______

// 2 SYMBOLS
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │  !  │  @  │  +  │  =  │  :  │                        │  `  │  {  │  }  │  |  │  \  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │  &  │  #  │  -  │  _  │  ,  │                        │  ~  │  (  │  )  │  '  │  "  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │  *  │  %  │  ^  │  $  │  .  │                        │  ?  │  [  │  ]  │  <  │  >  │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ --- │ --- │ --- │      │ --- │ --- │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_SYMBOLS \
    KC_EXLM, KC_AT,   KC_PLUS, KC_EQL,  KC_COLN,       KC_GRV,  KC_LCBR, KC_RCBR, KC_PIPE, KC_BSLS, \
    KC_AMPR, KC_HASH, KC_MINS, KC_UNDS, KC_COMM,       KC_TILD, KC_LPRN, KC_RPRN, KC_QUOT, KC_DQUO, \
    KC_ASTR, KC_PERC, KC_CIRC, KC_DLR,  KC_DOT,        KC_QUES, KC_LBRC, KC_RBRC, KC_LABK, KC_RABK, \
                      _______, _______, _______,       _______, _______

// 3 NAVIGATION
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │ ESC │     │     │     │     │                        │     │     │     │     │     │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ GUI │ ALT │ CTL │ SFT │ APP │                        │     │  ←  │  ↓  │  ↑  │  →  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ UND │ CUT │ CPY │ PST │ RDO │                        │     │ HME │ PDN │ PUP │ END │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ --- │ --- │ --- │      │ --- │ --- │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_NAVIGATION \
    KC_ESC,  KC_NO,   KC_NO,   KC_NO,   KC_NO,         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_APP,        XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, \
    C(KC_Z), C(KC_X), C(KC_C), C(KC_V), C(KC_Y),       XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  \
                      _______, _______, _______,       _______, _______

// 4 POINTER + HR MODS
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │ ESC │     │     │     │     │                        │     │     │     │     │BOOT │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ GUI │ ALT │ CTL │ SFT │ BT4 │                        │ BT4 │ SFT │ CTL │ ALT │ GUI │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ --- │SCRLL│     │     │ BT5 │                        │ BT5 │ BT1 │ BT2 │ BT3 │ --- │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ BT3 │ BT2 │ BT1 │      │ --- │ --- │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_POINTER \
    KC_ESC,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT, \
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_BTN4,        KC_BTN4, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, \
    _______, DRGSCRL, XXXXXXX, XXXXXXX, KC_BTN5,        KC_BTN5, KC_BTN1, KC_BTN2, KC_BTN3, _______, \
                      KC_BTN3, KC_BTN2, KC_BTN1,        _______, _______                                                            \

// 5 MEDIA (+RGB)
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │PAUSE│ INS │ BRI-│ BRI+│     │                        │ MUT │ MIC │ VOL-│ VOL+│ SCR │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ GUI │ ALT │ CTL │ SFT │     │                        │ ENT │  ←  │  ↓  │  ↑  │  →  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ HUE │ SAT │ VAL │ SPD │     │                        │ PP  │ RWN │ PRV │ NXT │ FRW │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ MOD<│ MOD>│ TOG │      │ --- │ --- │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_MEDIA \
KC_PAUS, KC_INS,  KC_BRID, KC_BRIU, XXXXXXX,        KC_MUTE, MIC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR, \
KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,        KC_ENT,  KC_LEFT,  KC_DOWN, KC_UP,   KC_RGHT, \
RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, XXXXXXX,        KC_MPLY, KC_MRWD,  KC_MPRV, KC_MNXT, KC_MFFD, \
                 RGB_RMOD, RGB_MOD, RGB_TOG,        _______, _______
                        
// 6 GAME - WASD (+ARROWS)
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │ TAB │  Q  │  W  │  E  │  R  │                        │  1  │  2  │  3  │  4  │  5  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ SFT │  A  │  S  │  D  │  F  │                        │ PUP │ HME │  ↑  │ END │ SFT │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ CTL │ ALT │  X  │  C  │  V  │                        │ PDN │  ←  │  ↓  │  →  │ CTL │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ ESC │ SPC │ ENT │      │ DEL │ BSP │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_GAME_WASD \
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,       KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    \
    KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,       KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_RSFT, \
    KC_LCTL, KC_LALT, KC_X,    KC_C,    KC_V,       KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_RCTL, \
                    ESC_SWT, KC_SPC,  KC_ENT,       KC_DEL,  KC_BSPC

// 7 GAME - QWERT
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │  Q  │  W  │  E  │  R  │  T  │                        │  Y  │  U  │  I  │  O  │  P  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │SFT/A│  S  │  D  │  F  │  G  │                        │  H  │  J  │  K  │  L  │  ;  │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │CTL/Z│ALT/X│  C  │  V  │  B  │                        │  N  │  M  │  ,  │  .  │  /  │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ ESC │ SPC │ ENT │      │ DEL │ BSP │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_GAME_QWERT \
  KC_Q,        KC_W,        KC_E,   KC_R,   KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
  SFT_T(KC_A), KC_S,        KC_D,   KC_F,   KC_G,       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, \
  CTL_T(KC_Z), ALT_T(KC_X), KC_C,   KC_V,   KC_B,       KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                         ESC_SWT, KC_SPC, KC_ENT,       KC_DEL,  KC_BSPC

// 8 SWITCH
// ╭─────┬─────┬─────┬─────┬─────╮                        ╭─────┬─────┬─────┬─────┬─────╮
// │ ESC │     │     │     │WASD │                        │     │     │     │     │     │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │ GUI │ ALT │ CTL │ SFT │QWERT│                        │     │     │     │     │     │
// ├─────┼─────┼─────┼─────┼─────┤                        ├─────┼─────┼─────┼─────┼─────┤
// │G-TAB│A-TAB│C-TAB│S-TAB│BASE │                        │     │     │     │     │     │
// ╰─────┴─────┴─────┴──┬──┴──┬──┴──┬─────╮      ╭─────┬──┴──┬──┴─────┴─────┴─────┴─────╯
//                      │ --- │     │     │      │     │     │
//                      ╰─────┴─────┴─────╯      ╰─────┴─────╯
#define LAYOUT_LAYER_SWITCH \
  KC_ESC,    XXXXXXX,   XXXXXXX,   XXXXXXX,   DF(LAYER_GAME_WASD),      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  KC_LGUI,   KC_LALT,   KC_LCTL,   KC_LSFT,   DF(LAYER_GAME_QWERT),     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  G(KC_TAB), A(KC_TAB), C(KC_TAB), S(KC_TAB), DF(LAYER_BASE),           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                                        _______, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX

/**
 * \brief Add Home Row mod to a layout.
 *
 * Expects a 10-key per row layout.  Adds support for GACS (Gui, Alt, Ctl, Shift)
 * home row.  The layout passed in parameter must contain at least 20 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     HOME_ROW_MOD_GACS(LAYER_ALPHAS_QWERTY)
 */
#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LGUI_T(L10), LALT_T(L11), LCTL_T(L12), LSFT_T(L13),        L14,  \
             R15,  RSFT_T(R16), RCTL_T(R17), RALT_T(R18), RGUI_T(R19), \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

#define _HOME_ROW_MOD_GACS_LEFT(                                       \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14,                                           \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LGUI_T(L10), LALT_T(L11), LCTL_T(L12), LSFT_T(L13),        L14,  \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS_LEFT(...) _HOME_ROW_MOD_GACS_LEFT(__VA_ARGS__)

/**
 * \brief Add pointer layer keys to a layout.
 *
 * Expects a 10-key per row layout.  The layout passed in parameter must contain
 * at least 30 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     POINTER_MOD(LAYER_ALPHAS_QWERTY)
 */
#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
      _L_PTR(L20),        L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,  _L_PTR(R29), \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_BASE] = LAYOUT_wrapper(
        POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))
    ),
    [LAYER_FUNCTION_NUMERAL] = LAYOUT_wrapper(
        HOME_ROW_MOD_GACS(LAYOUT_LAYER_FUNCTION_NUMERAL)
    ),
    [LAYER_SYMBOLS] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),
    [LAYER_NAVIGATION] = LAYOUT_wrapper(
        HOME_ROW_MOD_GACS_LEFT(LAYOUT_LAYER_NAVIGATION)
    ),
    [LAYER_POINTER] = LAYOUT_wrapper(
        HOME_ROW_MOD_GACS_LEFT(LAYOUT_LAYER_POINTER)
    ),
    [LAYER_MEDIA] = LAYOUT_wrapper(
        HOME_ROW_MOD_GACS_LEFT(LAYOUT_LAYER_MEDIA)
    ),
    [LAYER_GAME_WASD] = LAYOUT_wrapper(LAYOUT_LAYER_GAME_WASD),
    [LAYER_GAME_QWERT] = LAYOUT_wrapper(LAYOUT_LAYER_GAME_QWERT),
    [LAYER_SWITCH] = LAYOUT_wrapper(LAYOUT_LAYER_SWITCH)
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in
// rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif
