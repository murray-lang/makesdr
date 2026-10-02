cmake_minimum_required(VERSION 4.1)

project(settings-model-update-util-resolve-one)

set(SETTINGS_MODEL_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/resolveDottedString.h
        ${CMAKE_CURRENT_LIST_DIR}/src/resolveDottedString.cpp
)

add_library(settings-model-update-util-resolve-one STATIC ${SETTINGS_MODEL_LOOKUP_FILES})

target_include_directories(settings-model-update-util-resolve-one PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-update-util-resolve-one PUBLIC
        settings-model-update-base
        settings-model-update-common
        settings-model-proto
        settings-model-message
        etl::etl
)