add_executable(core-linux-transport-qt-event-out-tests
        QtTransportOutTest.cpp
)

target_link_libraries(core-linux-transport-qt-event-out-tests
        PRIVATE
        GTest::gtest_main
        core-linux-transport-qt-event-out
        settings-model-radios-iq-dual-split
        Qt6::Core
)

gtest_discover_tests(core-linux-transport-qt-event-out-tests)