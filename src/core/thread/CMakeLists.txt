cmake_minimum_required(VERSION 4.1)

project(makesdr-core-thread)

set(THREAD_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Mutex.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Runnable.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Thread.h

)

if(TARGET_PLATFORM STREQUAL "linux-pc" OR TARGET_PLATFORM STREQUAL "rpi")
    set(THREAD_SOURCES
            ${THREAD_SOURCES}
            ${CMAKE_CURRENT_LIST_DIR}/platform/MutexStd.cpp
            ${CMAKE_CURRENT_LIST_DIR}/platform/ThreadStd.cpp
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i")
    set(THREAD_SOURCES
            ${THREAD_SOURCES}
            ${CMAKE_CURRENT_LIST_DIR}/platform/MutexFreeRTOS.cpp
            ${CMAKE_CURRENT_LIST_DIR}/platform/ThreadFreeRTOS.cpp
    )
endif()

add_library(makesdr-core-thread STATIC ${THREAD_SOURCES})

target_include_directories(makesdr-core-thread PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${INCLUDE_DIR}
)