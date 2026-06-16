cmake_minimum_required(VERSION 4.1)

project(example-save)

add_executable(example-save
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp

)

target_include_directories(example-save PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-save PRIVATE
        settings-model-proto
        settings-model-core
)