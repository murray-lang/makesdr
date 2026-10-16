cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rx-settings-model)

set(DUAL_IQ_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/model/DualIqRxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/model/DualIqRxUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxUpdater.cpp
)

add_library(radios-iq-dual-rx-settings-model STATIC ${DUAL_IQ_RX_FILES})

target_include_directories(radios-iq-dual-rx-settings-model PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(DUAL_IQ_RX_LIBS
        settings-model-radio-base
        settings-model-radio-iq-dual-common
        radios-iq-dual-rx-settings-update-resolved
        settings-model-meta-radio
)

if (USE_DOTTED_STRING_PATHS)
    set(DUAL_IQ_RX_LIBS
            ${DUAL_IQ_RX_LIBS}
            radios-iq-dual-rx-settings-update-lookup
            settings-model-update-util-resolve-one
    )
endif()

target_link_libraries(radios-iq-dual-rx-settings-model PUBLIC
        ${DUAL_IQ_RX_LIBS}
)