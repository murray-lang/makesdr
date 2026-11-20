/**
 * @file panadapter_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "panadapter_private_gen.h"
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

void panadapter_constructor_hook(lv_obj_t * obj);
void panadapter_destructor_hook(lv_obj_t * obj);
void panadapter_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void panadapter_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void panadapter_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void panadapter_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t panadapter_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = panadapter_constructor,
    .destructor_cb = panadapter_destructor,
    .event_cb = panadapter_event,
    .instance_size = sizeof(panadapter_t),
    .editable = 1,
    .name = "panadapter"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * panadapter_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&panadapter_class, parent);
    lv_obj_class_init_obj(obj);

    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void panadapter_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    panadapter_t * widget = (panadapter_t *)obj;
    static lv_style_t style_base;
    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_base);

        lv_style_set_bg_opa(&style_base, (255 * 100 / 100));
        lv_style_set_bg_color(&style_base, lv_color_hex(0x6080e0));

        style_inited = true;
    }

    lv_obj_t * the_root = NULL;
    #if LVGL_STM32H745I_DISCO_UI_PROJECT_CHECK_COMPILE_TARGET(LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_ALL)
    if (lvgl_stm32h745i_disco_ui_project_check_target(LVGL_STM32H745I_DISCO_UI_PROJECT_TARGET_ALL)) {
        lv_obj_set_width(obj, lv_pct(100));
        lv_obj_set_height(obj, lv_pct(100));

        lv_obj_add_style(obj, &style_base, 0);
        lv_obj_t * panadapter_chart = lv_chart_create(obj);
        lv_obj_set_name(panadapter_chart, "panadapter_chart");
        lv_obj_set_width(panadapter_chart, lv_pct(100));
        lv_obj_set_height(panadapter_chart, lv_pct(100));
        lv_chart_set_type(panadapter_chart, LV_CHART_TYPE_LINE);
        lv_chart_set_point_count(panadapter_chart, 255);
        lv_chart_set_hor_div_line_count(panadapter_chart, 0);
        lv_chart_set_ver_div_line_count(panadapter_chart, 0);
        lv_obj_set_style_bg_color(panadapter_chart, lv_color_hex(0x0c394c), 0);
        widget->panadapter_chart = panadapter_chart;
        lv_chart_set_axis_min_value(panadapter_chart, LV_CHART_AXIS_PRIMARY_Y, 0);
        lv_chart_set_axis_max_value(panadapter_chart, LV_CHART_AXIS_PRIMARY_Y, 255);
        lv_chart_series_t * lv_chart_series_0 = lv_chart_add_series(panadapter_chart, lv_color_hex(0x22f2f2), LV_CHART_AXIS_PRIMARY_Y);
        widget->lv_chart_series_0 = lv_chart_series_0;
        static const int32_t panadapter_chart_values_0[] = {0};
        lv_chart_set_series_values(panadapter_chart, lv_chart_series_0, panadapter_chart_values_0, 1);

        the_root = obj;
    }
    #endif
    panadapter_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void panadapter_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    panadapter_destructor_hook(obj);
}

static void panadapter_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&panadapter_class, e);
    if(res != LV_RESULT_OK) return;

    panadapter_event_hook(e);
}

