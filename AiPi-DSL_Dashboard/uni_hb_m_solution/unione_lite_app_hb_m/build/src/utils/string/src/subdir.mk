################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/string/src/uni_string.c 

OBJS += \
./src/utils/string/src/uni_string.o 

C_DEPS += \
./src/utils/string/src/uni_string.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/string/src/%.o: ../src/utils/string/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


