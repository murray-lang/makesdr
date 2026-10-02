add_library(test-utils STATIC
        ${CMAKE_CURRENT_LIST_DIR}/include/test-utils/testRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/testRadioSettings.cpp
)

target_include_directories(test-utils PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(test-utils
        PUBLIC
        GTest::gtest  # Changed from gtest_main - Using QtTestMain
        config-struct
        settings-model-radio-iq-dual-split
        settings-model-meta-band
        settings-model-meta-mode
        radios-base
)
