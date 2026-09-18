add_executable(settings-control-transport-sink-tests
        TransportControlSinkTest.cpp
)

target_link_libraries(settings-control-transport-sink-tests
        PRIVATE
        GTest::gtest_main
        settings-control-transport-sink
        settings-model-radios-iq-dual-split
)

gtest_discover_tests(settings-control-transport-sink-tests)