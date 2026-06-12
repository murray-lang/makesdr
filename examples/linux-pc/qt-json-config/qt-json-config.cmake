cmake_minimum_required(VERSION 3.25)

project(example-qt-json-config VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/qt.cmake)

list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")

find_package(PkgConfig)
if(USE_GPIO)
    pkg_check_modules(GPIOD IMPORTED_TARGET libgpiod)
    if(GPIOD_FOUND)
        message(STATUS "libgpiod found")
        add_definitions(-DUSE_GPIOD)
        set(USE_GPIOD ON)
    else()
        message(FATAL_ERROR "USE_GPIO is set to true, but libgpiod not found")
    endif()
    add_definitions(-DMAX_GPIO=27)
endif()

set (PROJECT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

add_executable(example-qt-json-config
        ${PROJECT_SOURCES}
)

set(PROJECT_LIBRARIES
        ${LIQUID_LIBRARY}
        ${AUDIO_DRIVER_LIBS}
        ${AUDIO_LINK_LIBS}
        usb-1.0
        hidapi-libusb
        Qt${QT_VERSION_MAJOR}::Core

        config-struct
        config-json
        settings-control-radio
        ArduinoJson

)

if(USE_GPIO)
    set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} PkgConfig::GPIOD)
endif()

if(USE_ETL)
    set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} etl::etl)
endif()

target_include_directories(example-qt-json-config PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-qt-json-config PRIVATE ${PROJECT_LIBRARIES})

#set_target_properties(example-qt-json-config PROPERTIES
#        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#        MACOSX_BUNDLE TRUE
#        WIN32_EXECUTABLE TRUE
#        AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}"
#)
#
#install(TARGETS example-qt-json-config
#        BUNDLE DESTINATION .
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
#
#qt_finalize_executable(example-qt-json-config)
