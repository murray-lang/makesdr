cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-basic-rxtx)

set(BASIC_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/basic/BasicRxTxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicRxTxSettings.cpp
)

add_library(settings-model-radios-basic-rxtx STATIC ${BASIC_RXTX_FILES})

target_include_directories(settings-model-radios-basic-rxtx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(BASIC_RXTX_LIBS
        settings-model-radios-base
        settings-model-radios-basic-common
        settings-model-data-radio
)

if (USE_DOTTED_STRING_PATHS)
set(BASIC_RXTX_LIBS
        ${BASIC_RXTX_LIBS}
        settings-model-path-radios-basic-rxtx-lookup
        settings-model-path-util-resolve-one
)
endif()

target_link_libraries(settings-model-radios-basic-rxtx PUBLIC
        ${BASIC_RXTX_LIBS}
)