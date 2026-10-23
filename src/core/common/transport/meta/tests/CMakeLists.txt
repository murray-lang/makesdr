add_executable(core-common-transport-meta-tests
        MetaMessageRingsTest.cpp
        MetaSideRingsTest.cpp
)

target_include_directories(core-common-transport-meta-tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(core-common-transport-meta-tests
        PRIVATE
        GTest::gtest_main
        core-common-transport-meta
        settings-model-radio-basic-rx
)

gtest_discover_tests(core-common-transport-meta-tests)
