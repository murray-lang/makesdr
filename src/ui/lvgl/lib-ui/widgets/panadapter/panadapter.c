/**
 * @file panadapter.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "panadapter_private_gen.h"
#include "../../lvgl_stm32h745i_disco_ui_project.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void panadapter_constructor_hook(lv_obj_t * obj)
{

}

void panadapter_destructor_hook(lv_obj_t * obj)
{

}

void panadapter_event_hook(lv_event_t * e)
{

}

void panadapter_set_bins(lv_obj_t * panadapter, const void * bins)
{
    if(lv_image_src_get_type(bins) != LV_IMAGE_SRC_VARIABLE) return;

    const lv_image_dsc_t * dsc = (const lv_image_dsc_t *)bins;
    const uint8_t * data = dsc->data;
    uint32_t length = dsc->data_size;

    panadapter_t * widget = (panadapter_t *)panadapter;
    lv_obj_t * chart = widget->panadapter_chart;

    lv_chart_series_t * series = lv_chart_get_series_next(chart, NULL);
    if(series == NULL) return;

    /* Resize the chart's point count to match the byte array length */
    lv_chart_set_point_count(chart, length);

    for(uint32_t i = 0; i < length; i++) {
        lv_chart_set_value_by_id(chart, series, i, (int32_t)data[i]);
    }
    lv_chart_refresh(chart);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/