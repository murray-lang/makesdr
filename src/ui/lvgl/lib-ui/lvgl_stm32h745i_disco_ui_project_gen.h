/**
 * @file lvgl_stm32h745i_disco_ui_project_gen.h
 */

#ifndef LVGL_PRO_LVGL_STM32H745I_DISCO_UI_PROJECT_GEN_H
#define LVGL_PRO_LVGL_STM32H745I_DISCO_UI_PROJECT_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl.h"
    #include "lvgl/lvgl_private.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif



/* Prototypes for target functions, needed by responsive const definitions */

void lvgl_stm32h745i_disco_ui_project_set_target(uint32_t target);
uint32_t lvgl_stm32h745i_disco_ui_project_get_target(void);
bool lvgl_stm32h745i_disco_ui_project_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_UNDEFINED         (0 << 1)
#define LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_STM32H745I_DISCO  (1 << 1)
#define LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_ALL               0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef LVGL_STM32H745I_DISCO_UI_PROJECT_COMPILE_TARGET
#define LVGL_STM32H745I_DISCO_UI_PROJECT_COMPILE_TARGET LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_ALL
#endif

#define LVGL_STM32H745I_DISCO_UI_PROJECT_CHECK_COMPILE_TARGET(target) (LVGL_STM32H745I_DISCO_UI_PROJECT_COMPILE_TARGET & (target) ? 1 : 0)

/**
 * Smallest spacing/padding unit
 */
#define SPACE_XS 2
/**
 * Small spacing/padding unit
 */
#define SPACE_SM 4
/**
 * Default spacing/padding unit
 */
#define SPACE_MD 8
/**
 * Large spacing/padding unit
 */
#define SPACE_LG 16
/**
 * Extra-large spacing/padding unit
 */
#define SPACE_XL 32
/**
 * Default corner radius
 */
#define RADIUS_DEFAULT 12
/**
 * Default border width
 */
#define BORDER_WIDTH 1
/**
 * Default icon size
 */
#define ICON_SIZE 16
/**
 * Used to dim down contant
 */
#define OPA_MUTED (255 * 35 / 100)
/**
 * Light theme screen background
 */
#define COLOR_LIGHT_BG lv_color_hex(0xEEF1F6)
/**
 * Light theme panel/card background
 */
#define COLOR_LIGHT_PANEL lv_color_hex(0xFFFFFF)
/**
 * Light theme primary text
 */
#define COLOR_LIGHT_TEXT lv_color_hex(0x1B1F27)
/**
 * Dark theme screen background
 */
#define COLOR_DARK_BG lv_color_hex(0x061922)
/**
 * Dark theme panel/card background
 */
#define COLOR_DARK_PANEL lv_color_hex(0x0c394c)
/**
 * Dark theme primary text
 */
#define COLOR_DARK_TEXT lv_color_hex(0xffffff)
/**
 * Accent / active control color
 */
#define COLOR_ACCENT lv_color_hex(0x0dacf1)
/**
 * Text/icon drawn on top of the accent color
 */
#define COLOR_ACCENT_TEXT lv_color_hex(0x000000)
/**
 * Destructive / warning color
 */
#define COLOR_DANGER lv_color_hex(0xca1c00)
/**
 * Neutral track behind sliders/arcs/bars
 */
#define COLOR_TRACK lv_color_hex(0x065a7f)


#ifndef LV_XML_EVAL_STRING_BUF_SIZE
    #define LV_XML_EVAL_STRING_BUF_SIZE 256
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/*-------------------
 * Permanent screens
 *------------------*/

extern lv_obj_t * radio_screen;
extern lv_obj_t * screen_components;

/*----------------
 * Global styles
 *----------------*/

extern lv_style_t style_screen_light;
extern lv_style_t style_screen_dark;
extern lv_style_t style_panel_light;
extern lv_style_t style_panel_dark;
extern lv_style_t style_text_accent;
extern lv_style_t style_text_muted;
extern lv_style_t style_scrollbar;

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * font_body_symbols;
extern lv_font_t * font_body;
extern lv_font_t * font_h5;
extern lv_font_t * font_h4;
extern lv_font_t * font_h3;
extern lv_font_t * font_h2;
extern lv_font_t * font_h1;


/*----------------
 * Images
 *----------------*/

/* Targets: any */
extern const void * icon_arrow_down;
extern const void * icon_arrow_left;
extern const void * icon_arrow_right;
extern const void * icon_arrow_up;
extern const void * icon_battery;
extern const void * icon_battery_full;
extern const void * icon_bell;
extern const void * icon_bluetooth;
extern const void * icon_calendar;
extern const void * icon_camera;
extern const void * icon_check;
extern const void * icon_chevron_down;
extern const void * icon_chevron_left;
extern const void * icon_chevron_right;
extern const void * icon_chevron_up;
extern const void * icon_clock;
extern const void * icon_close;
extern const void * icon_download;
extern const void * icon_edit;
extern const void * icon_heart;
extern const void * icon_home;
extern const void * icon_info;
extern const void * icon_lock;
extern const void * icon_mail;
extern const void * icon_menu;
extern const void * icon_minus;
extern const void * icon_moon;
extern const void * icon_pause;
extern const void * icon_play;
extern const void * icon_plus;
extern const void * icon_power;
extern const void * icon_refresh;
extern const void * icon_search;
extern const void * icon_settings;
extern const void * icon_signal;
extern const void * icon_star;
extern const void * icon_sun;
extern const void * icon_trash;
extern const void * icon_unlock;
extern const void * icon_upload;
extern const void * icon_user;
extern const void * icon_volume;
extern const void * icon_wifi;
extern const void * icon_wifi_high;
extern const void * icon_wifi_low;
extern const void * icon_wifi_zero;

/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t subject_theme_dark;
extern lv_subject_t subject_brightness;
extern lv_subject_t subject_show_keyboard;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

/**
 * Initialize the component library
 */

void lvgl_stm32h745i_disco_ui_project_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "components/controls/arc/arc_gen.h"
#include "components/controls/bar/bar_gen.h"
#include "components/controls/button/button_gen.h"
#include "components/controls/checkbox/checkbox_gen.h"
#include "components/controls/dropdown/dropdown_gen.h"
#include "components/controls/keyboard/keyboard_gen.h"
#include "components/controls/slider/slider_gen.h"
#include "components/controls/switch/switch_gen.h"
#include "components/controls/text_box/text_box_gen.h"
#include "components/controls/text_input/text_input_gen.h"
#include "components/images/image/image_gen.h"
#include "components/images/monoicon/monoicon_gen.h"
#include "components/layout/base_box/base_box_gen.h"
#include "components/layout/column/column_gen.h"
#include "components/layout/container/container_gen.h"
#include "components/layout/panel/panel_gen.h"
#include "components/layout/row/row_gen.h"
#include "components/list/list_item/list_item_gen.h"
#include "components/list/list_section/list_section_gen.h"
#include "components/list/list_separator/list_separator_gen.h"
#include "components/list/list/list_gen.h"
#include "components/typography/h1/h1_gen.h"
#include "components/typography/h2/h2_gen.h"
#include "components/typography/h3/h3_gen.h"
#include "components/typography/h4/h4_gen.h"
#include "components/typography/h5/h5_gen.h"
#include "components/typography/text/text_gen.h"
#include "screens/radio_screen/radio_screen_gen.h"
#include "screens/screen_components_gen.h"
#include "widgets/frequency_display/frequency_display_gen.h"
#include "widgets/panadapter/panadapter_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_LVGL_STM32H745I_DISCO_UI_PROJECT_GEN_H*/