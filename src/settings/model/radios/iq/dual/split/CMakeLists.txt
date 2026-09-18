cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-iq-dual-split)

set(SPLIT_DUAL_IQ_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/SplitBandDualIqRxTxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/SplitBandDualIqRxTxUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqRxTxSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqRxTxUpdater.cpp
)

add_library(settings-model-radios-iq-dual-split STATIC ${SPLIT_DUAL_IQ_RXTX_FILES})

target_include_directories(settings-model-radios-iq-dual-split PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)


set(SPLIT_DUAL_IQ_RXTX_LIBS
        settings-model-radios-base
        settings-model-radios-iq-dual-common
        settings-model-data-radio
        settings-model-path-radios-dual-iq-split-resolved
)

if (USE_DOTTED_STRING_PATHS)
    set(SPLIT_DUAL_IQ_RXTX_LIBS
            ${SPLIT_DUAL_IQ_RXTX_LIBS}
            settings-model-path-radios-dual-iq-split-lookup
            settings-model-path-util-resolve-one
    )
endif()
target_link_libraries(settings-model-radios-iq-dual-split PUBLIC
        ${SPLIT_DUAL_IQ_RXTX_LIBS}
)