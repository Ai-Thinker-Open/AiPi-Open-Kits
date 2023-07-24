################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/data_buf/src/uni_databuf.c 

OBJS += \
./src/utils/data_buf/src/uni_databuf.o 

C_DEPS += \
./src/utils/data_buf/src/uni_databuf.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/data_buf/src/%.o: ../src/utils/data_buf/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


