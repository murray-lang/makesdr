cmake_minimum_required(VERSION 4.1)

project(core-common-transport-base)

set(COMMON_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IMessageChannelConsumer.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IMessagePublisherT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IMessageSubscriberT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MessageChannelSinkT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MessageChannelSourceT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MessageChannelT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MessageRingT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/TransportBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/TransportInBaseT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/TransportOutBaseT.h
)

add_library(core-common-transport-base INTERFACE ${COMMON_TRANSPORT_FILES})

target_include_directories(core-common-transport-base INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-base INTERFACE
        settings-model-message
        etl::etl
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
