################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/iboot/critical/subdir.mk
-include src/iboot/subdir.mk
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
SECONDARY_FLASH += \
zq1201_Iboot_Debug.bin \

SECONDARY_LIST += \
zq1201_Iboot_Debug.lst \

SECONDARY_SIZE += \
zq1201_Iboot_Debug.siz \


# All Target
all: zq1201_Iboot_Debug.elf secondary-outputs

# Tool invocations
zq1201_Iboot_Debug.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T "../src/lds/iboot.lds" -nostartfiles -Xlinker --gc-sections -L"D:/DIDE/djysrc/examples/ZQ1201/lib" -Wl,-Map,"zq1201_Iboot_Debug.map" --specs=nano.specs -o "zq1201_Iboot_Debug.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

zq1201_Iboot_Debug.bin: zq1201_Iboot_Debug.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary "zq1201_Iboot_Debug.elf"  "zq1201_Iboot_Debug.bin"
	@echo 'Finished building: $@'
	@echo ' '

zq1201_Iboot_Debug.lst: zq1201_Iboot_Debug.elf
	@echo 'Invoking: Cross ARM GNU Create Listing'
	arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide "zq1201_Iboot_Debug.elf" > "zq1201_Iboot_Debug.lst"
	@echo 'Finished building: $@'
	@echo ' '

zq1201_Iboot_Debug.siz: zq1201_Iboot_Debug.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "zq1201_Iboot_Debug.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) zq1201_Iboot_Debug.elf
	-@echo ' '

post-build:
	-make zq1201_Iboot_Debug.bin && elf_to_bin.exe zq1201_Iboot_Debug.elf zq1201_Iboot_Debug.bin && ren zq1201_Iboot_Debug.bin newzq1201_Iboot_Debug.bin
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents post-build

-include ../makefile.targets
