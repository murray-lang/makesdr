cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-iq-source)

set(STM32_IQ_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/source/AdcIqSource.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/source/IqSourceFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/source/IqSourceVariant.h

        ${CMAKE_CURRENT_LIST_DIR}/src/AdcIqSource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/IqSourceFactory.cpp
)



add_library(core-stm32h745i-iq-source STATIC ${STM32_IQ_SOURCE_FILES})

# ADC_DMA_HALF_FRAMES
#   I/Q frames per half of AdcIqSource's circular DMA buffer. The DMA IRQ fires once per half, so
#   this trades interrupt rate (sample rate / ADC_DMA_HALF_FRAMES per second) against latency and
#   memory. The buffer is 2 * ADC_DMA_HALF_FRAMES * 4 bytes in the linker's .dma_buffer section
#   (DMA_RAM, 4K on the CM4, shared with any other DMA buffers placed there).
#   ADC_BUFFER_SIZE (the sample queue between the IRQ and the source thread) must be at least
#   2 * ADC_DMA_HALF_FRAMES.
set(ADC_DMA_HALF_FRAMES 256 CACHE STRING "I/Q frames per half of the ADC circular DMA buffer")
if (NOT ADC_DMA_HALF_FRAMES MATCHES "^[1-9][0-9]*$")
    message(FATAL_ERROR "ADC_DMA_HALF_FRAMES must be a positive integer, got '${ADC_DMA_HALF_FRAMES}'")
endif()

target_compile_definitions(core-stm32h745i-iq-source PUBLIC
        ADC_DMA_HALF_FRAMES=${ADC_DMA_HALF_FRAMES}
        SAMPLE_RATE=32000
        ADC_BUFFER_SIZE=512
        STACK_SIZE=1024
        THREAD_PRIORITY=6
)

target_include_directories(core-stm32h745i-iq-source PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    target_link_libraries(core-stm32h745i-iq-source PUBLIC
            core-common-iq-base
            core-stm32h745i-setup-setup3-cm4
            core-freertos-thread
            etl::etl
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    target_link_libraries(core-stm32h745i-iq-source PUBLIC
            core-common-iq-base
            core-stm32h745i-setup-setup3-cm7
            core-freertos-thread
            etl::etl
    )
endif()