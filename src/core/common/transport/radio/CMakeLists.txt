cmake_minimum_required(VERSION 4.1)

project(core-common-transport-radio)

set(RADIO_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IRadioPublishers.h
)

add_library(core-common-transport-radio INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IRadioPublishers.h
)

target_include_directories(core-common-transport-radio INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-radio INTERFACE
        core-common-transport-iq
        core-common-transport-metering
        core-common-transport-spectrum
)

#if(BUILD_TESTING)
#    add_subdirectory(tests)
#endif()
