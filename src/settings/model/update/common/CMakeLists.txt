cmake_minimum_required(VERSION 4.1)

project(settings-model-update-common)

set(SETTINGS_MODEL_PATH_COMMON_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/FieldEntry.h
        ${CMAKE_CURRENT_LIST_DIR}/src/CommonTagLookup.cpp
)

add_library(settings-model-update-common OBJECT ${SETTINGS_MODEL_PATH_COMMON_FILES})

target_include_directories(settings-model-update-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-update-common PUBLIC
        settings-model-message
        settings-model-proto
)