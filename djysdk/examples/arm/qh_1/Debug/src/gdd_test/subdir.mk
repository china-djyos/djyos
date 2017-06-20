################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/djysdk/djysrc/test/gdd_test/gdd_demo_draw_text.c \
D:/djysdk/djysrc/test/gdd_test/gdd_demo_hello_world.c \
D:/djysdk/djysrc/test/gdd_test/gdd_demo_listbox.c \
D:/djysdk/djysrc/test/gdd_test/gdd_demo_move_window.c \
D:/djysdk/djysrc/test/gdd_test/gdd_demo_progressbar.c \
D:/djysdk/djysrc/test/gdd_test/gdd_demo_qurcode.c \
D:/djysdk/djysrc/test/gdd_test/gdd_demo_timer.c \
D:/djysdk/djysrc/test/gdd_test/gdd_demo_zigbee.c \
D:/djysdk/djysrc/test/gdd_test/gdd_test.c 

OBJS += \
./src/gdd_test/gdd_demo_draw_text.o \
./src/gdd_test/gdd_demo_hello_world.o \
./src/gdd_test/gdd_demo_listbox.o \
./src/gdd_test/gdd_demo_move_window.o \
./src/gdd_test/gdd_demo_progressbar.o \
./src/gdd_test/gdd_demo_qurcode.o \
./src/gdd_test/gdd_demo_timer.o \
./src/gdd_test/gdd_demo_zigbee.o \
./src/gdd_test/gdd_test.o 

C_DEPS += \
./src/gdd_test/gdd_demo_draw_text.d \
./src/gdd_test/gdd_demo_hello_world.d \
./src/gdd_test/gdd_demo_listbox.d \
./src/gdd_test/gdd_demo_move_window.d \
./src/gdd_test/gdd_demo_progressbar.d \
./src/gdd_test/gdd_demo_qurcode.d \
./src/gdd_test/gdd_demo_timer.d \
./src/gdd_test/gdd_demo_zigbee.d \
./src/gdd_test/gdd_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/gdd_test/gdd_demo_draw_text.o: D:/djysdk/djysrc/test/gdd_test/gdd_demo_draw_text.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gdd_test/gdd_demo_hello_world.o: D:/djysdk/djysrc/test/gdd_test/gdd_demo_hello_world.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gdd_test/gdd_demo_listbox.o: D:/djysdk/djysrc/test/gdd_test/gdd_demo_listbox.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gdd_test/gdd_demo_move_window.o: D:/djysdk/djysrc/test/gdd_test/gdd_demo_move_window.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gdd_test/gdd_demo_progressbar.o: D:/djysdk/djysrc/test/gdd_test/gdd_demo_progressbar.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gdd_test/gdd_demo_qurcode.o: D:/djysdk/djysrc/test/gdd_test/gdd_demo_qurcode.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gdd_test/gdd_demo_timer.o: D:/djysdk/djysrc/test/gdd_test/gdd_demo_timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gdd_test/gdd_demo_zigbee.o: D:/djysdk/djysrc/test/gdd_test/gdd_demo_zigbee.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gdd_test/gdd_test.o: D:/djysdk/djysrc/test/gdd_test/gdd_test.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -nostdinc -D__ARM_ARCH_7M__=1 -DUSE_HAL_DRIVER -DSTM32F103xG -DDEBUG=1 -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libm\common" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\libc\libc\include\sys" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\chipdrv\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\djyos\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include" -I"D:/djysdk/examples/arm/qh_1/src/OS_prjcfg/include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103\qh_1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\arch\arm\cortex-m\armv7-m\cortex-m3\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\CMSIS\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\cpudrv\stm32f1xx\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\boarddrv\qh_1\include" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\bsp\startup\stm32f103" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\cfg" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\gui\gdd\widget" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\tcpip" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\qrencode" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\component\include\net" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1" -I"D:/djysdk/examples/arm/qh_1\..\..\..\djysrc\third\firmware\stm32f1\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


