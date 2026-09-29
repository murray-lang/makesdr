cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-iq-dual-rxtx)

set(DUAL_IQ_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/DualIqRxTxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTxSettings.cpp
)

add_library(settings-model-radio-iq-dual-rxtx STATIC ${DUAL_IQ_RXTX_FILES})

target_include_directories(settings-model-radio-iq-dual-rxtx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(DUAL_IQ_RXTX_LIBS
        settings-model-radio-base
        settings-model-radio-iq-dual-common
        settings-model-meta-radio
)

if (USE_DOTTED_STRING_PATHS)
    set(DUAL_IQ_RXTX_LIBS
            ${DUAL_IQ_RXTX_LIBS}
            settings-model-update-radios-dual-iq-rxtx-lookup
            settings-model-update-util-resolve-one
    )
endif()

target_link_libraries(settings-model-radio-iq-dual-rxtx PUBLIC
        ${DUAL_IQ_RXTX_LIBS}
)