add_executable(core-linux-transport-qt-event-in-tests
        QtTransportInTest.cpp
)

target_link_libraries(core-linux-transport-qt-event-in-tests
        PRIVATE
        GTest::gtest_main
        core-linux-transport-qt-event-in
        settings-model-radios-iq-dual-split
        Qt6::Core
)

gtest_discover_tests(core-linux-transport-qt-event-in-tests)