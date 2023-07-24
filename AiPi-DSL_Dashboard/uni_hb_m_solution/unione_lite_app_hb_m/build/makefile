################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CODE_ROOT=..

ifndef CROSS_COMPILE
CROSS_COMPILE=/opt/Andestech/BSPv422/toolchains/nds32le-elf-mculib-v3s/bin/nds32le-elf-
endif

ifndef SECONDARY_OUTPUT_PATH
SECONDARY_OUTPUT_PATH=$(CODE_ROOT)/output
endif

$(shell mkdir -p $(SECONDARY_OUTPUT_PATH))

-include ../makefile.init

RM := rm -rf

ifeq ($(OTA_UPDATE), TRUE)
UART_UPDATE=1
else
UART_UPDATE=0
endif

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include middleware/rtos/freertos/src/subdir.mk
-include middleware/rtos/rtos_api/subdir.mk
-include src/app/src/subdir.mk
-include src/app/src/sessions/subdir.mk
-include src/sdk/audio/audio_player/src/subdir.mk
-include src/sdk/idle_detect/src/subdir.mk
-include src/sdk/player/src/pcm/src/subdir.mk
-include src/sdk/player/src/subdir.mk
-include src/sdk/uart/src/subdir.mk
-include src/sdk/vui/src/subdir.mk
-include src/utils/arpt/src/subdir.mk
-include src/utils/auto_string/src/subdir.mk
-include src/utils/bitmap/src/subdir.mk
-include src/utils/black_board/src/subdir.mk
-include src/utils/cJSON/src/subdir.mk
-include src/utils/config/src/subdir.mk
-include src/utils/crc16/src/subdir.mk
-include src/utils/data_buf/src/subdir.mk
-include src/utils/event/src/subdir.mk
-include src/utils/event_list/src/subdir.mk
-include src/utils/event_route/src/subdir.mk
-include src/utils/float2string/src/subdir.mk
-include src/utils/fsm/src/subdir.mk
-include src/utils/hash/src/subdir.mk
-include src/utils/interruptable_sleep/src/subdir.mk
-include src/utils/list/src/subdir.mk
-include src/utils/log/src/subdir.mk
-include src/utils/string/src/subdir.mk
-include src/utils/timer/src/subdir.mk
-include src/utils/uart/src/subdir.mk
-include startup/subdir.mk
-include user/src/subdir.mk
-include user/src/examples/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
BUILD_ARTIFACT_FILE_BASE_NAME = uni_app

LINKER_OUTPUTS = uni_app.adx

GCOV_OUT += \
$(foreach subdir,$(SUBDIRS),$(wildcard $(subdir)/*.gcda) $(wildcard $(subdir)/*.gcno)) \

GPROF_OUT += \
../gmon.sum ../gmon.out \

SYMBOL_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/symbol.txt \

READELF_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/readelf.txt \

OBJCOPY_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin \

SIZE_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/.PHONY.size \


# All Target
all: uni_app.adx secondary-outputs

# Tool invocations
uni_app.adx: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Andes C Linker'
	$(CROSS_COMPILE)gcc -O1 -nostartfiles -static  -L"$(CODE_ROOT)/lib" -mcmodel=medium -ldsp -mext-dsp -T"../nds32-ae210p.ld" -Wl,--gc-sections -mvh -o "uni_app.adx" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(foreach subdir,$(SUBDIRS),$(wildcard $(subdir)/*.gcda) $(wildcard $(subdir)/*.gcno)): uni_app.adx $(OBJS) $(USER_OBJS)
../gmon.sum ../gmon.out: uni_app.adx $(OBJS) $(USER_OBJS)

$(SECONDARY_OUTPUT_PATH)/symbol.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: NM (symbol listing)'
	$(CROSS_COMPILE)nm -n -l -C "uni_app.adx" > $(SECONDARY_OUTPUT_PATH)/symbol.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/readelf.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: Readelf (ELF info listing)'
	$(CROSS_COMPILE)readelf -a "uni_app.adx" > $(SECONDARY_OUTPUT_PATH)/readelf.txt
	@echo 'Finished building: $@'
	@echo ' '

: $(LINKER_OUTPUTS)
	@echo 'Invoking: Objdump (disassembly)'
	$(CROSS_COMPILE)objdump -x -d -C "uni_app.adx" >
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin: $(LINKER_OUTPUTS)
	@echo 'Invoking: Objcopy (object content copy)'
	$(CROSS_COMPILE)objcopy -S -O binary "uni_app.adx" $(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/.PHONY.size: $(LINKER_OUTPUTS)
	@echo 'Invoking: Size (section size listing)'
	$(CROSS_COMPILE)size  "uni_app.adx" | tee $(SECONDARY_OUTPUT_PATH)/.PHONY.size
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-@$(RM) $(OBJCOPY_OUTPUTS)$(OBJS)$(C_DEPS)$(GCOV_OUT)$(SYMBOL_OUTPUTS)$(READELF_OUTPUTS)$(GPROF_OUT)$(SIZE_OUTPUTS)$(EXECUTABLES)$(S_UPPER_DEPS) uni_app.adx
	-@echo 'clean finish'

secondary-outputs: $(SYMBOL_OUTPUTS) $(READELF_OUTPUTS) $(OBJCOPY_OUTPUTS) $(SIZE_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:
.NOTPARALLEL: pre-build

-include ../makefile.targets
