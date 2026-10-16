add_executable(settings-control-transport-sink-tests
        TransportControlSinkTest.cpp
)

target_link_libraries(settings-control-transport-sink-tests
        PRIVATE
        GTest::gtest_main
        settings-control-transport-sink
        radios-iq-dual-split-settings-model
)

gtest_discover_tests(settings-control-transport-sink-tests)