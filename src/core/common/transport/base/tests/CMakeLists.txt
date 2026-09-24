add_executable(core-common-transport-base-tests
        MessageChannelTest.cpp
        MessageChannelAdaptersTest.cpp
)

target_link_libraries(core-common-transport-base-tests
        PRIVATE
        GTest::gtest_main
        core-common-transport-base
)

gtest_discover_tests(core-common-transport-base-tests)
