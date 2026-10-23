cmake_minimum_required(VERSION 4.1)

project(radios-iq-single-rx)

set(RADIO_IQ_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/SingleIqRxRadio.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SingleIqRxRadio.cpp
)

add_library(radios-iq-single-rx STATIC ${RADIO_IQ_RX_FILES})

target_include_directories(radios-iq-single-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(SINGLE_RX_RADIO_LIBS
        radios-base
        radios-iq-single-rx-iq
        settings-control-radio
        settings-model-radio-iq-single-rx
)

if(IS_QT)
    list(APPEND SINGLE_RX_RADIO_LIBS
            radios-iq-single-rx-transport-qt-globals
            core-linux-transport-qt-radio-iq
            core-linux-transport-qt-settings
            core-linux-transport-qt-globals-iq
            core-linux-transport-qt-globals-meta
    )
endif()

if(IS_LINUX)
    list(APPEND SINGLE_RX_RADIO_LIBS
            core-linux-thread
    )
else()
    list(APPEND SINGLE_RX_RADIO_LIBS
            core-freertos-thread
    )
endif()

target_link_libraries(radios-iq-single-rx PUBLIC ${SINGLE_RX_RADIO_LIBS} )