################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/src/examples/hb_smart_ac.c \
../user/src/examples/hb_uart.c \
../user/src/examples/hb_uart_ucp.c \
../user/src/examples/ac_device_simulator.c \
../user/src/examples/hb_housekeeper.c \
../user/src/examples/hb_gpio_adc.c \
../user/src/examples/hb_gpio_key.c \
../user/src/examples/hb_pwm_led.c \
../user/src/examples/hb_timer_buzzer.c \
../user/src/examples/hb_sw_timers.c \
../user/src/examples/hb_flash_example.c \
. /user/src/examples/hb_asr_control.c \
../user/src/examples/hb_player.c \
../user/src/examples/housekeeper_irc.c \
../user/src/examples/hb_uart_sucp.c \
../user/src/examples/hb_auto_gpio.c \
../user/src/examples/hb_power_sleep.c

OBJS += \
./user/src/examples/hb_smart_ac.o \
./user/src/examples/hb_uart.o \
./user/src/examples/hb_uart_ucp.o \
./user/src/examples/ac_device_simulator.o \
./user/src/examples/hb_housekeeper.o \
./user/src/examples/hb_gpio_adc.o \
./user/src/examples/hb_gpio_key.o \
./user/src/examples/hb_pwm_led.o \
./user/src/examples/hb_timer_buzzer.o \
./user/src/examples/hb_sw_timers.o \
./user/src/examples/hb_flash_example.o \
./user/src/examples/hb_asr_control.o \
./user/src/examples/hb_player.o \
./user/src/examples/housekeeper_irc.o \
./user/src/examples/hb_uart_sucp.o \
./user/src/examples/hb_auto_gpio.o \
./user/src/examples/hb_power_sleep.o

C_DEPS += \
./user/src/examples/hb_smart_ac.d \
./user/src/examples/hb_uart.d \
./user/src/examples/hb_uart_ucp.d \
./user/src/examples/ac_device_simulator.d \
./user/src/examples/hb_housekeeper.d \
./user/src/examples/hb_gpio_adc.d \
./user/src/examples/hb_gpio_key.d \
./user/src/examples/hb_pwm_led.d \
./user/src/examples/hb_timer_buzzer.d \
./user/src/examples/hb_sw_timers.d \
./user/src/examples/hb_flash_example.d \
./user/src/examples/hb_asr_control.d \
./user/src/examples/hb_player.d \
./user/src/examples/housekeeper_irc.d \
./user/src/examples/hb_uart_sucp.d \
./user/src/examples/hb_auto_gpio.d \
./user/src/examples/hb_power_sleep.d

# Each subdirectory must supply rules for building sources it contributes
user/src/examples/%.o: ../user/src/examples/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"

-include user/src/examples/drivers/subdir.mk
