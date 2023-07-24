################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/interruptable_sleep/src/uni_interruptable.c 

OBJS += \
./src/utils/interruptable_sleep/src/uni_interruptable.o 

C_DEPS += \
./src/utils/interruptable_sleep/src/uni_interruptable.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/interruptable_sleep/src/%.o: ../src/utils/interruptable_sleep/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


