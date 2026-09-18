cmake_minimum_required(VERSION 4.1)

project(core-linux-event-data)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_DATA_EVENT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/event/EventDispatcher.h
        ${CMAKE_CURRENT_LIST_DIR}/include/event/FieldUpdateEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/event/QtEventRegistrar.h

        ${CMAKE_CURRENT_LIST_DIR}/include/event/sample/RxAudioEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/event/sample/TxAudioEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/event/sample/RxIqEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/event/sample/TxIqEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/event/metering/RxIqMeterEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/event/metering/RxMeterEvent.h
)

add_library(core-linux-event-data INTERFACE ${QT_DATA_EVENT_FILES})

target_include_directories(core-linux-event-data INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-event-data INTERFACE
        core-common-event
        settings-model-message
        Qt6::Core
)