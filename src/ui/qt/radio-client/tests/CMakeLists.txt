add_executable(ui-qt-radio-client-tests
        QtRadioClientWithMocks.h
        QtRadioClientTest.cpp
)

# Enable automatic MOC processing
#set_target_properties(ui-qt-radio-client-tests PROPERTIES
#    AUTOMOC ON
#)

target_link_libraries(ui-qt-radio-client-tests
        PRIVATE
        GTest::gtest
        GTest::gmock
        ui-qt-radio-client
        settings-model-radios-iq-dual-split
        Qt6::Core
)

target_include_directories(ui-qt-radio-client-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

gtest_discover_tests(ui-qt-radio-client-tests)