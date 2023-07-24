################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/uart/src/uni_communication.c \
../src/utils/uart/src/uni_uart.c 

OBJS += \
./src/utils/uart/src/uni_communication.o \
./src/utils/uart/src/uni_uart.o 

C_DEPS += \
./src/utils/uart/src/uni_communication.d \
./src/utils/uart/src/uni_uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/uart/src/%.o: ../src/utils/uart/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


