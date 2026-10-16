add_executable(core-common-transport-basic-tests
        BasicMessageRingsTest.cpp
        BasicSideRingsTest.cpp
)

target_include_directories(core-common-transport-basic-tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(core-common-transport-basic-tests
        PRIVATE
        GTest::gtest_main
        core-common-transport-basic
        settings-model-radio-basic-rx
)

gtest_discover_tests(core-common-transport-basic-tests)
