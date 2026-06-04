cmake_minimum_required(VERSION 4.1)

project(makesdr-example-save)

add_executable(makesdr-example-save
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp

)

target_include_directories(makesdr-example-save PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(makesdr-example-save PRIVATE
        makesdr-settings-model-proto
        makesdr-settings-model-core
)