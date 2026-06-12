cmake_minimum_required(VERSION 3.25)

project(settings-model-util)

set(MODEL_UTIL_SOURCES

        include/settings/model/util/GenerateResolvedPathSourceFiles.h
        src/GenerateResolvedPathSourceFiles.cpp
)

add_library(settings-model-util
        STATIC
        ${MODEL_UTIL_SOURCES}
)

target_include_directories(settings-model-util PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-util PUBLIC
        settings-model-core
#        settings-model-basic

)
