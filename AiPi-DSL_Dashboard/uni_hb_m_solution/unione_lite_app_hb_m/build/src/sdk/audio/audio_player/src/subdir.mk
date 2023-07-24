################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sdk/audio/audio_player/src/uni_audio_player.c 

OBJS += \
./src/sdk/audio/audio_player/src/uni_audio_player.o 

C_DEPS += \
./src/sdk/audio/audio_player/src/uni_audio_player.d 


# Each subdirectory must supply rules for building sources it contributes
src/sdk/audio/audio_player/src/%.o: ../src/sdk/audio/audio_player/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


