################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/black_board/src/uni_black_board.c 

OBJS += \
./src/utils/black_board/src/uni_black_board.o 

C_DEPS += \
./src/utils/black_board/src/uni_black_board.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/black_board/src/%.o: ../src/utils/black_board/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


