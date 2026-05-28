cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-example-restore)

add_executable(RadioSettingsProto-example-restore
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories(RadioSettingsProto-example-restore PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(RadioSettingsProto-example-restore PRIVATE
        RadioSettingsProto-settings-model-proto
        RadioSettingsProto-settings-model-core
)