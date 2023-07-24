################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/src/examples/drivers/user_ad_gl5528.c \
../user/src/examples/drivers/user_digital_keys.c \
../user/src/examples/drivers/user_i2c_ds3231.c \
../user/src/examples/drivers/user_timer_buzzer.c \
../user/src/examples/drivers/user_pwm_led.c \
../user/src/examples/drivers/user_spi_ssd1306.c \
../user/src/examples/drivers/user_sw_timer.c

OBJS += \
./user/src/examples/drivers/user_ad_gl5528.o \
./user/src/examples/drivers/user_digital_keys.o \
./user/src/examples/drivers/user_i2c_ds3231.o \
./user/src/examples/drivers/user_timer_buzzer.o \
./user/src/examples/drivers/user_pwm_led.o \
./user/src/examples/drivers/user_spi_ssd1306.o \
./user/src/examples/drivers/user_sw_timer.o

C_DEPS += \
./user/src/examples/drivers/user_ad_gl5528.d \
./user/src/examples/drivers/user_digital_keys.d \
./user/src/examples/drivers/user_i2c_ds3231.d \
./user/src/examples/drivers/user_timer_buzzer.d \
./user/src/examples/drivers/user_pwm_led.d \
./user/src/examples/drivers/user_spi_ssd1306.d \
./user/src/examples/drivers/user_sw_timer.d

# Each subdirectory must supply rules for building sources it contributes
user/src/examples/drivers/%.o: ../suser/src/examples/drivers/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


