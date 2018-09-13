################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/libos/third/firmware/stm32f7/Src/subdir.mk
-include src/libos/third/firmware/sldrv/src/subdir.mk
-include src/libos/loader/si/subdir.mk
-include src/libos/libc/libm/math/subdir.mk
-include src/libos/libc/libm/machine/arm/subdir.mk
-include src/libos/libc/libm/complex/subdir.mk
-include src/libos/libc/libm/common/subdir.mk
-include src/libos/libc/libc/time/subdir.mk
-include src/libos/libc/libc/string/subdir.mk
-include src/libos/libc/libc/stdlib/subdir.mk
-include src/libos/libc/libc/stdio/subdir.mk
-include src/libos/libc/libc/search/subdir.mk
-include src/libos/libc/libc/posix/subdir.mk
-include src/libos/libc/libc/misc/md5/subdir.mk
-include src/libos/libc/libc/misc/ecc/subdir.mk
-include src/libos/libc/libc/misc/crc/subdir.mk
-include src/libos/libc/libc/misc/subdir.mk
-include src/libos/libc/libc/linux/subdir.mk
-include src/libos/libc/libc/ctype/subdir.mk
-include src/libos/djyos/time/subdir.mk
-include src/libos/djyos/pool/subdir.mk
-include src/libos/djyos/msgqueue/subdir.mk
-include src/libos/djyos/lock/subdir.mk
-include src/libos/djyos/int/subdir.mk
-include src/libos/djyos/heap/subdir.mk
-include src/libos/djyos/core/subdir.mk
-include src/libos/component/stddev/subdir.mk
-include src/libos/component/shell/subdir.mk
-include src/libos/component/multiplex/subdir.mk
-include src/libos/component/fs/iap/subdir.mk
-include src/libos/component/fs/subdir.mk
-include src/libos/component/exp/subdir.mk
-include src/libos/component/device/uart/subdir.mk
-include src/libos/component/device/flash/subdir.mk
-include src/libos/component/device/subdir.mk
-include src/libos/component/dbug/subdir.mk
-include src/libos/component/buffer/subdir.mk
-include src/libos/bsp/boarddrv/demo/NuvotonM487/startup/subdir.mk
-include src/libos/bsp/boarddrv/demo/NuvotonM487/drv/board/subdir.mk
-include src/libos/bsp/boarddrv/demo/APOLLO-STM32F7/startup/subdir.mk
-include src/libos/bsp/boarddrv/demo/APOLLO-STM32F7/drv/bypass_timer/subdir.mk
-include src/libos/bsp/boarddrv/demo/APOLLO-STM32F7/drv/boardnetcfg/subdir.mk
-include src/libos/bsp/boarddrv/demo/APOLLO-STM32F7/drv/board/subdir.mk
-include src/libos/bsp/arch/arm/armv7e-m/cortex-m7/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libos_App_Debug.a secondary-outputs

# Tool invocations
libos_App_Debug.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU Archiver'
	arm-none-eabi-ar -q  "libos_App_Debug.a" $(addsuffix *.o,$(subst \,/,$(dir $(subst $(dir $(shell for /r  %%i in (*makefile) do @echo %%i)),./,$(dir $(shell for /r  %%i in (*subdir.mk) do @echo %%i))))))
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(ARCHIVES)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) libos_App_Debug.a
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents

-include ../makefile.targets
