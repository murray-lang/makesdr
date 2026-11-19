add_library(core-stm32h745i-drivers-bsp-pdm-cm4 STATIC IMPORTED GLOBAL)
set_target_properties(core-stm32h745i-drivers-bsp-pdm-cm4 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/libPDMFilter_CM4_GCC_wc32.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include"
)

add_library(core-stm32h745i-drivers-bsp-pdm-cm7 STATIC IMPORTED GLOBAL)
set_target_properties(core-stm32h745i-drivers-bsp-pdm-cm7 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/libPDMFilter_CM7_GCC_wc32.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include"
)
