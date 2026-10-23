cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rx)

set(DUAL_RX_RADIO_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/DualIqRxRadio.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxRadio.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RadioControl.cpp
)

add_library(radios-iq-dual-rx STATIC ${DUAL_RX_RADIO_FILES})

target_include_directories(radios-iq-dual-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(DUAL_RX_RADIO_LIBS
        radios-base
        radios-iq-dual-rx-iq
        settings-control-radio
        settings-model-radio-iq-dual-rx
        radios-iq-dual-rx-iq
)

if(IS_QT)
    list(APPEND DUAL_RX_RADIO_LIBS
            radios-iq-dual-rx-transport-qt-globals
            core-linux-transport-qt-radio-iq
            core-linux-transport-qt-settings
            core-linux-transport-qt-globals-iq
            core-linux-transport-qt-globals-meta
    )
endif()

if(IS_LINUX)
    list(APPEND DUAL_RX_RADIO_LIBS
            core-linux-thread
    )
else()
    list(APPEND DUAL_RX_RADIO_LIBS
            core-freertos-thread
    )
endif()

target_link_libraries(radios-iq-dual-rx PUBLIC
        ${DUAL_RX_RADIO_LIBS}
)
