cmake_minimum_required(VERSION 4.1)

project(settings-model-update-base)

set(MESSAGE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/FieldDescriptor.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/FieldPath.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/FieldUpdate.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/FieldUpdateMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/FieldUpdateSink.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/FieldUpdateSource.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/FieldUpdateVariant.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/MessageTraverser.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/ResolveDottedStringFunc.h

        ${CMAKE_CURRENT_LIST_DIR}/src/FieldDescriptor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FieldUpdate.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FieldUpdateMessage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/MessageTraverser.cpp
)

add_library(settings-model-update-base STATIC ${MESSAGE_SOURCES})

target_include_directories(settings-model-update-base PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-update-base PUBLIC
        settings-model-message
        etl::etl
)
