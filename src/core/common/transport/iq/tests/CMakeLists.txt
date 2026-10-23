add_executable(core-common-transport-iq-tests
        IqMessageRingsTest.cpp
        IqSideRingsTest.cpp
)

target_include_directories(core-common-transport-iq-tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(core-common-transport-iq-tests
        PRIVATE
        GTest::gtest_main
        core-common-transport-iq
        settings-model-radio-iq-basic-rx
)

gtest_discover_tests(core-common-transport-iq-tests)
