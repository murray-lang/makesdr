cmake_minimum_required(VERSION 4.1)

project(core-common-transport-settings)

set(SETTINGS_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SettingsClientSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SettingsMessageRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SettingsRadioSideRingsT.h

 )

add_library(core-common-transport-settings INTERFACE ${SETTINGS_TRANSPORT_FILES})

target_include_directories(core-common-transport-settings INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-settings INTERFACE
        core-common-transport-base
)

#if(BUILD_TESTING)
#    add_subdirectory(tests)
#endif()
