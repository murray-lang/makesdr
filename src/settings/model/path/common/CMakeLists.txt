cmake_minimum_required(VERSION 4.1)

project(settings-model-path-common)

set(SETTINGS_MODEL_PATH_COMMON_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path/FieldEntry.h
        ${CMAKE_CURRENT_LIST_DIR}/src/CommonTagLookup.cpp
)

add_library(settings-model-path-common OBJECT ${SETTINGS_MODEL_PATH_COMMON_FILES})

target_include_directories(settings-model-path-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-path-common PUBLIC
        settings-model-message
        settings-model-proto
)