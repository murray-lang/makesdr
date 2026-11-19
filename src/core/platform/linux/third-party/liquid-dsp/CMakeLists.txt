cmake_minimum_required(VERSION 4.1)

project(core-linux-liquid-dsp)

set(BUILD_EXAMPLES OFF)
set(BUILD_AUTOTESTS OFF)
set(BUILD_BENCHMARKS OFF)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)
set(FIND_FFTW OFF)
set(ENABLE_LOGGING OFF)

add_subdirectory(${LIQUID_DSP_DIR} ${CMAKE_BINARY_DIR}/liquid-dsp-linux)

add_library(core-linux-liquid-dsp STATIC src/dummy.c)

target_include_directories(core-linux-liquid-dsp PUBLIC
        ${LIQUID_DSP_DIR}/include
        ${CMAKE_BINARY_DIR}/liquid-dsp-linux
)

target_link_libraries(core-linux-liquid-dsp PUBLIC liquid-static)
