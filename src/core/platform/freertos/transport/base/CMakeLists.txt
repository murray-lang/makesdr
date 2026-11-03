cmake_minimum_required(VERSION 4.1)

project(core-freertos-transport-base)

set(FREERTOS_TRANSPORT_BASE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SemaphoreNotifier.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/freertos/TaskDispatcher.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/freertos/TaskNotifier.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/freertos/TaskNotify.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/freertos/TaskRecipientT.h
)

add_library(core-freertos-transport-base INTERFACE ${FREERTOS_TRANSPORT_BASE_FILES})

target_include_directories(core-freertos-transport-base INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-freertos-transport-base INTERFACE
        core-common-transport-base
        core-freertos-thread
)
