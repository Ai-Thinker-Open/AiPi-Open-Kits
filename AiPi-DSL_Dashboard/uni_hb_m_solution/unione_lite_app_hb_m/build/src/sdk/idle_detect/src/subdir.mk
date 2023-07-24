################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sdk/idle_detect/src/idle_detect_service.c 

OBJS += \
./src/sdk/idle_detect/src/idle_detect_service.o 

C_DEPS += \
./src/sdk/idle_detect/src/idle_detect_service.d 


# Each subdirectory must supply rules for building sources it contributes
src/sdk/idle_detect/src/%.o: ../src/sdk/idle_detect/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


