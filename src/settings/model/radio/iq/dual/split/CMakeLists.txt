cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-iq-dual-split)

set(SPLIT_DUAL_IQ_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/SplitBandDualIqRxTxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/SplitBandDualIqRxTxUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqRxTxSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqRxTxUpdater.cpp
)

add_library(settings-model-radio-iq-dual-split STATIC ${SPLIT_DUAL_IQ_RXTX_FILES})

target_include_directories(settings-model-radio-iq-dual-split PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)


set(SPLIT_DUAL_IQ_RXTX_LIBS
        settings-model-radio-base
        settings-model-radio-iq-dual-common
        settings-model-meta-radio
        settings-model-update-radios-dual-iq-split-resolved
)

if (USE_DOTTED_STRING_PATHS)
    set(SPLIT_DUAL_IQ_RXTX_LIBS
            ${SPLIT_DUAL_IQ_RXTX_LIBS}
            settings-model-update-radios-dual-iq-split-lookup
            settings-model-update-util-resolve-one
    )
endif()
target_link_libraries(settings-model-radio-iq-dual-split PUBLIC
        ${SPLIT_DUAL_IQ_RXTX_LIBS}
)