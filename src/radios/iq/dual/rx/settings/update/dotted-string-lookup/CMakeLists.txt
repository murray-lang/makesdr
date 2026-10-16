cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rx-settings-update-lookup)

set(DUAL_IQ_RX_TAG_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/update/DualIqRxTagLookup.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTagLookup.cpp
)

add_library(radios-iq-dual-rx-settings-update-lookup STATIC ${DUAL_IQ_RX_TAG_LOOKUP_FILES})

target_include_directories(radios-iq-dual-rx-settings-update-lookup PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-dual-rx-settings-update-lookup PUBLIC
        settings-model-update-common
)