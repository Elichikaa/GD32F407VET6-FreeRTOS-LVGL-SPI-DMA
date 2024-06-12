CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_C_COMPILER "D:/0_tool_program/4_env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_ASM_COMPILER "D:/0_tool_program/4_env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_C_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -DGD32F450 -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_ASM_FLAGS " -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -DGD32F450 -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2")
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET(CMAKE_CXX_COMPILER "D:/0_tool_program/4_env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-g++.exe")
SET(CMAKE_CXX_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -DGD32F450 -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_CXX_COMPILER_WORKS TRUE)

SET(CMAKE_OBJCOPY "D:/0_tool_program/4_env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-objcopy.exe")
SET(CMAKE_SIZE "D:/0_tool_program/4_env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-size.exe")

SET(CMAKE_EXE_LINKER_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -DGD32F450 -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,Reset_Handler -T ${CMAKE_SOURCE_DIR}/board/linker_scripts/link.ld")

SET(CMAKE_CXX_STANDARD 14)
PROJECT(rtthread C CXX ASM)
INCLUDE_DIRECTORIES(
		app
		bsp
		board
		libraries/CMSIS
		libraries/CMSIS/GD/GD32F4xx/Include
		libraries/GD32F4xx_standard_peripheral/Include
		libraries/GD32F4xx_Firmware_Library/CMSIS/GD/GD32F4xx/Include
		module/soft_timer
		module/ringbuffer
		module/buzzer
)

ADD_DEFINITIONS(
	-DGD32F407
	-DUSE_STDPERIPH_DRIVER
)

SET(PROJECT_SOURCES
	app/main.c
	board/board.c
	libraries/CMSIS/GD/GD32F4xx/Source/system_gd32f4xx.c
	libraries/CMSIS/GD/GD32F4xx/Source/GCC/startup_gd32f4xx.s
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_timer.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_i2c.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_dma.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_syscfg.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_sdio.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_exti.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_gpio.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_rcu.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_misc.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_usart.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_spi.c
	libraries/GD32F4xx_standard_peripheral/Source/gd32f4xx_adc.c
	bsp/bsp_led.c
	bsp/bsp_uart.c
	bsp/bsp_beep.c
	module/soft_timer/soft_timer.c
	module/ringbuffer/ringbuffer.c
	module/buzzer/buzzer.c
)

LINK_DIRECTORIES(
)

LINK_LIBRARIES(
	c
	m
)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES})
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD 
COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin COMMAND ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf)

set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs -u _printf_float ")
