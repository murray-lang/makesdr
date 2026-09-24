add_executable(core-linux-transport-qt-globals-tests
        QtMessageExchangeTest.cpp
)

target_link_libraries(core-linux-transport-qt-globals-tests
        PRIVATE
        GTest::gtest
        core-linux-transport-qt-globals
        Qt6::Core
)

gtest_discover_tests(core-linux-transport-qt-globals-tests)
