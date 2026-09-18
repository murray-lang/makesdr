add_executable(settings-control-transport-source-tests
        TransportControlSourceTest.cpp
)

target_link_libraries(settings-control-transport-source-tests
        PRIVATE
        GTest::gtest_main
        settings-control-transport-source
        settings-model-radios-iq-dual-split
)

gtest_discover_tests(settings-control-transport-source-tests)