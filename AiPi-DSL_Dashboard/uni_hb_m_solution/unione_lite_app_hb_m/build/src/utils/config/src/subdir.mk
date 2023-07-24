################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/config/src/uni_config.c 

OBJS += \
./src/utils/config/src/uni_config.o 

C_DEPS += \
./src/utils/config/src/uni_config.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/config/src/%.o: ../src/utils/config/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


