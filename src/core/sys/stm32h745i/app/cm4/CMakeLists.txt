cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-app-cm4)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/sys/stm32h745i/app/cm4)

set(STARTUP_FILE "${CMAKE_CURRENT_LIST_DIR}/src/startup_stm32h745xihx.s")
set(LD_FILE ${CMAKE_CURRENT_LIST_DIR}/STM32H745XIHX_QSPI.ld)

set(CM4_APP_SOURCES
        ${LOCAL_INCLUDE_DIR}/config.h
        src/config.c
        src/stm32h7xx_it.c
        src/stm32h7xx_hal_msp.c
)

add_library(core-sys-stm32h745i-app-cm4-startup OBJECT ${STARTUP_FILE})

add_library(core-sys-stm32h745i-app-cm4 STATIC ${CM4_APP_SOURCES})

target_compile_definitions(core-sys-stm32h745i-app-cm4 PUBLIC
        CORE_CM4
)

target_include_directories(core-sys-stm32h745i-app-cm4 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(core-sys-stm32h745i-app-cm4 PUBLIC
        -T${LD_FILE}
        core-sys-stm32h745i-app-common-cm4
        $<TARGET_OBJECTS:core-sys-stm32h745i-app-cm4-startup>
)
