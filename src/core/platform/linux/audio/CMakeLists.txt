cmake_minimum_required(VERSION 4.1)

project(core-linux-audio)

set(AUDIO_LINK_LIBS)
set(AUDIO_INCL_DIRS)

include(FindPkgConfig)

# Check for Jack (any OS)
find_library(JACK_LIB jack)
if(JACK_LIB OR jack_FOUND)
    message(STATUS "Jack found")
else()
    message(STATUS "Jack not found")
endif()

if (JACK_LIB OR jack_FOUND)
    if(jack_FOUND)
        list(APPEND AUDIO_LINK_LIBS ${JACK_LIBRARIES})
        list(APPEND AUDIO_INCL_DIRS ${JACK_INCLUDE_DIRS})
    elseif(JACK_LIB)
        list(APPEND AUDIO_LINK_LIBS ${JACK_LIB})
    endif()
endif()

# Check for Pulse (any OS)
pkg_check_modules(pulse libpulse-simple)

find_package(Threads REQUIRED
        CMAKE_THREAD_PREFER_PTHREAD
        THREADS_PREFER_PTHREAD_FLAG
)
list(APPEND AUDIO_LINK_LIBS Threads::Threads)
if(${pulse_FOUND})
    find_library(PULSE_LIB pulse)
    find_library(PULSESIMPLE_LIB pulse-simple)
    list(APPEND AUDIO_LINK_LIBS ${PULSE_LIB} ${PULSESIMPLE_LIB})
endif()

# ALSA
find_package(ALSA)
if (NOT ALSA_FOUND)
    message(FATAL_ERROR "ALSA API requested but no ALSA dev libraries found")
endif()
list(APPEND AUDIO_INCL_DIRS ${ALSA_INCLUDE_DIR})
list(APPEND AUDIO_LINK_LIBS ${ALSA_LIBRARIES})

# RtAudio
find_library(RTAUDIO_LIB rtaudio)
if(RTAUDIO_LIB)
    message(STATUS "RtAudio found")
    list(APPEND AUDIO_LINK_LIBS ${RTAUDIO_LIB})
else()
    message(FATAL_ERROR "RtAudio library not found")
endif()

set(LINUX_AUDIO_DRIVER_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/RtAudioFactoryBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioInputFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioOutputFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/RtAudioDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioInput.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioOutput.h

        ${CMAKE_CURRENT_LIST_DIR}/src/RtAudioFactoryBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/AudioInputFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/AudioOutputFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/AudioInput.cpp
)

add_library(core-linux-audio STATIC ${LINUX_AUDIO_DRIVER_FILES})
target_include_directories(core-linux-audio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${AUDIO_INCL_DIRS}
)
target_link_libraries(core-linux-audio PUBLIC
        ${AUDIO_LINK_LIBS}
        config-struct
        core-common-audio-base
        core-linux-thread
)
