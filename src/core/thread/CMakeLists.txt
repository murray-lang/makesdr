cmake_minimum_required(VERSION 3.25)

project(core-thread)

set(THREAD_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Mutex.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Runnable.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Thread.h

)

if(USE_FREERTOS)
    set(THREAD_SOURCES
            ${THREAD_SOURCES}
            ${CMAKE_CURRENT_LIST_DIR}/platform/MutexFreeRTOS.cpp
            ${CMAKE_CURRENT_LIST_DIR}/platform/ThreadFreeRTOS.cpp
    )
else()
    set(THREAD_SOURCES
            ${THREAD_SOURCES}
            ${CMAKE_CURRENT_LIST_DIR}/platform/MutexStd.cpp
            ${CMAKE_CURRENT_LIST_DIR}/platform/ThreadStd.cpp
    )
endif()

add_library(core-thread STATIC ${THREAD_SOURCES})

target_include_directories(core-thread PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${INCLUDE_DIR}
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    target_link_libraries(core-thread PUBLIC core-sys-stm32h745i-freertos-cm7)
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    target_link_libraries(core-thread PUBLIC core-sys-stm32h745i-freertos-cm4)
endif()