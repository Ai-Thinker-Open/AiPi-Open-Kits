################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/auto_string/src/uni_auto_string.c 

OBJS += \
./src/utils/auto_string/src/uni_auto_string.o 

C_DEPS += \
./src/utils/auto_string/src/uni_auto_string.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/auto_string/src/%.o: ../src/utils/auto_string/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


