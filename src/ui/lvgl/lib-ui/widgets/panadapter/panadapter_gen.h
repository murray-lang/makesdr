/**
 * @file panadapter_gen.h
 *
 */

#ifndef LVGL_PRO_PANADAPTER_GEN_H
#define LVGL_PRO_PANADAPTER_GEN_H

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
 * Create a panadapter object
 * @param parent pointer to an object, it will be the parent of the new panadapter
 * @return pointer to the created panadapter
 */
lv_obj_t * panadapter_create(lv_obj_t * parent);
/**
 * Spectrum bins as bytes
 * @param obj   pointer to a panadapter
 * @param bins  Spectrum bins as bytes
 */
void panadapter_set_bins(lv_obj_t * panadapter, const void * bins);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_PANADAPTER_GEN_H*/