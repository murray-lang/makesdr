/**
 * @file panadapter_private_gen.h
 *
 */

#ifndef LVGL_PRO_PANADAPTER_PRIVATE_GEN_H
#define LVGL_PRO_PANADAPTER_PRIVATE_GEN_H

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
#include "panadapter.h"

/*********************
 *      DEFINES
 *********************/

#ifndef PANADAPTER_USER_DATA
#define PANADAPTER_USER_DATA
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_obj_t obj;  /* Base widget to extend */
    const void * bins;
    lv_obj_t * panadapter_chart;
    void * lv_chart_axis_0;
    lv_chart_series_t * lv_chart_series_0;
    PANADAPTER_USER_DATA
} panadapter_t;

extern const lv_obj_class_t panadapter_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if defined(LV_USE_XML) && LV_USE_XML
    void panadapter_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_PANADAPTER_PRIVATE_GEN_H*/