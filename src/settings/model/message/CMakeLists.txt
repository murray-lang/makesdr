cmake_minimum_required(VERSION 4.1)

project(settings-model-message)

set(MESSAGE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/IMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/PayloadBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/PayloadPurpose.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/MessageT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/PayloadType.h
)

add_library(settings-model-message INTERFACE ${MESSAGE_SOURCES})

target_include_directories(settings-model-message INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-message INTERFACE
        config-struct
        settings-model-proto
        etl::etl
)
