cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-split-settings-model)

set(SPLIT_DUAL_IQ_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/model/SplitBandDualIqRxTxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/model/SplitBandDualIqRxTxUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqRxTxSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqRxTxUpdater.cpp
)

add_library(radios-iq-dual-split-settings-model STATIC ${SPLIT_DUAL_IQ_RXTX_FILES})

target_include_directories(radios-iq-dual-split-settings-model PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)


set(SPLIT_DUAL_IQ_RXTX_LIBS
        settings-model-radio-base
        settings-model-radio-iq-dual-common
        settings-model-meta-radio
        radios-iq-dual-split-settings-update-resolved
)

if (USE_DOTTED_STRING_PATHS)
    set(SPLIT_DUAL_IQ_RXTX_LIBS
            ${SPLIT_DUAL_IQ_RXTX_LIBS}
            radios-iq-dual-split-settings-update-lookup
            settings-model-update-util-resolve-one
            settings-model-update-base
    )
endif()
target_link_libraries(radios-iq-dual-split-settings-model PUBLIC
        ${SPLIT_DUAL_IQ_RXTX_LIBS}
)