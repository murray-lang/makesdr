cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-drivers-bsp-disco)

set(DISCO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_audio.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_bus.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_conf_template.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_errno.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_lcd.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_mmc.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_qspi.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_sdram.h
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco/stm32h745i_discovery_ts.h

        ${CMAKE_CURRENT_LIST_DIR}/src/stm32h745i_discovery.c
        ${CMAKE_CURRENT_LIST_DIR}/src/stm32h745i_discovery_audio.c
        ${CMAKE_CURRENT_LIST_DIR}/src/stm32h745i_discovery_bus.c
        ${CMAKE_CURRENT_LIST_DIR}/src/stm32h745i_discovery_lcd.c
        ${CMAKE_CURRENT_LIST_DIR}/src/stm32h745i_discovery_mmc.c
        ${CMAKE_CURRENT_LIST_DIR}/src/stm32h745i_discovery_qspi.c
        ${CMAKE_CURRENT_LIST_DIR}/src/stm32h745i_discovery_sdram.c
        ${CMAKE_CURRENT_LIST_DIR}/src/stm32h745i_discovery_ts.c
)

add_definitions(-DUSE_STM32H745I_DISCO)


if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-drivers-bsp-disco-cm7 STATIC ${DISCO_SOURCES})
    target_include_directories(core-stm32h745i-drivers-bsp-disco-cm7 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
            ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco
    )
    target_link_libraries(core-stm32h745i-drivers-bsp-disco-cm7 PUBLIC
            core-stm32h745i-drivers-bsp-components-cm7
            core-stm32h745i-drivers-bsp-pdm-cm7
            core-stm32h745i-hsem-cm7
    )
endif()

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-drivers-bsp-disco-cm4 STATIC ${DISCO_SOURCES})
    target_include_directories(core-stm32h745i-drivers-bsp-disco-cm4 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
            ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/drivers/bsp/disco

    )
    target_link_libraries(core-stm32h745i-drivers-bsp-disco-cm4 PUBLIC
            core-stm32h745i-drivers-bsp-components-cm4
            core-stm32h745i-drivers-bsp-pdm-cm4
            core-stm32h745i-hsem-cm4
    )
endif()