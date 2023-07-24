################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/timer/src/uni_timer.c 

OBJS += \
./src/utils/timer/src/uni_timer.o 

C_DEPS += \
./src/utils/timer/src/uni_timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/timer/src/%.o: ../src/utils/timer/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


