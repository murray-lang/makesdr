cmake_minimum_required(VERSION 4.1)

project(settings-model-update-radios-basic-iq-rx-resolved)

set(RESOLVED_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/BasicIqRxResolved.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqRxResolved.cpp
)

add_library(settings-model-update-radios-basic-iq-rx-resolved STATIC ${RESOLVED_SOURCES})


target_include_directories(settings-model-update-radios-basic-iq-rx-resolved PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(settings-model-update-radios-basic-iq-rx-resolved PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path
)

target_link_libraries(settings-model-update-radios-basic-iq-rx-resolved PUBLIC
        settings-model-message
)