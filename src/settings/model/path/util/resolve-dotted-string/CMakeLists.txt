cmake_minimum_required(VERSION 4.1)

project(settings-model-path-util-resolve-one)

set(SETTINGS_MODEL_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path/resolveDottedString.h
        ${CMAKE_CURRENT_LIST_DIR}/src/resolveDottedString.cpp
)

add_library(settings-model-path-util-resolve-one STATIC ${SETTINGS_MODEL_LOOKUP_FILES})

target_include_directories(settings-model-path-util-resolve-one PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-path-util-resolve-one PUBLIC
        settings-model-path-common
        settings-model-proto
        settings-model-message
        etl::etl
)