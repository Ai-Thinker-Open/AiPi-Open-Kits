################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/event_list/src/uni_event_list.c 

OBJS += \
./src/utils/event_list/src/uni_event_list.o 

C_DEPS += \
./src/utils/event_list/src/uni_event_list.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/event_list/src/%.o: ../src/utils/event_list/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


