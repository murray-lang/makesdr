cmake_minimum_required(VERSION 4.1)

project(core-common-samples)

set(SAMPLE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/samples/SampleTypes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/samples/PingPongBuffers.h
)

add_library(core-common-samples INTERFACE ${SAMPLE_SOURCES})

# AUDIO_BUFFER_LENGTH
#   Capacity, in samples (frames x channels), of AudioSamplesBuffer: the RX pipeline's demodulated
#   audio buffer. Per packet the demodulator produces
#     IQ frames x (audio rate / IQ rate) x demodulator output channels
#   samples, and anything beyond this capacity is lost. With 48 kHz sound-card IQ the rate ratio is
#   1, so the default matches PIPELINE_BUFFER_LENGTH. Targets with a higher IQ rate (e.g. STM32 ADC
#   IQ) can set it much lower.
set(AUDIO_BUFFER_LENGTH 2048 CACHE STRING
        "Capacity in samples of AudioSamplesBuffer (demodulated audio per packet)")
if (NOT AUDIO_BUFFER_LENGTH MATCHES "^[1-9][0-9]*$")
    message(FATAL_ERROR "AUDIO_BUFFER_LENGTH must be a positive integer, got '${AUDIO_BUFFER_LENGTH}'")
endif()

target_compile_definitions(core-common-samples INTERFACE
        AUDIO_BUFFER_LENGTH=${AUDIO_BUFFER_LENGTH}
        PIPELINE_BUFFER_LENGTH=2048 # Currently this needs to be no smaller than FILTER_FFT_SIZE
        FILTER_FFT_SIZE=2048
        MONITOR_FFT_SIZE=1024
)

target_include_directories(core-common-samples INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-common-samples INTERFACE etl::etl)