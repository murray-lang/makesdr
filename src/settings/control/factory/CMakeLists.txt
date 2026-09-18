cmake_minimum_required(VERSION 4.1)

project(settings-control-factory)

set(CONTROL_FACTORY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/factory/SettingsControlSinkFactoryT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/factory/SettingsControlSinkTypesT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/factory/SettingsControlSourceFactoryT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/factory/SettingsControlSourceTypesT.h
)

add_library(settings-control-factory INTERFACE ${CONTROL_FACTORY_SOURCES})

target_include_directories(settings-control-factory INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(CONTROL_FACTORY_LIBS
        config-struct
        settings-control-base
        settings-control-hal-usb-sinks
        settings-control-sink
        settings-control-source
)

if (PLATFORM_FAMILY STREQUAL "linux")
    set(CONTROL_FACTORY_LIBS
            ${CONTROL_FACTORY_LIBS}
            settings-control-transport-sink
            settings-control-transport-source
    )
endif()

if(USE_GPIO)
    set(CONTROL_FACTORY_LIBS
            ${CONTROL_FACTORY_LIBS}
            settings-control-hal-digital
    )
endif()

target_link_libraries(settings-control-factory INTERFACE
        ${CONTROL_FACTORY_LIBS}
)