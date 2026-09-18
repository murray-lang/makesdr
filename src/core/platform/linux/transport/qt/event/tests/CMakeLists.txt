add_executable(core-linux-transport-qt-event-tests
        QtTransportTest.cpp
)

# Enable automatic MOC processing
set_target_properties(core-linux-transport-qt-event-tests PROPERTIES
        AUTOMOC ON
)

target_link_libraries(core-linux-transport-qt-event-tests
        PRIVATE
        linux-test-utils
)

gtest_discover_tests(core-linux-transport-qt-event-tests)