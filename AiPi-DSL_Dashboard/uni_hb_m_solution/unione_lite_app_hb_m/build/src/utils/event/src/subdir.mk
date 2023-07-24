################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/event/src/uni_event.c 

OBJS += \
./src/utils/event/src/uni_event.o 

C_DEPS += \
./src/utils/event/src/uni_event.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/event/src/%.o: ../src/utils/event/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


