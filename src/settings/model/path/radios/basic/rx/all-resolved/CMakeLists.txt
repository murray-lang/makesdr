cmake_minimum_required(VERSION 4.1)

project(settings-model-path-radios-basic-rx-resolved)

set(RESOLVED_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path/BasicRxResolved.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicRxResolved.cpp
)

add_library(settings-model-path-radios-basic-rx-resolved STATIC ${RESOLVED_SOURCES})


target_include_directories(settings-model-path-radios-basic-rx-resolved PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(settings-model-path-radios-basic-rx-resolved PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path
)

target_link_libraries(settings-model-path-radios-basic-rx-resolved PUBLIC
        settings-model-message
)