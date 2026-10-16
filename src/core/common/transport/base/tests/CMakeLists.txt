add_executable(core-common-transport-base-tests
        MessageRingsTest.cpp
        MessageRingsAdaptersTest.cpp
)

target_link_libraries(core-common-transport-base-tests
        PRIVATE
        GTest::gtest_main
        core-common-transport-base
)

gtest_discover_tests(core-common-transport-base-tests)
