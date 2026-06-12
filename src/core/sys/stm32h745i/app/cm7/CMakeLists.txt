cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-app-cm7)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/sys/stm32h745i/app/cm7)

set(STARTUP_FILE "${CMAKE_CURRENT_LIST_DIR}/src/startup_stm32h745xihx.s")
set(LD_FILE ${CMAKE_CURRENT_LIST_DIR}/STM32H745XIHX_QSPI.ld)

set(CM7_APP_SOURCES
        ${LOCAL_INCLUDE_DIR}/config.h
        ${LOCAL_INCLUDE_DIR}/usb_msc.h
        src/config.c
        src/stm32h7xx_it.c
        src/usb_msc.c
)

add_library(core-sys-stm32h745i-app-cm7-startup OBJECT ${STARTUP_FILE})

add_library(core-sys-stm32h745i-app-cm7 STATIC ${CM7_APP_SOURCES})

target_compile_definitions(core-sys-stm32h745i-app-cm7 PUBLIC
        CORE_CM7
        TS_MULTI_TOUCH_SUPPORTED
        LV_DRV_NO_CONF
        LV_CONF_INCLUDE_SIMPLE
)

target_include_directories(core-sys-stm32h745i-app-cm7 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(core-sys-stm32h745i-app-cm7 PUBLIC
        -T${LD_FILE}
        core-sys-stm32h745i-app-common-cm7
        core-sys-stm32h745i-lvgl-cm7
        $<TARGET_OBJECTS:core-sys-stm32h745i-app-cm7-startup>
)
