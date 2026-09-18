cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-iq-basic-rxtx)

set(BASIC_IQ_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/BasicIqRxTxSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqRxTxSettings.cpp
)

add_library(settings-model-radios-iq-basic-rxtx STATIC ${BASIC_IQ_RXTX_FILES})

target_include_directories(settings-model-radios-iq-basic-rxtx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(BASIC_IQ_RXTX_LIBS
        settings-model-radios-base
        settings-model-radios-iq-basic-common
        settings-model-data-radio
)

if (USE_DOTTED_STRING_PATHS)
    set(BASIC_IQ_RXTX_LIBS
            ${BASIC_IQ_RXTX_LIBS}
            settings-model-path-radios-basic-iq-rxtx-lookup
            settings-model-path-util-resolve-one
    )
endif()

target_link_libraries(settings-model-radios-iq-basic-rxtx PUBLIC
        ${BASIC_IQ_RXTX_LIBS}
)