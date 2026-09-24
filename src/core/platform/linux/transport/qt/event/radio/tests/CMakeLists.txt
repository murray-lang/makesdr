add_executable(core-linux-transport-qt-event-radio-tests
        QtRadioTransportTest.cpp
)

target_link_libraries(core-linux-transport-qt-event-radio-tests
        PRIVATE
        GTest::gtest
        core-linux-transport-qt-event-radio
        Qt6::Core
)

gtest_discover_tests(core-linux-transport-qt-event-radio-tests)
