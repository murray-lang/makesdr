cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-config)

set(CONFIG_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/config/AudioConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/AudioIqSourceConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/audio/AudioConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/include/config/AudioSignalIqSourceConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/IqIoConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/ConfigBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/TypedJson.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/BandSelectorConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/DigitalInputsConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/FunCubeConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/QtControlSourceConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/BandSelectorBandsConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/DigitalInputConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/DigitalOutputsConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/QtControlSinkConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/ControlConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/DigitalOutputConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/GpioLinesConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/GpioInputLinesConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/RotaryEncoderConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/RadioConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/ReceiverConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/TransmitterConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/include/config/UiConfig.h


        ${CMAKE_CURRENT_LIST_DIR}/audio/AudioIqSourceConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/audio/AudioSignalIqSourceConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/audio/IqIoConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/base/ConfigBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/BandSelectorBandsConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/DigitalInputConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/DigitalOutputsConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/QtControlSinkConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/ControlConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/DigitalOutputConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/GpioLinesConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/GpioInputLinesConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/RotaryEncoderConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/BandSelectorConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/DigitalInputsConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/FunCubeConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/control/QtControlSourceConfig.cpp

        ${CMAKE_CURRENT_LIST_DIR}/radio/RadioConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/radio/ReceiverConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/radio/TransmitterConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ui/UiConfig.cpp
)

add_library(RadioSettingsProto-config STATIC ${CONFIG_SOURCES})

target_include_directories(RadioSettingsProto-config PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(RadioSettingsProto-config PUBLIC ArduinoJson etl)


