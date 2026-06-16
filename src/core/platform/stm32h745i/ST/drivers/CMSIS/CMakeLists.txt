cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-drivers-cmsis)

set(CMSIS_SOURCES
        include/stm32h745i/drivers/cmsis/cmsis_armcc.h
        include/stm32h745i/drivers/cmsis/cmsis_armclang.h
        include/stm32h745i/drivers/cmsis/cmsis_armclang_ltm.h
        include/stm32h745i/drivers/cmsis/cmsis_compiler.h
        include/stm32h745i/drivers/cmsis/cmsis_gcc.h
        include/stm32h745i/drivers/cmsis/cmsis_iccarm.h
        include/stm32h745i/drivers/cmsis/cmsis_version.h
        include/stm32h745i/drivers/cmsis/core_armv8mml.h
        include/stm32h745i/drivers/cmsis/core_armv8mbl.h
        include/stm32h745i/drivers/cmsis/core_armv81mml.h
        include/stm32h745i/drivers/cmsis/core_cm0.h
        include/stm32h745i/drivers/cmsis/core_cm0plus.h
        include/stm32h745i/drivers/cmsis/core_cm1.h
        include/stm32h745i/drivers/cmsis/core_cm3.h
        include/stm32h745i/drivers/cmsis/core_cm4.h
        include/stm32h745i/drivers/cmsis/core_cm7.h
        include/stm32h745i/drivers/cmsis/core_cm23.h
        include/stm32h745i/drivers/cmsis/core_cm33.h
        include/stm32h745i/drivers/cmsis/core_cm35p.h
        include/stm32h745i/drivers/cmsis/core_sc000.h
        include/stm32h745i/drivers/cmsis/core_sc300.h
        include/stm32h745i/drivers/cmsis/mpu_armv7.h
        include/stm32h745i/drivers/cmsis/mpu_armv8.h
        include/stm32h745i/drivers/cmsis/tz_context.h
        include/stm32h745i/drivers/cmsis/device/stm32h7xx.h
        include/stm32h745i/drivers/cmsis/device/stm32h745xx.h
        include/stm32h745i/drivers/cmsis/device/system_stm32h7xx.h
        include/stm32h745i/drivers/cmsis/RTOS2/cmsis_os2.h
        include/stm32h745i/drivers/cmsis/RTOS2/os_tick.h

        # These seem incomplete
#        src/RTOS2/os_systick.c
#        src/RTOS2/os_tick_gtim._c
#        src/RTOS2/os_tick_ptim.c
)

# Exclude this from external use because CORE_CM7 and CORE_CM4 are not defined yet
add_library(core-stm32h745i-drivers-cmsis INTERFACE EXCLUDE_FROM_ALL
        ${CMSIS_SOURCES}
)

target_include_directories(core-stm32h745i-drivers-cmsis INTERFACE
        include
        # Also expose device headers directly for libraries expecting standard ST layout
        include/stm32h745i/drivers/cmsis/device
)


if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
# CM7-specific variant
    add_library(core-stm32h745i-drivers-cmsis-cm7 INTERFACE)
    target_link_libraries(core-stm32h745i-drivers-cmsis-cm7 INTERFACE
            core-stm32h745i-drivers-cmsis
    )
    target_compile_definitions(core-stm32h745i-drivers-cmsis-cm7 INTERFACE
            CORE_CM7
            STM32H745xx
    )
endif()

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    # CM4-specific variant
    add_library(core-stm32h745i-drivers-cmsis-cm4 INTERFACE)
    target_link_libraries(core-stm32h745i-drivers-cmsis-cm4 INTERFACE
            core-stm32h745i-drivers-cmsis
    )
    target_compile_definitions(core-stm32h745i-drivers-cmsis-cm4 INTERFACE
            CORE_CM4
            STM32H745xx
    )
endif()

