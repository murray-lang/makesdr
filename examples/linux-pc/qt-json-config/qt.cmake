cmake_minimum_required(VERSION 4.1)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --enable-threads")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --enable-threads")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(PkgConfig)

find_path(LIQUID_INCLUDE_DIR NAMES liquid/liquid.h) # TODO modify this per TARGET_PLATFORM
find_library(LIQUID_LIBRARY NAMES liquid)

if(LIQUID_INCLUDE_DIR AND LIQUID_LIBRARY)
    message(STATUS "Found liquid-dsp: ${LIQUID_LIBRARY}")
else()
    message(FATAL_ERROR "liquid-dsp not found!")
endif()

find_package(RtAudio REQUIRED)

if(USE_GPIO)
    pkg_check_modules(GPIOD IMPORTED_TARGET libgpiod)
    if(GPIOD_FOUND)
        message(STATUS "libgpiod found")
        add_definitions(-DUSE_GPIOD)
        set(USE_GPIOD ON)
    else()
        message(FATAL_ERROR "USE_GPIO is set to true, but libgpiod not found")
    endif()
    set(USE_PIGPIO OFF) #PIGPIO causes problems with modern RPi distros. Keeping reference just in case
endif()

include(${CMAKE_CURRENT_LIST_DIR}/audio-libs.cmake)

message(STATUS "${AUDIO_LINK_LIBS}")

find_library(HIDAPI_LIBRARY hidapi-libusb)
find_package(USB)
if(NOT USB_FOUND)
    message(FATAL_ERROR "libusb required to compile nexusdr")
endif()
if(HIDAPI_LIBRARY)
    MESSAGE(STATUS "System Hidapi Lib ${HIDAPI_LIBRARY} is used")
else()
    MESSAGE(FATAL_ERROR "Hidapi required to compile nexusdr")
endif()

if (USE_GUI)
    find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Charts)
    #find_package(Qwt REQUIRED)
    #if(QWT_FOUND)
    #    message("Qwt found")
    #else()
    #    message("Qwt NOT found")
    #endif()
else()
    find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
    if(QT_FOUND)
        message("Qt found")
    else()
        message("Qt NOT found")
    endif()
endif()