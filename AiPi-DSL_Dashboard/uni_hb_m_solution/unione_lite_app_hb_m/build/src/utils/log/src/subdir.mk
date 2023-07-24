################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/log/src/uni_log.c 

OBJS += \
./src/utils/log/src/uni_log.o 

C_DEPS += \
./src/utils/log/src/uni_log.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/log/src/%.o: ../src/utils/log/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


