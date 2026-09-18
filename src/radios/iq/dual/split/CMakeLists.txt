cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-split)

set(CMAKE_AUTOMOC ON)

set(DUAL_SPLIT_RADIO_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/SplitBandDualIqRadio.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqRadio.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RadioControl.cpp
)

add_library(radios-iq-dual-split STATIC ${DUAL_SPLIT_RADIO_FILES})

target_include_directories(radios-iq-dual-split PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(DUAL_SPLIT_RADIO_LIBS
        radios-base
        core-common-iq-dual-split
        settings-control-radio
        settings-model-radios-iq-dual-split
)

if(IS_QT)
    list(APPEND DUAL_SPLIT_RADIO_LIBS
            core-linux-transport-qt-event-radio
    )
endif()

if(IS_LINUX)
    list(APPEND DUAL_SPLIT_RADIO_LIBS
            core-linux-thread
    )
else()
    list(APPEND DUAL_SPLIT_RADIO_LIBS
            core-freertos-thread
    )
endif()

target_link_libraries(radios-iq-dual-split PUBLIC
        ${DUAL_SPLIT_RADIO_LIBS}
)