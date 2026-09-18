cmake_minimum_required(VERSION 4.1)

project(radios-base)

set(RADIO_BASE_FILES
#        ${CMAKE_CURRENT_LIST_DIR}/include/radios/base/RadioBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/base/RadioBaseT.h
)

add_library(radios-base INTERFACE ${RADIO_BASE_FILES})

target_include_directories(radios-base INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-base INTERFACE
        settings-model-radios-base
        settings-control-sink
        core-common-event
)