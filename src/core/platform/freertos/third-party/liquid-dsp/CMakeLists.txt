cmake_minimum_required(VERSION 4.1)

project(core-freertos-liquid-dsp)

set(BUILD_EXAMPLES OFF)
set(BUILD_AUTOTESTS OFF)
set(BUILD_BENCHMARKS OFF)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)
set(FIND_FFTW OFF)
set(ENABLE_LOGGING OFF)
# Even with logging disabled, liquid printf()s anything >= info, such as
# "liquid_runtime_cores(), could not get core count; unknown/unsupported OS"
# on every dotprod creation (it falls back to 1 core, which is correct here).
# That bypasses the shared UART's HSEM lock, so compile out all but errors.
set(LOGGING_LEVEL "error" CACHE STRING "" FORCE)

# Send liquid-dsp's heap calls to the FreeRTOS heap. This must come before
# add_subdirectory() so that liquid's OBJECT library targets inherit it.
add_compile_definitions(
        malloc=pvPortMalloc
        calloc=liquid_freertos_calloc  # FreeRTOS has no pvPortCalloc
        free=vPortFree
        posix_memalign=liquid_freertos_posix_memalign  # not in newlib for arm-none-eabi
)

add_subdirectory(${LIQUID_DSP_DIR} ${CMAKE_BINARY_DIR}/freertos-liquid-dsp-${TARGET_PLATFORM})

set(LIQUID_OVERRIDE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/liquid_freertos_calloc.c
        ${CMAKE_CURRENT_LIST_DIR}/src/liquid_freertos_posix_memalign.c
)

# The overrides live in their own library, linked *after* liquid-static below.
# libliquid.a references these symbols, and the linker only resolves a static
# lib's undefined symbols against libs that come later on the link line.
# (Making liquid-static depend on this target instead trips liquid's
# install(EXPORT)/export() checks.)
add_library(core-freertos-liquid-dsp-overrides STATIC ${LIQUID_OVERRIDE_SOURCES})

# Header-only wrapper that consumers link against.
add_library(core-freertos-liquid-dsp INTERFACE)

target_include_directories(core-freertos-liquid-dsp INTERFACE
        ${LIQUID_DSP_DIR}/include
        ${CMAKE_BINARY_DIR}/freertos-liquid-dsp-${TARGET_PLATFORM}
)

# With BUILD_SHARED_LIBS OFF, liquid-dsp only defines the static target
# "liquid-static" (output name libliquid.a). A bare "liquid" isn't a target,
# so CMake would emit -lliquid and the linker would search for a system lib.
if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    set(LIQUID_FREERTOS_LIB core-stm32h745i-cm7-freertos)
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    set(LIQUID_FREERTOS_LIB core-stm32h745i-cm4-freertos)
endif()

target_link_libraries(core-freertos-liquid-dsp-overrides PRIVATE ${LIQUID_FREERTOS_LIB})

target_link_libraries(core-freertos-liquid-dsp INTERFACE
        ${LIQUID_FREERTOS_LIB}
        liquid::liquid-static
        core-freertos-liquid-dsp-overrides   # must follow liquid-static
)
