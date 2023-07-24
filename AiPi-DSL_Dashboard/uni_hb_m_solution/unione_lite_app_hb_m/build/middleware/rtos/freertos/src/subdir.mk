################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middleware/rtos/freertos/src/croutine.c \
../middleware/rtos/freertos/src/event_groups.c \
../middleware/rtos/freertos/src/heap_5s.c \
../middleware/rtos/freertos/src/list.c \
../middleware/rtos/freertos/src/port.c \
../middleware/rtos/freertos/src/portISR.c \
../middleware/rtos/freertos/src/queue.c \
../middleware/rtos/freertos/src/tasks.c \
../middleware/rtos/freertos/src/timers.c 

S_UPPER_SRCS += \
../middleware/rtos/freertos/src/os_cpu_a.S 

OBJS += \
./middleware/rtos/freertos/src/croutine.o \
./middleware/rtos/freertos/src/event_groups.o \
./middleware/rtos/freertos/src/heap_5s.o \
./middleware/rtos/freertos/src/list.o \
./middleware/rtos/freertos/src/os_cpu_a.o \
./middleware/rtos/freertos/src/port.o \
./middleware/rtos/freertos/src/portISR.o \
./middleware/rtos/freertos/src/queue.o \
./middleware/rtos/freertos/src/tasks.o \
./middleware/rtos/freertos/src/timers.o 

C_DEPS += \
./middleware/rtos/freertos/src/croutine.d \
./middleware/rtos/freertos/src/event_groups.d \
./middleware/rtos/freertos/src/heap_5s.d \
./middleware/rtos/freertos/src/list.d \
./middleware/rtos/freertos/src/port.d \
./middleware/rtos/freertos/src/portISR.d \
./middleware/rtos/freertos/src/queue.d \
./middleware/rtos/freertos/src/tasks.d \
./middleware/rtos/freertos/src/timers.d 

S_UPPER_DEPS += \
./middleware/rtos/freertos/src/os_cpu_a.d 


# Each subdirectory must supply rules for building sources it contributes
middleware/rtos/freertos/src/%.o: ../middleware/rtos/freertos/src/%.c
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"

middleware/rtos/freertos/src/%.o: ../middleware/rtos/freertos/src/%.S
	@echo 'Building file: $<'
	@$(CROSS_COMPILE)gcc $(CCFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"


