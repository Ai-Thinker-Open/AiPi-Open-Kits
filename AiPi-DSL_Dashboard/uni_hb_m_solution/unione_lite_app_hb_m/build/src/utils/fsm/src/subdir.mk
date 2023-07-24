################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/fsm/src/uni_fsm.c 

OBJS += \
./src/utils/fsm/src/uni_fsm.o 

C_DEPS += \
./src/utils/fsm/src/uni_fsm.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/fsm/src/%.o: ../src/utils/fsm/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


