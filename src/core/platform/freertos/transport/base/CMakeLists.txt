cmake_minimum_required(VERSION 4.1)

project(core-freertos-transport-base)

set(FREERTOS_TRANSPORT_BASE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SemaphoreNotifier.h
)

add_library(core-freertos-transport-base INTERFACE ${FREERTOS_TRANSPORT_BASE_FILES})

target_link_libraries(core-freertos-transport-base INTERFACE
        core-freertos-thread
)
