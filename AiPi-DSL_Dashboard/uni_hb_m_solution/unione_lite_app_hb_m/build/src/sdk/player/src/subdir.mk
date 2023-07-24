################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sdk/player/src/uni_media_player.c \
../src/sdk/player/src/uni_pcm_default.c 

OBJS += \
./src/sdk/player/src/uni_media_player.o \
./src/sdk/player/src/uni_pcm_default.o 

C_DEPS += \
./src/sdk/player/src/uni_media_player.d \
./src/sdk/player/src/uni_pcm_default.d 


# Each subdirectory must supply rules for building sources it contributes
src/sdk/player/src/%.o: ../src/sdk/player/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


