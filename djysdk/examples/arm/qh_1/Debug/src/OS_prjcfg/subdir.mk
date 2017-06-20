################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/OS_prjcfg/IO_config.c \
../src/OS_prjcfg/core_config.c \
../src/OS_prjcfg/gui_config.c \
../src/OS_prjcfg/local_config.c \
../src/OS_prjcfg/misc_config.c 

OBJS += \
./src/OS_prjcfg/IO_config.o \
./src/OS_prjcfg/core_config.o \
./src/OS_prjcfg/gui_config.o \
./src/OS_prjcfg/local_config.o \
./src/OS_prjcfg/misc_config.o 

C_DEPS += \
./src/OS_prjcfg/IO_config.d \
./src/OS_prjcfg/core_config.d \
./src/OS_prjcfg/gui_config.d \
./src/OS_prjcfg/local_config.d \
./src/OS_prjcfg/misc_config.d 


# Each subdirectory must supply rules for building sources it contributes
src/OS_prjcfg/%.o: ../src/OS_prjcfg/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


