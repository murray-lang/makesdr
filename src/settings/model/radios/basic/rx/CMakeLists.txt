cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-basic-rx)

set(BASIC_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/basic/BasicRxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicRxSettings.cpp
)

add_library(settings-model-radios-basic-rx STATIC ${BASIC_RX_FILES})

target_include_directories(settings-model-radios-basic-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(BASIC_RX_LIBS
        settings-model-radios-base
        settings-model-radios-basic-common
        settings-model-data-radio
)

if (USE_DOTTED_STRING_PATHS)
    set(BASIC_RX_LIBS
            ${BASIC_RX_LIBS}
            settings-model-path-radios-basic-rx-lookup
            settings-model-path-util-resolve-one
    )
endif()

target_link_libraries(settings-model-radios-basic-rx PUBLIC
        ${BASIC_RX_LIBS}
)