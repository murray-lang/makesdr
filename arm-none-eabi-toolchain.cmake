cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Generic-ELF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(TOOL_PREFIX "arm-none-eabi-")
set(CMAKE_C_COMPILER ${TOOL_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOL_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOL_PREFIX}gcc)
set(CMAKE_OBJCOPY ${TOOL_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOL_PREFIX}size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Common ARM embedded compiler flags
add_compile_options(
    -fno-exceptions
    -fno-rtti
    -ffunction-sections
    -fdata-sections
    -Wall
    -fstack-usage
    -fcyclomatic-complexity
    -static
    -MMD
    -MP
)

# Common ARM embedded linker flags
add_link_options(
    -Wl,--gc-sections
#    --specs=nosys.specs
    --specs=nano.specs
    -Wl,--start-group -lc -lm -Wl,--end-group
)

# ASM flags
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp -MMD -MP")

# Build type specific flags
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_C_FLAGS_RELEASE "-Os")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")

# ETL configuration for embedded targets
add_compile_definitions(
    ETL_NO_STL=1
    ETL_TARGET_DEVICE_GENERIC=1
    ETL_NO_CHECKS=1
)

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fpermissive>)

# CPU-specific flags (set by TARGET_PLATFORM in main CMakeLists.txt)
if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_compile_options(-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard)
    add_link_options(-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard)
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")
    set(CMAKE_SYSTEM_PROCESSOR cortex-m7)
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_compile_options(-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
    add_link_options(-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
    set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
endif()
