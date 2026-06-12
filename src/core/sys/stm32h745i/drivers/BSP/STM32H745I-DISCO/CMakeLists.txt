cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-drivers-bsp-disco)

set(DISCO_SOURCES
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_audio.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_bus.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_conf_template.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_errno.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_lcd.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_mmc.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_qspi.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_sdram.h
        include/sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_ts.h

        src/stm32h745i_discovery_bus.c
        src/stm32h745i_discovery.c
        src/stm32h745i_discovery_lcd.c
        src/stm32h745i_discovery_mmc.c
        src/stm32h745i_discovery_qspi.c
        src/stm32h745i_discovery_sdram.c
        src/stm32h745i_discovery_ts.c
)

add_definitions(-DUSE_STM32H745I_DISCO)


if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-sys-stm32h745i-drivers-bsp-disco-cm7 STATIC ${DISCO_SOURCES})
    target_include_directories(core-sys-stm32h745i-drivers-bsp-disco-cm7 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
    )
    target_link_libraries(core-sys-stm32h745i-drivers-bsp-disco-cm7 PUBLIC
            core-sys-stm32h745i-drivers-bsp-components-cm7
    )
endif()

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-sys-stm32h745i-drivers-bsp-disco-cm4 STATIC ${DISCO_SOURCES})
    target_include_directories(core-sys-stm32h745i-drivers-bsp-disco-cm4 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
    )
    target_link_libraries(core-sys-stm32h745i-drivers-bsp-disco-cm4 PUBLIC
            core-sys-stm32h745i-drivers-bsp-components-cm4
    )
endif()