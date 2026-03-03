function(add_target_properties TARGET_NAME)

target_compile_definitions(
    ${TARGET_NAME} PRIVATE
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM>>:DEBUG>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:DEBUG>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:USE_HAL_DRIVER>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:STM32G474xx>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:USE_HAL_DRIVER>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:STM32G474xx>"

    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:DEBUG>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:USE_HAL_DRIVER>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:STM32G474xx>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:USE_HAL_DRIVER>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:STM32G474xx>"
)

target_include_directories(
    ${TARGET_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/include/Core/Inc"
    "${PROJECT_SOURCE_DIR}/include/Drivers/STM32G4xx_HAL_Driver/Inc"
    "${PROJECT_SOURCE_DIR}/include/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy"
    "${PROJECT_SOURCE_DIR}/include/Drivers/CMSIS/Device/ST/STM32G4xx/Include"
    "${PROJECT_SOURCE_DIR}/include/Drivers/CMSIS/Include"
)

target_compile_options(
    ${TARGET_NAME} PRIVATE
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM>>:-g3>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g3>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g3>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:ASM>>:-g0>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:-g0>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:-g0>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:-Os>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:-Os>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:>"
    "$<$<CONFIG:Debug>:-mcpu=cortex-m4>"
    "$<$<CONFIG:Debug>:-mfpu=fpv4-sp-d16>"
    "$<$<CONFIG:Debug>:-mfloat-abi=hard>"
    "$<$<NOT:$<CONFIG:Debug>>:-mcpu=cortex-m4>"
    "$<$<NOT:$<CONFIG:Debug>>:-mfpu=fpv4-sp-d16>"
    "$<$<NOT:$<CONFIG:Debug>>:-mfloat-abi=hard>"
)

target_link_libraries(
    ${TARGET_NAME} PRIVATE
)

target_link_directories(
    ${TARGET_NAME} PRIVATE
)

target_link_options(
    ${TARGET_NAME} PRIVATE
    "$<$<CONFIG:Debug>:-mcpu=cortex-m4>"
    "$<$<CONFIG:Debug>:-mfpu=fpv4-sp-d16>"
    "$<$<CONFIG:Debug>:-mfloat-abi=hard>"
    "$<$<NOT:$<CONFIG:Debug>>:-mcpu=cortex-m4>"
    "$<$<NOT:$<CONFIG:Debug>>:-mfpu=fpv4-sp-d16>"
    "$<$<NOT:$<CONFIG:Debug>>:-mfloat-abi=hard>"
    -T
    "$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/include/STM32G474VETX_FLASH.ld>"
    "$<$<NOT:$<CONFIG:Debug>>:${PROJECT_SOURCE_DIR}/include/STM32G474VETX_FLASH.ld>"
)

target_sources(
    ${TARGET_NAME} PRIVATE
    "src/Core/Main.cpp"
    "src/Core/motor.cpp"
    "src/Core/Device.cpp"
    "src/Interface/HCMSDisplay.cpp"
    "src/Interface/UartPrint.cpp"
    "include/startup_stm32g474xx.s"
    "include/Core/Src/gpio.c"
    "include/Core/Src/spi.c"
    "include/Core/Src/stm32g4xx_hal_msp.c"
    "include/Core/Src/stm32g4xx_it.c"
    "include/Core/Src/system_stm32g4xx.c"
    "include/Core/Src/usart.c"
    "include/Core/Src/tim.c"
    "include/Core/Src/i2c.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c"
    "include/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c"
)

set_target_properties(${TARGET_NAME} PROPERTIES
    LINK_FLAGS "-Wl,-Map,${TARGET_NAME}.map"
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}>
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex
)

endfunction()