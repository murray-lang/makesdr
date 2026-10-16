cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rxtx-settings-update-lookup)

set(DUAL_IQ_RXTX_TAG_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/update/DualIqRxTxTagLookup.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTxTagLookup.cpp
)

add_library(radios-iq-dual-rxtx-settings-update-lookup STATIC ${DUAL_IQ_RXTX_TAG_LOOKUP_FILES})

target_include_directories(radios-iq-dual-rxtx-settings-update-lookup PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-dual-rxtx-settings-update-lookup PUBLIC
        settings-model-update-common
)