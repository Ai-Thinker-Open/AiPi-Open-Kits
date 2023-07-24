################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/hash/src/bkdr_hash.c 

OBJS += \
./src/utils/hash/src/bkdr_hash.o 

C_DEPS += \
./src/utils/hash/src/bkdr_hash.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/hash/src/%.o: ../src/utils/hash/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


