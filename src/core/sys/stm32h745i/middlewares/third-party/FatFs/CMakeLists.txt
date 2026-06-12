cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-fatfs)

set(STM32_FATFS_SOURCES
        include/sys/stm32h745i/fatfs/ff.h
        include/sys/stm32h745i/fatfs/ffconf.h
        include/sys/stm32h745i/fatfs/integer.h
        include/sys/stm32h745i/fatfs/ff_gen_drv.h
        include/sys/stm32h745i/fatfs/diskio.h

        src/diskio.c
        src/ff.c
        src/ff_gen_drv.c
        src/ffunicode.c
#        src/option/cc932.c
#        src/option/cc936.c
#        src/option/cc949.c
#        src/option/cc950.c
        src/option/ccsbcs.c
        src/option/syscall.c
        src/option/unicode.c
)

add_library(core-sys-stm32h745i-fatfs-cm7 STATIC ${STM32_FATFS_SOURCES})
target_include_directories(core-sys-stm32h745i-fatfs-cm7 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-sys-stm32h745i-fatfs-cm7 PUBLIC
        core-sys-stm32h745i-drivers-hal-cm7
)

add_library(core-sys-stm32h745i-fatfs-cm4 STATIC ${STM32_FATFS_SOURCES})
target_include_directories(core-sys-stm32h745i-fatfs-cm4 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-sys-stm32h745i-fatfs-cm4 PUBLIC
        core-sys-stm32h745i-drivers-hal-cm4
)