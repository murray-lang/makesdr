cmake_minimum_required(VERSION 4.1)

project(settings-model-path-radios-basic-rxtx-resolved)

set(RESOLVED_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path/BasicRxTxResolved.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicRxTxResolved.cpp
)

add_library(settings-model-path-radios-basic-rxtx-resolved STATIC ${RESOLVED_SOURCES})


target_include_directories(settings-model-path-radios-basic-rxtx-resolved PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(settings-model-path-radios-basic-rxtx-resolved PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path
)

target_link_libraries(settings-model-path-radios-basic-rxtx-resolved PUBLIC
        settings-model-message
)