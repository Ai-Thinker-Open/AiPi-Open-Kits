################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/crc16/src/uni_crc16.c 

OBJS += \
./src/utils/crc16/src/uni_crc16.o 

C_DEPS += \
./src/utils/crc16/src/uni_crc16.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/crc16/src/%.o: ../src/utils/crc16/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


