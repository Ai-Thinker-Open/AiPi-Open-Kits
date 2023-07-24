################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app/src/sessions/uni_setting_session.c \
../src/app/src/sessions/uni_wakeup_session.c \
../src/app/src/sessions/uni_watchdog_session.c 

OBJS += \
./src/app/src/sessions/uni_setting_session.o \
./src/app/src/sessions/uni_wakeup_session.o \
./src/app/src/sessions/uni_watchdog_session.o 

C_DEPS += \
./src/app/src/sessions/uni_setting_session.d \
./src/app/src/sessions/uni_wakeup_session.d \
./src/app/src/sessions/uni_watchdog_session.d 


# Each subdirectory must supply rules for building sources it contributes
src/app/src/sessions/%.o: ../src/app/src/sessions/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


