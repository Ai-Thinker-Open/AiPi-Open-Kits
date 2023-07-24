################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/cJSON/src/cJSON.c \
../src/utils/cJSON/src/uni_json.c 

OBJS += \
./src/utils/cJSON/src/cJSON.o \
./src/utils/cJSON/src/uni_json.o 

C_DEPS += \
./src/utils/cJSON/src/cJSON.d \
./src/utils/cJSON/src/uni_json.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/cJSON/src/%.o: ../src/utils/cJSON/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


