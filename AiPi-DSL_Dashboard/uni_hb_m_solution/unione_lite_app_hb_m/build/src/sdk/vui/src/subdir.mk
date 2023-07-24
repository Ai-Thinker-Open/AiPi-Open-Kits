################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sdk/vui/src/uni_lasr_result_parser.c \
../src/sdk/vui/src/uni_nlu.c \
../src/sdk/vui/src/uni_recog_engine_control.c \
../src/sdk/vui/src/uni_recog_preproc.c \
../src/sdk/vui/src/uni_recog_service.c \
../src/sdk/vui/src/uni_vui_interface.c 

OBJS += \
./src/sdk/vui/src/uni_lasr_result_parser.o \
./src/sdk/vui/src/uni_nlu.o \
./src/sdk/vui/src/uni_recog_engine_control.o \
./src/sdk/vui/src/uni_recog_preproc.o \
./src/sdk/vui/src/uni_recog_service.o \
./src/sdk/vui/src/uni_vui_interface.o 

C_DEPS += \
./src/sdk/vui/src/uni_lasr_result_parser.d \
./src/sdk/vui/src/uni_nlu.d \
./src/sdk/vui/src/uni_recog_engine_control.d \
./src/sdk/vui/src/uni_recog_preproc.d \
./src/sdk/vui/src/uni_recog_service.d \
./src/sdk/vui/src/uni_vui_interface.d 


# Each subdirectory must supply rules for building sources it contributes
src/sdk/vui/src/%.o: ../src/sdk/vui/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


