cmake_minimum_required(VERSION 4.1)

project(settings-control-factory)

set(CONTROL_FACTORY_SOURCES
        include/settings/control/factory/SettingsControlSinkFactory.h
        include/settings/control/factory/SettingsControlSinkVariant.h
        include/settings/control/factory/SettingsControlSourceFactory.h
        include/settings/control/factory/SettingsControlSourceVariant.h

        src/SettingsControlSinkFactory.cpp
        src/SettingsControlSourceFactory.cpp
)

add_library(settings-control-factory STATIC ${CONTROL_FACTORY_SOURCES})

target_include_directories(settings-control-factory PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(CONTROL_FACTORY_LIBS
        settings-control-hal-usb

        settings-control-hal-funcube
        settings-control-qt
        config-struct
        settings-control-base
        settings-control-sink
        settings-control-source
        settings-model-core
)

if(USE_GPIO)
    set(CONTROL_FACTORY_LIBS
            ${CONTROL_FACTORY_LIBS}
            settings-control-hal-gpio
    )
endif()

target_link_libraries(settings-control-factory PUBLIC
        ${CONTROL_FACTORY_LIBS}
)