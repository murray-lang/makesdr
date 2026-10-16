cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rxtx-settings-update-resolved)

set(RESOLVED_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/update/DualIqRxTxResolved.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTxResolved.cpp
)

add_library(radios-iq-dual-rxtx-settings-update-resolved STATIC ${RESOLVED_SOURCES})


target_include_directories(radios-iq-dual-rxtx-settings-update-resolved PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(radios-iq-dual-rxtx-settings-update-resolved PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/update
)

target_link_libraries(radios-iq-dual-rxtx-settings-update-resolved PUBLIC
        settings-model-message
        settings-model-update-base
)