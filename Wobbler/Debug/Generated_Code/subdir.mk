################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/AD1.c \
../Generated_Code/AdcLdd1.c \
../Generated_Code/CI2C1.c \
../Generated_Code/CLOCK.c \
../Generated_Code/Cpu.c \
../Generated_Code/DACSEL.c \
../Generated_Code/DATA.c \
../Generated_Code/KSDK1.c \
../Generated_Code/LATCH.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/PTA.c \
../Generated_Code/PTB.c \
../Generated_Code/RETRIGGERINT.c \
../Generated_Code/SM1.c \
../Generated_Code/TI1.c \
../Generated_Code/TU1.c \
../Generated_Code/TimerIntLdd1.c \
../Generated_Code/Vectors.c \
../Generated_Code/WAIT1.c 

OBJS += \
./Generated_Code/AD1.o \
./Generated_Code/AdcLdd1.o \
./Generated_Code/CI2C1.o \
./Generated_Code/CLOCK.o \
./Generated_Code/Cpu.o \
./Generated_Code/DACSEL.o \
./Generated_Code/DATA.o \
./Generated_Code/KSDK1.o \
./Generated_Code/LATCH.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/PTA.o \
./Generated_Code/PTB.o \
./Generated_Code/RETRIGGERINT.o \
./Generated_Code/SM1.o \
./Generated_Code/TI1.o \
./Generated_Code/TU1.o \
./Generated_Code/TimerIntLdd1.o \
./Generated_Code/Vectors.o \
./Generated_Code/WAIT1.o 

C_DEPS += \
./Generated_Code/AD1.d \
./Generated_Code/AdcLdd1.d \
./Generated_Code/CI2C1.d \
./Generated_Code/CLOCK.d \
./Generated_Code/Cpu.d \
./Generated_Code/DACSEL.d \
./Generated_Code/DATA.d \
./Generated_Code/KSDK1.d \
./Generated_Code/LATCH.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/PTA.d \
./Generated_Code/PTB.d \
./Generated_Code/RETRIGGERINT.d \
./Generated_Code/SM1.d \
./Generated_Code/TI1.d \
./Generated_Code/TU1.d \
./Generated_Code/TimerIntLdd1.d \
./Generated_Code/Vectors.d \
./Generated_Code/WAIT1.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fsingle-precision-constant  -g3 -I"C:\Freescale\KDS_3.0.0\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:\Freescale\KDS_3.0.0\eclipse\ProcessorExpert/lib/Kinetis/iofiles" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/Wobbler/Sources" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/Wobbler/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


