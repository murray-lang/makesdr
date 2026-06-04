cmake_minimum_required(VERSION 4.1)

project(makesdr-settings-model-util)

set(MODEL_UTIL_SOURCES

        include/settings/model/util/GenerateResolvedPathSourceFiles.h
        src/GenerateResolvedPathSourceFiles.cpp

        include/settings/model/util/defaultRadioCategories.h
        src/defaultRadioCategories.cpp
)

add_library(makesdr-settings-model-util
        STATIC
        ${MODEL_UTIL_SOURCES}
)

target_include_directories(makesdr-settings-model-util PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(makesdr-settings-model-util PUBLIC
        makesdr-settings-model-core
#        makesdr-settings-model-basic

)
