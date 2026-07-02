cmake_minimum_required(VERSION 4.1)

project(core-linux-thread)

find_package(Threads REQUIRED)

set(LINUX_THREAD_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Mutex.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Queue.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Runnable.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Semaphore.h
        ${CMAKE_CURRENT_LIST_DIR}/include/thread/Thread.h

        ${CMAKE_CURRENT_LIST_DIR}/src/MutexStd.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SemaphoreStd.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ThreadStd.cpp
)

add_library(core-linux-thread STATIC ${LINUX_THREAD_SOURCES})
target_include_directories(core-linux-thread PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(core-linux-thread PUBLIC Threads::Threads)