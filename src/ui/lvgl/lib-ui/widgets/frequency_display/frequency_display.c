/**
 * @file frequency_display.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "frequency_display_private_gen.h"
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

void frequency_display_constructor_hook(lv_obj_t * obj)
{

}

void frequency_display_destructor_hook(lv_obj_t * obj)
{

}

void frequency_display_event_hook(lv_event_t * e)
{

}

void frequency_display_set_frequency(lv_obj_t * frequency_display, int32_t frequency)
{
  
    frequency_display_t * widget = (frequency_display_t *)frequency_display;

    /* Guard against negative values if needed */
    if(frequency < 0) frequency = 0;

    int khz = frequency / 1000;
    int hz  = frequency % 1000;

    lv_label_set_text_fmt(widget->khz, "%" LV_PRId32, khz);
    lv_label_set_text_fmt(widget->hz, "%03" LV_PRId32, hz);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/