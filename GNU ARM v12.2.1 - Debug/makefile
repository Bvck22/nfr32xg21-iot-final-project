################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include autogen/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := nfr32xg21-iot-final-project
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: nfr32xg21-iot-final-project.axf

# Tool invocations
nfr32xg21-iot-final-project.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "C:\Users\ADMIN\SimplicityStudio\v5_workspace\nfr32xg21-iot-final-project\autogen\linkerfile.ld" -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="nfr32xg21-iot-final-project.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o nfr32xg21-iot-final-project.axf -Wl,--start-group "./BSP/bsp_bluetooth.o" "./BSP/bsp_usart.o" "./app.o" "./main.o" "./autogen/sl_board_default_init.o" "./autogen/sl_event_handler.o" "./autogen/sl_iostream_handles.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: nfr32xg21-iot-final-project.out'
	arm-none-eabi-objcopy "nfr32xg21-iot-final-project.axf" "nfr32xg21-iot-final-project.out"
	@echo ' '

	@echo 'Building hex file: nfr32xg21-iot-final-project.hex'
	arm-none-eabi-objcopy -O ihex "nfr32xg21-iot-final-project.axf" "nfr32xg21-iot-final-project.hex"
	@echo ' '

	@echo 'Building bin file: nfr32xg21-iot-final-project.bin'
	arm-none-eabi-objcopy -O binary "nfr32xg21-iot-final-project.axf" "nfr32xg21-iot-final-project.bin"
	@echo ' '

	@echo 'Building s37 file: nfr32xg21-iot-final-project.s37'
	arm-none-eabi-objcopy -O srec "nfr32xg21-iot-final-project.axf" "nfr32xg21-iot-final-project.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "nfr32xg21-iot-final-project.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) nfr32xg21-iot-final-project.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
