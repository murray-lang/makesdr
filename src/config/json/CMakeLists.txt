cmake_minimum_required(VERSION 4.1)

project(makesdr-config-json)

set(CONFIG_SOURCES
        include/config/json/AudioConfig.json.h
        include/config/json/AudioIqSourceConfig.json.h
        include/config/json/AudioSignalIqSourceConfig.json.h
        include/config/json/BandSelectorBandsConfig.json.h
        include/config/json/BandSelectorConfig.json.h
        include/config/json/ConfigBase.json.h
        include/config/json/DigitalInputConfig.json.h
        include/config/json/DigitalInputsConfig.json.h
        include/config/json/DigitalOutputConfig.json.h
        include/config/json/DigitalOutputsConfig.json.h
        include/config/json/FunCubeConfig.json.h
        include/config/json/GpioInputLinesConfig.json.h
        include/config/json/GpioLinesConfig.json.h
        include/config/json/IqIoConfig.json.h
        include/config/json/QtControlSinkConfig.json.h
        include/config/json/QtControlSourceConfig.json.h
        include/config/json/RadioConfig.json.h
        include/config/json/ReceiverConfig.json.h
        include/config/json/RotaryEncoderConfig.json.h
        include/config/json/TransmitterConfig.json.h
        include/config/json/TypedJson.h
        include/config/json/UiConfig.json.h

        src/audio/AudioConfig.json.cpp
        src/audio/AudioIqSourceConfig.json.cpp
        src/audio/AudioSignalIqSourceConfig.json.cpp
        src/audio/IqIoConfig.json.cpp
        src/base/ConfigBase.json.cpp
        src/control/BandSelectorBandsConfig.json.cpp
        src/control/DigitalInputConfig.json.cpp
        src/control/DigitalOutputsConfig.json.cpp
        src/control/QtControlSinkConfig.json.cpp
        src/control/ControlConfig.json.cpp
        src/control/DigitalOutputConfig.json.cpp
        src/control/GpioLinesConfig.json.cpp
        src/control/GpioInputLinesConfig.json.cpp
        src/control/RotaryEncoderConfig.json.cpp
        src/control/BandSelectorConfig.json.cpp
        src/control/DigitalInputsConfig.json.cpp
        src/control/FunCubeConfig.json.cpp
        src/control/QtControlSourceConfig.json.cpp

        src/radio/RadioConfig.json.cpp
        src/radio/ReceiverConfig.json.cpp
        src/radio/TransmitterConfig.json.cpp
        src/ui/UiConfig.json.cpp
)

add_library(makesdr-config-json STATIC ${CONFIG_SOURCES})

target_include_directories(makesdr-config-json PUBLIC
        ${INCLUDE_DIR}
#        ${ARDUINO_JSON_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(makesdr-config-json PUBLIC
        makesdr-config-struct
        ArduinoJson
        etl)


