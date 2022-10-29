################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Micro_KeyPad.c \
../Micro_KeyPad_main.c \
../gpio.c \
../keypad.c \
../uart.c 

OBJS += \
./Micro_KeyPad.o \
./Micro_KeyPad_main.o \
./gpio.o \
./keypad.o \
./uart.o 

C_DEPS += \
./Micro_KeyPad.d \
./Micro_KeyPad_main.d \
./gpio.d \
./keypad.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


