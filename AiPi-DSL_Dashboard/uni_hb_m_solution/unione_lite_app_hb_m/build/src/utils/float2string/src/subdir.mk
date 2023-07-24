################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/float2string/src/uni_float2string.c 

OBJS += \
./src/utils/float2string/src/uni_float2string.o 

C_DEPS += \
./src/utils/float2string/src/uni_float2string.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/float2string/src/%.o: ../src/utils/float2string/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


