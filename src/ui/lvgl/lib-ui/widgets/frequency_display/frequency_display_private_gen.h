/**
 * @file frequency_display_private_gen.h
 *
 */

#ifndef LVGL_PRO_FREQUENCY_DISPLAY_PRIVATE_GEN_H
#define LVGL_PRO_FREQUENCY_DISPLAY_PRIVATE_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl_private.h"
#endif
#include "frequency_display.h"

/*********************
 *      DEFINES
 *********************/

#ifndef FREQUENCY_DISPLAY_USER_DATA
#define FREQUENCY_DISPLAY_USER_DATA
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_obj_t obj;  /* Base widget to extend */
    int32_t frequency;
    lv_obj_t * row_0;
    lv_obj_t * khz;
    lv_obj_t * hz;
    FREQUENCY_DISPLAY_USER_DATA
} frequency_display_t;

extern const lv_obj_class_t frequency_display_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if defined(LV_USE_XML) && LV_USE_XML
    void frequency_display_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_FREQUENCY_DISPLAY_PRIVATE_GEN_H*/