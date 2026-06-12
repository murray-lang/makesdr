cmake_minimum_required(VERSION 3.25)

project(config-struct)


set(CONFIG_STRUCT_SOURCES
        include/config/struct/AudioConfig.h
        include/config/struct/AudioIqSourceConfig.h
        include/config/struct/AudioSignalIqSourceConfig.h
        include/config/struct/IqIoConfig.h
        include/config/struct/ConfigBase.h

        include/config/struct/BandSelectorConfig.h
        include/config/struct/DigitalInputsConfig.h
        include/config/struct/FunCubeConfig.h
        include/config/struct/QtControlSourceConfig.h
        include/config/struct/BandSelectorBandsConfig.h
        include/config/struct/DigitalInputConfig.h
        include/config/struct/DigitalOutputsConfig.h
        include/config/struct/QtControlSinkConfig.h
        include/config/struct/ControlConfig.h
        include/config/struct/DigitalOutputConfig.h
        include/config/struct/GpioLinesConfig.h
        include/config/struct/GpioInputLinesConfig.h
        include/config/struct/RotaryEncoderConfig.h
        include/config/struct/RadioConfig.h
        include/config/struct/ReceiverConfig.h
        include/config/struct/TransmitterConfig.h
        include/config/struct/UiConfig.h
)

add_library(config-struct INTERFACE ${CONFIG_STRUCT_SOURCES})

target_include_directories(config-struct INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)