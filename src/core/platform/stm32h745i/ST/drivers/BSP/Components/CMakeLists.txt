cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-drivers-bsp-components)

set(STM32_BSP_COMPONENTS_SOURCES
        include/stm32h745i/drivers/bsp/components/ft5336_conf_template.h
        include/stm32h745i/drivers/bsp/components/ft5336.h
        include/stm32h745i/drivers/bsp/components/ft5336_reg.h
        include/stm32h745i/drivers/bsp/components/gt911_conf_template.h
        include/stm32h745i/drivers/bsp/components/gt911.h
        include/stm32h745i/drivers/bsp/components/gt911_reg.h
        include/stm32h745i/drivers/bsp/components/io.h
        include/stm32h745i/drivers/bsp/components/lcd.h
        include/stm32h745i/drivers/bsp/components/mt25tl01g_conf_template.h
        include/stm32h745i/drivers/bsp/components/mt25tl01g.h
        include/stm32h745i/drivers/bsp/components/mt48lc4m32b2_conf_template.h
        include/stm32h745i/drivers/bsp/components/mt48lc4m32b2.h
        include/stm32h745i/drivers/bsp/components/rk043fn48h.h
        include/stm32h745i/drivers/bsp/components/ts.h

        src/ft5336/ft5336.c
        src/ft5336/ft5336_reg.c
        src/gt911/gt911.c
        src/gt911/gt911_reg.c
        src/mt25tl01g/mt25tl01g.c
        src/mt48lc4m32b2/mt48lc4m32b2.c
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-drivers-bsp-components-cm7 STATIC ${STM32_BSP_COMPONENTS_SOURCES})
    target_include_directories(core-stm32h745i-drivers-bsp-components-cm7 PUBLIC
            include
    )
    target_link_libraries(core-stm32h745i-drivers-bsp-components-cm7 PUBLIC
            core-stm32h745i-drivers-hal-cm7
    )
endif()

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-drivers-bsp-components-cm4 STATIC ${STM32_BSP_COMPONENTS_SOURCES})
    target_include_directories(core-stm32h745i-drivers-bsp-components-cm4 PUBLIC
            include
    )
    target_link_libraries(core-stm32h745i-drivers-bsp-components-cm4 PUBLIC
            core-stm32h745i-drivers-hal-cm4
    )
endif()