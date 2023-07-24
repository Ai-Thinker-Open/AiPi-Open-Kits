################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/event_route/src/uni_event_route.c 

OBJS += \
./src/utils/event_route/src/uni_event_route.o 

C_DEPS += \
./src/utils/event_route/src/uni_event_route.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/event_route/src/%.o: ../src/utils/event_route/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


