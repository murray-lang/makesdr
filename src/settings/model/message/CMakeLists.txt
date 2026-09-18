cmake_minimum_required(VERSION 4.1)

project(settings-model-message)

set(MESSAGE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/FieldDescriptor.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/FieldPath.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/FieldUpdate.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/FieldUpdateMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/FieldUpdateSink.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/FieldUpdateSource.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/FieldUpdateVariant.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/IMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/MessageTraverser.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/PayloadBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/PayloadPurpose.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/MessageT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/PayloadType.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/ResolveDottedStringFunc.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/message/StringRef.h

        ${CMAKE_CURRENT_LIST_DIR}/src/FieldDescriptor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FieldUpdate.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FieldUpdateMessage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/MessageTraverser.cpp
)

add_library(settings-model-message STATIC ${MESSAGE_SOURCES})

target_include_directories(settings-model-message PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-message PUBLIC
        config-struct
        settings-model-proto
        etl::etl
)