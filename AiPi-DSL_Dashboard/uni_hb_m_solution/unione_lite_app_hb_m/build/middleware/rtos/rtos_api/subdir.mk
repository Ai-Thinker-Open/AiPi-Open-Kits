################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middleware/rtos/rtos_api/rtos_api.c 

OBJS += \
./middleware/rtos/rtos_api/rtos_api.o 

C_DEPS += \
./middleware/rtos/rtos_api/rtos_api.d 


# Each subdirectory must supply rules for building sources it contributes
middleware/rtos/rtos_api/%.o: ../middleware/rtos/rtos_api/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


