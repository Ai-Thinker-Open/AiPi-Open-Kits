################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sdk/player/src/pcm/src/uni_pcm_player.c \
../src/sdk/player/src/pcm/src/uni_pcm_resource.c 

OBJS += \
./src/sdk/player/src/pcm/src/uni_pcm_player.o \
./src/sdk/player/src/pcm/src/uni_pcm_resource.o 

C_DEPS += \
./src/sdk/player/src/pcm/src/uni_pcm_player.d \
./src/sdk/player/src/pcm/src/uni_pcm_resource.d 


# Each subdirectory must supply rules for building sources it contributes
src/sdk/player/src/pcm/src/%.o: ../src/sdk/player/src/pcm/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


