cmake_minimum_required(VERSION 4.1)

project(core-common-transport-base)

set(BASE_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IMessageRingConsumer.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IMessagePublisherT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IMessageSubscriberT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MessageRingSinkT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MessageRingSourceT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MessageRingT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MessageRingBufferT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/RecipientSideT.h
 )

add_library(core-common-transport-base INTERFACE ${BASE_TRANSPORT_FILES})

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
