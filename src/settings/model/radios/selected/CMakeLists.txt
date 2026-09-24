cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-selected)

set(RADIOS_SELECTED_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettings.h
)

add_library(settings-model-radios-selected INTERFACE ${RADIOS_SELECTED_FILES})

target_include_directories(settings-model-radios-selected INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# Must match the settings type chosen in RadioSettings.h
target_link_libraries(settings-model-radios-selected INTERFACE
        settings-model-radios-iq-dual-split
)
