cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-iq-basic-rx)

set(BASIC_IQ_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/BasicIqRxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqRxSettings.cpp
)

add_library(settings-model-radios-iq-basic-rx STATIC ${BASIC_IQ_RX_FILES})

target_include_directories(settings-model-radios-iq-basic-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(BASIC_IQ_RX_LIBS
        settings-model-radios-base
        settings-model-radios-iq-basic-common
        settings-model-data-radio
)

if (USE_DOTTED_STRING_PATHS)
    set(BASIC_IQ_RX_LIBS
            ${BASIC_IQ_RX_LIBS}
            settings-model-path-radios-basic-iq-rx-lookup
            settings-model-path-util-resolve-one
    )
endif()

target_link_libraries(settings-model-radios-iq-basic-rx PUBLIC
        ${BASIC_IQ_RX_LIBS}
)