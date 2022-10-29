################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../Display.c \
../Display_main.c \
../Internal_Light.c \
../Timers.c \
../dc-motor.c \
../gpio.c \
../uart.c 

OBJS += \
./Buzzer.o \
./Display.o \
./Display_main.o \
./Internal_Light.o \
./Timers.o \
./dc-motor.o \
./gpio.o \
./uart.o 

C_DEPS += \
./Buzzer.d \
./Display.d \
./Display_main.d \
./Internal_Light.d \
./Timers.d \
./dc-motor.d \
./gpio.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


