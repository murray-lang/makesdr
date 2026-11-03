cmake_minimum_required(VERSION 4.1)

project(core-common-iq-io)

set(IQ_IO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/io/IqIo.h
        ${CMAKE_CURRENT_LIST_DIR}/src/IqIo.cpp
)

set(IQ_IO_LIBS
        config-struct
        core-common-audio-base
        core-common-iq-base
        core-common-samples
)

if(PLATFORM_FAMILY STREQUAL "linux")
    set(IQ_IO_LIBS
            ${IQ_IO_LIBS}
            core-linux-iq-source
            core-linux-audio
    )
elseif(PLATFORM_FAMILY STREQUAL "stm32h745i")
    set(IQ_IO_LIBS
            ${IQ_IO_LIBS}
#            core-freertos-thread
            core-stm32h745i-audio
            core-stm32h745i-iq-source
    )
endif()

add_library(core-common-iq-io STATIC ${IQ_IO_SOURCES})

target_include_directories(core-common-iq-io PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-common-iq-io PUBLIC ${IQ_IO_LIBS})