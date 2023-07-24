################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/init-default.c \
../startup/interrupt.c \
../startup/retarget.c 

S_UPPER_SRCS += \
../startup/crt0.S 

OBJS += \
./startup/crt0.o \
./startup/init-default.o \
./startup/interrupt.o \
./startup/retarget.o 

C_DEPS += \
./startup/init-default.d \
./startup/interrupt.d \
./startup/retarget.d 

S_UPPER_DEPS += \
./startup/crt0.d 

ifeq ($(UART_UPDATE), 1)
C_SRCS += ../startup/flash_boot.c
OBJS += ./startup/flash_boot.o
C_DEPS += ./startup/flash_boot.d
endif

# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"

startup/%.o: ../startup/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


