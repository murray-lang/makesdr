cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-basic-rxtx)

set(BASIC_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/basic/BasicRxTxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicRxTxSettings.cpp
)

add_library(settings-model-radio-basic-rxtx STATIC ${BASIC_RXTX_FILES})

target_include_directories(settings-model-radio-basic-rxtx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(BASIC_RXTX_LIBS
        settings-model-radio-base
        settings-model-radio-basic-common
        settings-model-meta-radio
)

if (USE_DOTTED_STRING_PATHS)
set(BASIC_RXTX_LIBS
        ${BASIC_RXTX_LIBS}
        settings-model-update-radios-basic-rxtx-lookup
        settings-model-update-util-resolve-one
)
endif()

target_link_libraries(settings-model-radio-basic-rxtx PUBLIC
        ${BASIC_RXTX_LIBS}
)