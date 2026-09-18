cmake_minimum_required(VERSION 4.1)

project(settings-model-data-mode)

set(MODE_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/mode/basicModes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/mode/ModeList.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/mode/ModeTypes.h

        ${CMAKE_CURRENT_LIST_DIR}/src/basicModes.cpp
)

add_library(settings-model-data-mode STATIC ${MODE_LOOKUP_FILES})

target_include_directories(settings-model-data-mode PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-data-mode PUBLIC
        settings-model-proto
        settings-model-message
        settings-model-radios-base
        etl::etl
)
