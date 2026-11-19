cmake_minimum_required(VERSION 4.1)

project(core-common-audio-mixer)

set(AUDIO_MIXER_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/mixer/AudioMixer.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/mixer/AudioMixerInput.h

        ${CMAKE_CURRENT_LIST_DIR}/src/AudioMixer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/AudioMixerInput.cpp
)

add_library(core-common-audio-mixer STATIC ${AUDIO_MIXER_FILES})

# AUDIO_MIXER_MAX_FRAMES
#   Maximum mono audio frames per packet that each AudioMixer input can hold; larger packets are
#   truncated. Sizes all of the mixer's buffers: two mono inputs plus the interleaved stereo output,
#   4 * AUDIO_MIXER_MAX_FRAMES * sizeof(sdrreal) bytes in total.
#   Should be at least IqRxPipeline::getMaxFramesPerOutputPacket() for the configured sample rates.
#   Values above AUDIO_BUFFER_LENGTH (see core-common-samples) gain nothing, as the RX pipeline
#   can't deliver larger packets.
#   Code that needs a different size can use AudioMixerT<N> directly instead of AudioMixer.
set(AUDIO_MIXER_MAX_FRAMES 1024 CACHE STRING
        "Max mono frames per packet per AudioMixer input")
if (NOT AUDIO_MIXER_MAX_FRAMES MATCHES "^[1-9][0-9]*$")
    message(FATAL_ERROR "AUDIO_MIXER_MAX_FRAMES must be a positive integer, got '${AUDIO_MIXER_MAX_FRAMES}'")
endif()

target_compile_definitions(core-common-audio-mixer PUBLIC
        AUDIO_MIXER_MAX_FRAMES=${AUDIO_MIXER_MAX_FRAMES}
)

target_include_directories(core-common-audio-mixer PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-audio-mixer PUBLIC
        core-common-audio-base
        core-common-samples
)