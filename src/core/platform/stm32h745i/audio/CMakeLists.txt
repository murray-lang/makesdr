cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-audio)

set (STM32_AUDIO_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioOutputVariant.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioOutputFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/SaiAudio.h
        ${CMAKE_CURRENT_LIST_DIR}/src/AudioOutputFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SaiAudio.cpp
)

add_library(core-stm32h745i-audio STATIC ${STM32_AUDIO_FILES})

# SAIAUDIO_DMA_HALF_FRAMES
#   Frames per half of SaiAudio's circular DMA buffer. The DMA IRQ fires once per half (every
#   SAIAUDIO_DMA_HALF_FRAMES / 48000 s). The buffer is 2 * SAIAUDIO_DMA_HALF_FRAMES * 4 bytes in the
#   linker's .dma_buffer section (DMA_RAM, 4K on the CM4, shared with AdcIqSource's buffer).
# SAIAUDIO_OUT_QUEUE_FRAMES
#   Stereo frames (4 bytes each) queued between the RX pipeline and the output DMA IRQ. It must
#   hold more than one pipeline output packet plus one DMA half. The extra absorbs the slow drift
#   between the ADC's sample clock (TIM6) and the SAI's (PLL2P).
set(SAIAUDIO_DMA_HALF_FRAMES 256 CACHE STRING "Frames per half of the SAI audio circular DMA buffer")
set(SAIAUDIO_OUT_QUEUE_FRAMES 3072 CACHE STRING "Stereo frames queued for SAI audio output")
foreach(VAR SAIAUDIO_DMA_HALF_FRAMES SAIAUDIO_OUT_QUEUE_FRAMES)
    if (NOT ${VAR} MATCHES "^[1-9][0-9]*$")
        message(FATAL_ERROR "${VAR} must be a positive integer, got '${${VAR}}'")
    endif()
endforeach()

target_compile_definitions(core-stm32h745i-audio PUBLIC
        SAIAUDIO_DMA_HALF_FRAMES=${SAIAUDIO_DMA_HALF_FRAMES}
        SAIAUDIO_OUT_QUEUE_FRAMES=${SAIAUDIO_OUT_QUEUE_FRAMES}
)

target_include_directories(core-stm32h745i-audio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-stm32h745i-audio PUBLIC
        config-struct
        core-common-audio-base
        core-common-samples
        core-freertos-thread
        etl::etl
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    target_link_libraries(core-stm32h745i-audio PUBLIC core-stm32h745i-drivers-bsp-disco-cm4)
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    target_link_libraries(core-stm32h745i-audio PUBLIC core-stm32h745i-drivers-bsp-disco-cm7)
endif()
