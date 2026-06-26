cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-thread)

set(STM32_THREAD_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Mutex.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Queue.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Runnable.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Semaphore.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Thread.h

        ${CMAKE_CURRENT_LIST_DIR}/src/MutexFreeRTOS.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SemaphoreFreeRTOS.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ThreadFreeRTOS.cpp
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-thread-cm4 STATIC ${STM32_THREAD_SOURCES})
    target_include_directories(core-stm32h745i-thread-cm4 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(core-stm32h745i-thread-cm4 PUBLIC
            core-stm32h745i-freertos-cm4
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-thread-cm7 STATIC ${STM32_THREAD_SOURCES})
    target_include_directories(core-stm32h745i-thread-cm7 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(core-stm32h745i-thread-cm7 PUBLIC
            core-stm32h745i-freertos-cm7
    )
endif()