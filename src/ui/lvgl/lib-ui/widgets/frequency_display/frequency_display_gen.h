/**
 * @file frequency_display_gen.h
 *
 */

#ifndef LVGL_PRO_FREQUENCY_DISPLAY_GEN_H
#define LVGL_PRO_FREQUENCY_DISPLAY_GEN_H

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

#include "../../lvgl_stm32h745i_disco_ui_project_gen.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a frequency_display object
 * @param parent pointer to an object, it will be the parent of the new frequency_display
 * @return pointer to the created frequency_display
 */
lv_obj_t * frequency_display_create(lv_obj_t * parent);
/**
 * Radio frequency in Hz.
 * @param obj   pointer to a frequency_display
 * @param frequency  Radio frequency in Hz.
 */
void frequency_display_set_frequency(lv_obj_t * frequency_display, int32_t frequency);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_FREQUENCY_DISPLAY_GEN_H*/