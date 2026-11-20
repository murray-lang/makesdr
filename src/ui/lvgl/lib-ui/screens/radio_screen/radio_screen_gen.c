/**
 * @file radio_screen_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "radio_screen_gen.h"
#include "../../lvgl_stm32h745i_disco_ui_project.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * radio_screen_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static bool screen_created = false;
    if (screen_created) {
        LV_LOG_WARN("`radio_screen` is already initialized as a permanent screen. Returning with no change.");
        return radio_screen;
    }

    static lv_style_t centered_text;
    static lv_style_t right_text;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&centered_text);
        lv_style_init(&right_text);

        lv_style_set_text_align(&centered_text, LV_TEXT_ALIGN_CENTER);
        lv_style_set_text_align(&right_text, LV_TEXT_ALIGN_RIGHT);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if LVGL_STM32H745I_DISCO_UI_PROJECT_CHECK_COMPILE_TARGET(LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_ALL)
    if (lvgl_stm32h745i_disco_ui_project_check_target(LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_ALL)) {
        radio_screen = lv_obj_create(NULL);
        lv_obj_t * lv_obj_0 = radio_screen;
        lv_obj_set_name_static(lv_obj_0, "radio_screen_#");
        lv_obj_set_style_bg_color(lv_obj_0, COLOR_DARK_BG, 0);

        lv_obj_add_style(lv_obj_0, &style_screen_dark, 0);
        lv_obj_bind_style(lv_obj_0, &style_screen_dark, 0, &subject_theme_dark, 1);
        lv_obj_t * content = column_create(lv_obj_0, SPACE_MD, SPACE_MD, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_name(content, "content");
        lv_obj_set_width(content, lv_pct(100));
        lv_obj_set_height(content, LV_SIZE_CONTENT);
        lv_obj_t * row_0 = row_create(content, 0, SPACE_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_0, lv_pct(100));
        lv_obj_t * frequency = frequency_display_create(row_0);
        lv_obj_set_name(frequency, "frequency");

        lv_obj_t * row_1 = row_create(content, 0, SPACE_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_1, lv_pct(100));
        lv_obj_t * panadapter = panadapter_create(row_1);
        lv_obj_set_name(panadapter, "panadapter");

        lv_obj_t * row_2 = row_create(content, 0, SPACE_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_2, lv_pct(100));
        lv_obj_t * my_canvas = lv_canvas_create(row_2);
        lv_obj_set_name(my_canvas, "my_canvas");
        lv_obj_set_width(my_canvas, lv_pct(100));
        lv_obj_set_height(my_canvas, 80);
        lv_obj_set_style_bg_color(my_canvas, lv_color_hex(0xffffff), 0);

        lv_obj_t * row_3 = row_create(content, 0, SPACE_SM, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_3, lv_pct(100));
        button_create(row_3, "", icon_settings, COLOR_ACCENT, COLOR_ACCENT_TEXT, RADIUS_DEFAULT);

        button_create(row_3, "Band", NULL, COLOR_ACCENT, COLOR_ACCENT_TEXT, RADIUS_DEFAULT);

        container_create(row_3, 8, 0, LV_FLEX_FLOW_COLUMN, 1);

        button_create(row_3, "AGC", NULL, COLOR_ACCENT, COLOR_ACCENT_TEXT, RADIUS_DEFAULT);

        button_create(row_3, "Mode", NULL, COLOR_ACCENT, COLOR_ACCENT_TEXT, RADIUS_DEFAULT);

        button_create(row_3, "", icon_volume, COLOR_ACCENT, COLOR_ACCENT_TEXT, RADIUS_DEFAULT);

        the_root = lv_obj_0;
    }
    #endif

    if (the_root) screen_created = true;

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

