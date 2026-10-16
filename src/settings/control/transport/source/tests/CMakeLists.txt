add_executable(settings-control-transport-source-tests
        TransportControlSourceTest.cpp
)

target_link_libraries(settings-control-transport-source-tests
        PRIVATE
        GTest::gtest_main
        settings-control-transport-source
        radios-iq-dual-split-settings-model
)

gtest_discover_tests(settings-control-transport-source-tests)