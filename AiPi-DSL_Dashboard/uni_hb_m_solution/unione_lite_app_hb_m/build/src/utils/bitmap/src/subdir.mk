################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/bitmap/src/uni_bitmap.c 

OBJS += \
./src/utils/bitmap/src/uni_bitmap.o 

C_DEPS += \
./src/utils/bitmap/src/uni_bitmap.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/bitmap/src/%.o: ../src/utils/bitmap/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


