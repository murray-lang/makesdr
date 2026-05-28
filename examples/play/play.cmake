cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-play)

set(SETTINGS_SOURCES)

add_executable(RadioSettingsProto-play
        ${SETTINGS_SOURCES}
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories(RadioSettingsProto-play PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(RadioSettingsProto-play PUBLIC
        RadioSettingsProto-settings-model-proto
        RadioSettingsProto-settings-model-basic
        RadioSettingsProto-settings-model-mutable
)