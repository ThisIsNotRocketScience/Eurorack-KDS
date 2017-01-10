################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Events.c \
../Sources/Wobbler.c \
../Sources/exit.c \
../Sources/main.c 

OBJS += \
./Sources/Events.o \
./Sources/Wobbler.o \
./Sources/exit.o \
./Sources/main.o 

C_DEPS += \
./Sources/Events.d \
./Sources/Wobbler.d \
./Sources/exit.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fsingle-precision-constant  -g3 -I"C:\Freescale\KDS_3.0.0\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:\Freescale\KDS_3.0.0\eclipse\ProcessorExpert/lib/Kinetis/iofiles" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/Wobbler/Sources" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/Wobbler/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


