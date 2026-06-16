cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-lvgl)

set(STM32_LVGL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/lvgl)

set(LV_BUILD_CONF_PATH "${STM32_LVGL_INCLUDE_DIR}/lv_conf.h" CACHE PATH "Path to lv_conf.h" FORCE)

add_compile_definitions(LV_DRV_NO_CONF LV_CONF_INCLUDE_SIMPLE)
add_subdirectory(${LVGL_DIR} ${CMAKE_BINARY_DIR}/lvgl)

# Make FreeRTOS visible to lvgl
target_link_libraries(lvgl PUBLIC core-stm32h745i-freertos-cm7)

set(STM32_LVGL_SOURCES
        ${STM32_LVGL_INCLUDE_DIR}/lv_conf.h
        ${STM32_LVGL_INCLUDE_DIR}/lvgl_port_lcd.h
        ${STM32_LVGL_INCLUDE_DIR}/lvgl_port_touchpad.h
        src/lvgl_port_lcd.c
        src/lvgl_port_touchpad.c
)
# No CM4 support

add_library(core-stm32h745i-lvgl-cm7 STATIC ${STM32_LVGL_SOURCES})
target_include_directories(core-stm32h745i-lvgl-cm7 PUBLIC
        include
)
target_link_libraries(core-stm32h745i-lvgl-cm7 PUBLIC
        core-stm32h745i-drivers-bsp-disco-cm7
        core-stm32h745i-freertos-cm7
        lvgl
)