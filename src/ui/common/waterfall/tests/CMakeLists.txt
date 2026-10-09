add_executable(ui-common-waterfall-tests
        WaterfallTest.cpp
)

target_link_libraries(ui-common-waterfall-tests
        PRIVATE
        GTest::gtest_main
        ui-common-waterfall
)

gtest_discover_tests(ui-common-waterfall-tests)
