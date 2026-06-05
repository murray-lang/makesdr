cmake_minimum_required(VERSION 3.25)

project(makesdr-settings-control-factory)

set(CONTROL_FACTORY_SOURCES
        include/settings/control/factory/SettingsControlSinkFactory.h
        include/settings/control/factory/SettingsControlSinkVariant.h
        include/settings/control/factory/SettingsControlSourceFactory.h
        include/settings/control/factory/SettingsControlSourceVariant.h

        src/SettingsControlSinkFactory.cpp
        src/SettingsControlSourceFactory.cpp
)

add_library(makesdr-settings-control-factory STATIC ${CONTROL_FACTORY_SOURCES})

target_include_directories(makesdr-settings-control-factory PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(CONTROL_FACTORY_LIBS
        makesdr-settings-control-hal-usb

        makesdr-settings-control-hal-funcube
        makesdr-settings-control-qt
        makesdr-config-struct
        makesdr-settings-control-base
        makesdr-settings-control-sink
        makesdr-settings-control-source
        makesdr-settings-model-core
)

if(USE_GPIO)
    set(CONTROL_FACTORY_LIBS
            ${CONTROL_FACTORY_LIBS}
            makesdr-settings-control-hal-gpio
    )
endif()

target_link_libraries(makesdr-settings-control-factory PUBLIC
        ${CONTROL_FACTORY_LIBS}
)