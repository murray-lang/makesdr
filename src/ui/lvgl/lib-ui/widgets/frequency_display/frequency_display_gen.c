/**
 * @file frequency_display_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "frequency_display_private_gen.h"
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
#include "../../lvgl_stm32h745i_disco_ui_project.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

void frequency_display_constructor_hook(lv_obj_t * obj);
void frequency_display_destructor_hook(lv_obj_t * obj);
void frequency_display_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void frequency_display_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void frequency_display_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void frequency_display_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t frequency_display_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = frequency_display_constructor,
    .destructor_cb = frequency_display_destructor,
    .event_cb = frequency_display_event,
    .instance_size = sizeof(frequency_display_t),
    .editable = 1,
    .name = "frequency_display"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * frequency_display_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&frequency_display_class, parent);
    lv_obj_class_init_obj(obj);

    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void frequency_display_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    frequency_display_t * widget = (frequency_display_t *)obj;
    static lv_style_t style_base;
    static lv_style_t left_text;
    static lv_style_t right_text;
    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);
        lv_style_init(&left_text);
        lv_style_init(&right_text);

        lv_style_set_bg_opa(&style_base, (255 * 100 / 100));
        lv_style_set_bg_color(&style_base, lv_color_hex(0x000000));
        lv_style_set_text_color(&style_base, lv_color_hex(0x0dacf1));
        lv_style_set_text_align(&left_text, LV_TEXT_ALIGN_LEFT);
        lv_style_set_align(&left_text, LV_ALIGN_BOTTOM_RIGHT);
        lv_style_set_text_font(&left_text, roboto_mono_medium);
        lv_style_set_text_align(&right_text, LV_TEXT_ALIGN_RIGHT);
        lv_style_set_align(&right_text, LV_ALIGN_BOTTOM_LEFT);
        lv_style_set_text_font(&right_text, roboto_mono_large);

        style_inited = true;
    }

    lv_obj_t * the_root = NULL;
    #if LVGL_STM32H745I_DISCO_UI_PROJECT_CHECK_COMPILE_TARGET(LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_ALL)
    if (lvgl_stm32h745i_disco_ui_project_check_target(LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_ALL)) {
        lv_obj_set_width(obj, 165);
        lv_obj_set_height(obj, 35);

        lv_obj_add_style(obj, &style_base, 0);
        lv_obj_t * row_0 = row_create(obj, 0, 0, 0, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row_0, lv_pct(100));
        widget->row_0 = row_0;
        lv_obj_t * khz = h3_create(row_0, "00000");
        lv_obj_set_name(khz, "khz");
        lv_obj_set_width(khz, 120);
        widget->khz = khz;
        lv_obj_add_style(khz, &right_text, 0);

        lv_obj_t * hz = h4_create(row_0, "000", "");
        lv_obj_set_name(hz, "hz");
        widget->hz = hz;
        lv_obj_add_style(hz, &left_text, 0);

        the_root = obj;
    }
    #endif
    frequency_display_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void frequency_display_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    frequency_display_destructor_hook(obj);
}

static void frequency_display_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&frequency_display_class, e);
    if(res != LV_RESULT_OK) return;

    frequency_display_event_hook(e);
}

