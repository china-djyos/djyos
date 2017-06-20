################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/gdd_test/subdir.mk
-include src/critical/subdir.mk
-include src/OS_prjcfg/network/subdir.mk
-include src/OS_prjcfg/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
qh-1.bin \

SECONDARY_LIST += \
qh-1.lst \

SECONDARY_SIZE += \
qh-1.siz \


# All Target
all: qh-1.elf secondary-outputs

# Tool invocations
qh-1.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -Wall  -g -T "..\lds\debug.lds" -nostartfiles -nodefaultlibs -nostdlib -Xlinker --gc-sections -L"..\lib" -Wl,-Map,"qh-1.map" -Xlinker --cref -o "qh-1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

qh-1.bin: qh-1.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary "qh-1.elf"  "qh-1.bin"
	@echo 'Finished building: $@'
	@echo ' '

qh-1.lst: qh-1.elf
	@echo 'Invoking: Cross ARM GNU Create Listing'
	arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide "qh-1.elf" > "qh-1.lst"
	@echo 'Finished building: $@'
	@echo ' '

qh-1.siz: qh-1.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "qh-1.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) qh-1.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
