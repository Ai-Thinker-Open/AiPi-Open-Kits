################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app/src/main.c \
../src/app/src/uni_record_save.c \
../src/app/src/uni_session.c \
../src/app/src/uni_session_manage.c \
../src/app/src/uni_user_meeting.c

OBJS += \
./src/app/src/main.o \
./src/app/src/uni_record_save.o \
./src/app/src/uni_session.o \
./src/app/src/uni_session_manage.o \
./src/app/src/uni_user_meeting.o

C_DEPS += \
./src/app/src/main.d \
./src/app/src/uni_record_save.d \
./src/app/src/uni_session.d \
./src/app/src/uni_session_manage.d \
./src/app/src/uni_user_meeting.d


# Each subdirectory must supply rules for building sources it contributes
src/app/src/%.o: ../src/app/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


