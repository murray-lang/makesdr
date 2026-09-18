cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-iq-dual-rx)

set(DUAL_IQ_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/DualIqRxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxSettings.cpp
)

add_library(settings-model-radios-iq-dual-rx STATIC ${DUAL_IQ_RX_FILES})

target_include_directories(settings-model-radios-iq-dual-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(DUAL_IQ_RX_LIBS
        settings-model-radios-base
        settings-model-radios-iq-dual-common
        settings-model-data-radio
)

if (USE_DOTTED_STRING_PATHS)
    set(DUAL_IQ_RX_LIBS
            ${DUAL_IQ_RX_LIBS}
            settings-model-path-radios-dual-iq-rx-lookup
            settings-model-path-util-resolve-one
    )
endif()

target_link_libraries(settings-model-radios-iq-dual-rx PUBLIC
        ${DUAL_IQ_RX_LIBS}
)