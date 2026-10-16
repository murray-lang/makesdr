cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rxtx-settings-model)

set(DUAL_IQ_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/model/DualIqRxTxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/model/DualIqRxTxUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTxSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTxUpdater.cpp
)

add_library(radios-iq-dual-rxtx-settings-model STATIC ${DUAL_IQ_RXTX_FILES})

target_include_directories(radios-iq-dual-rxtx-settings-model PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(DUAL_IQ_RXTX_LIBS
        settings-model-radio-base
        settings-model-radio-iq-dual-common
        settings-model-meta-radio
        radios-iq-dual-rxtx-settings-update-resolved
)

if (USE_DOTTED_STRING_PATHS)
    set(DUAL_IQ_RXTX_LIBS
            ${DUAL_IQ_RXTX_LIBS}
            radios-iq-dual-rxtx-settings-update-lookup
            settings-model-update-util-resolve-one
    )
endif()

target_link_libraries(radios-iq-dual-rxtx-settings-model PUBLIC
        ${DUAL_IQ_RXTX_LIBS}
)