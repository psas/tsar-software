################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32G0xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Actuator-Controller.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Actuator-Controller.list \


# All Target
all: Actuator-Controller.elf secondary-outputs

# Tool invocations
Actuator-Controller.elf: $(OBJS) $(USER_OBJS) C:\Users\Susan\Documents\TSAR\capstone\actuator-controller\firmware\STM32G071R8TX_FLASH.ld
	arm-none-eabi-gcc -o "Actuator-Controller.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\Users\Susan\Documents\TSAR\capstone\actuator-controller\firmware\STM32G071R8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Actuator-Controller.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Actuator-Controller.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Actuator-Controller.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
