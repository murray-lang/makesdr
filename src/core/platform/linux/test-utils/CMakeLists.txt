add_library(linux-test-utils STATIC
        ${CMAKE_CURRENT_LIST_DIR}/include/linux/test-utils/QtTransportTestRadio.h

        ${CMAKE_CURRENT_LIST_DIR}/src/QtTestMain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/QtTransportTestRadio.cpp
)

target_include_directories(linux-test-utils PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(linux-test-utils
        PUBLIC
        GTest::gtest  # Changed from gtest_main - Using QtTestMain
        test-utils
        core-linux-transport-qt-event-radio
        config-struct
        settings-model-radios-iq-dual-split
        settings-model-data-band
        settings-model-data-mode
        radios-base
        #        settings-model-data-radio
)
