cmake_minimum_required(VERSION 4.1)

project(config-struct)


set(CONFIG_STRUCT_SOURCES
        include/config/struct/AudioConfig.h
        include/config/struct/AdcIqSourceConfig.h
        include/config/struct/AudioIqSourceConfig.h
        include/config/struct/AudioSignalIqSourceConfig.h
        include/config/struct/IqIoConfig.h
        include/config/struct/ConfigBase.h
        include/config/struct/BandSelectorConfig.h
        include/config/struct/DigitalInputsConfig.h
        include/config/struct/FunCubeConfig.h
        include/config/struct/BandSelectorBandsConfig.h
        include/config/struct/DigitalInputConfig.h
        include/config/struct/DigitalOutputsConfig.h
        include/config/struct/ControlConfig.h
        include/config/struct/DigitalOutputConfig.h
        include/config/struct/GpioLinesConfig.h
        include/config/struct/GpioInputLinesConfig.h
        include/config/struct/RadioConfig.h
        include/config/struct/IqReceiverConfig.h
        include/config/struct/QtTransportInConfig.h
        include/config/struct/QtTransportConfig.h
        include/config/struct/QtTransportOutConfig.h
        include/config/struct/SdrConfig.h
        include/config/struct/IqTransmitterConfig.h
        include/config/struct/FieldDescriptorConfig.h
        include/config/struct/SoftRockConfig.h
        include/config/struct/UiConfig.h
        include/config/struct/UsbControlSinksConfig.h
)

add_library(config-struct INTERFACE ${CONFIG_STRUCT_SOURCES})

target_include_directories(config-struct INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)