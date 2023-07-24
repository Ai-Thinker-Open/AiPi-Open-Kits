################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/src/user_asr.c \
../user/src/user_event.c \
../user/src/user_file.c \
../user/src/user_flash.c \
../user/src/user_gpio.c \
../user/src/user_main.c \
../user/src/user_player.c \
../user/src/user_power.c \
../user/src/user_record.c \
../user/src/user_uart.c \
../user/src/user_uni_ucp.c \
../user/src/user_timer.c \
../user/src/user_pwm.c \
../user/src/user_uni_sucp.c \
../user/src/user_start.c

OBJS += \
./user/src/user_asr.o \
./user/src/user_event.o \
./user/src/user_file.o \
./user/src/user_flash.o \
./user/src/user_gpio.o \
./user/src/user_main.o \
./user/src/user_player.o \
./user/src/user_power.o \
./user/src/user_record.o \
./user/src/user_uart.o \
./user/src/user_uni_ucp.o \
./user/src/user_timer.o \
./user/src/user_pwm.o \
./user/src/user_uni_sucp.o \
./user/src/user_start.o

C_DEPS += \
./user/src/user_asr.d \
./user/src/user_event.d \
./user/src/user_file.d \
./user/src/user_flash.d \
./user/src/user_gpio.d \
./user/src/user_main.d \
./user/src/user_player.d \
./user/src/user_power.d \
./user/src/user_record.d \
./user/src/user_uart.d \
./user/src/user_uni_ucp.d \
./user/src/user_timer.d \
./user/src/user_pwm.d \
./user/src/user_uni_sucp.d \
./user/src/user_start.d

# Each subdirectory must supply rules for building sources it contributes
user/src/%.o: ../user/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


