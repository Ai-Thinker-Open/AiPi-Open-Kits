################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/arpt/src/uni_arpt.c 

OBJS += \
./src/utils/arpt/src/uni_arpt.o 

C_DEPS += \
./src/utils/arpt/src/uni_arpt.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/arpt/src/%.o: ../src/utils/arpt/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


