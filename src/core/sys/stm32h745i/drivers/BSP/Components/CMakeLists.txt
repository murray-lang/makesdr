cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-drivers-bsp-components)

set(STM32_BSP_COMPONENTS_SOURCES
        include/sys/stm32h745i/drivers/bsp/components/ft5336_conf_template.h
        include/sys/stm32h745i/drivers/bsp/components/ft5336.h
        include/sys/stm32h745i/drivers/bsp/components/ft5336_reg.h
        include/sys/stm32h745i/drivers/bsp/components/gt911_conf_template.h
        include/sys/stm32h745i/drivers/bsp/components/gt911.h
        include/sys/stm32h745i/drivers/bsp/components/gt911_reg.h
        include/sys/stm32h745i/drivers/bsp/components/io.h
        include/sys/stm32h745i/drivers/bsp/components/lcd.h
        include/sys/stm32h745i/drivers/bsp/components/mt25tl01g_conf_template.h
        include/sys/stm32h745i/drivers/bsp/components/mt25tl01g.h
        include/sys/stm32h745i/drivers/bsp/components/mt48lc4m32b2_conf_template.h
        include/sys/stm32h745i/drivers/bsp/components/mt48lc4m32b2.h
        include/sys/stm32h745i/drivers/bsp/components/rk043fn48h.h
        include/sys/stm32h745i/drivers/bsp/components/ts.h

        src/ft5336/ft5336.c
        src/ft5336/ft5336_reg.c
        src/gt911/gt911.c
        src/gt911/gt911_reg.c
        src/mt25tl01g/mt25tl01g.c
        src/mt48lc4m32b2/mt48lc4m32b2.c
)

add_library(core-sys-stm32h745i-drivers-bsp-components-cm7 STATIC ${STM32_BSP_COMPONENTS_SOURCES})
target_include_directories(core-sys-stm32h745i-drivers-bsp-components-cm7 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-sys-stm32h745i-drivers-bsp-components-cm7 PUBLIC
        core-sys-stm32h745i-drivers-hal-cm7
)

add_library(core-sys-stm32h745i-drivers-bsp-components-cm4 STATIC ${STM32_BSP_COMPONENTS_SOURCES})
target_include_directories(core-sys-stm32h745i-drivers-bsp-components-cm4 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-sys-stm32h745i-drivers-bsp-components-cm4 PUBLIC
        core-sys-stm32h745i-drivers-hal-cm4
)