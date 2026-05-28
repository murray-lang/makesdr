cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-example-save)

add_executable(RadioSettingsProto-example-save
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../data/exampleRadioSettings.cpp

)

target_include_directories(RadioSettingsProto-example-save PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(RadioSettingsProto-example-save PRIVATE
        RadioSettingsProto-settings-model-proto
        RadioSettingsProto-settings-model-core
)