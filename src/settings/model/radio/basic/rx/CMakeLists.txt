cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-basic-rx)

set(BASIC_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/basic/BasicRxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicRxSettings.cpp
)

add_library(settings-model-radio-basic-rx STATIC ${BASIC_RX_FILES})

target_include_directories(settings-model-radio-basic-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(BASIC_RX_LIBS
        settings-model-radio-base
        settings-model-radio-basic-common
        settings-model-meta-radio
)

if (USE_DOTTED_STRING_PATHS)
    set(BASIC_RX_LIBS
            ${BASIC_RX_LIBS}
            settings-model-update-radios-basic-rx-lookup
            settings-model-update-util-resolve-one
    )
endif()

target_link_libraries(settings-model-radio-basic-rx PUBLIC
        ${BASIC_RX_LIBS}
)