add_executable(settings-control-radio-tests
        RadioControlTest.cpp
        linux-pc-RadioControlConfig.cpp
        linux-pc-RadioControlConfig.h
        RadioControl.h
        ExposedRadioControl.h
)

target_link_libraries(settings-control-radio-tests
        PRIVATE
        GTest::gtest_main
        settings-control-radio
        settings-model-radios-iq-dual-split
        config-json
        ArduinoJson
)

gtest_discover_tests(settings-control-radio-tests)