cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-iq-single-rx)

set(BASIC_IQ_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/SingleIqRxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SingleIqRxSettings.cpp
)

add_library(settings-model-radio-iq-single-rx STATIC ${BASIC_IQ_RX_FILES})

target_include_directories(settings-model-radio-iq-single-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(BASIC_IQ_RX_LIBS
        settings-model-radio-base
        settings-model-radio-iq-single-common
        settings-model-meta-radio
)

if (USE_DOTTED_STRING_PATHS)
    set(BASIC_IQ_RX_LIBS
            ${BASIC_IQ_RX_LIBS}
            settings-model-update-radios-single-iq-rx-lookup
            settings-model-update-util-resolve-one
    )
endif()

target_link_libraries(settings-model-radio-iq-single-rx PUBLIC
        ${BASIC_IQ_RX_LIBS}
)