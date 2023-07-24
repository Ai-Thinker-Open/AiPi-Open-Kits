################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/list/src/list_head.c 

OBJS += \
./src/utils/list/src/list_head.o 

C_DEPS += \
./src/utils/list/src/list_head.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/list/src/%.o: ../src/utils/list/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


