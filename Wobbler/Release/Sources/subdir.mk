################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/DAC.c \
../Sources/EdgeCutter.c \
../Sources/Events.c \
../Sources/exit.c \
../Sources/main.c 

OBJS += \
./Sources/DAC.o \
./Sources/EdgeCutter.o \
./Sources/Events.o \
./Sources/exit.o \
./Sources/main.o 

C_DEPS += \
./Sources/DAC.d \
./Sources/EdgeCutter.d \
./Sources/Events.d \
./Sources/exit.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fsingle-precision-constant -flto  -g -I"C:\Freescale\KDS_3.0.0\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:\Freescale\KDS_3.0.0\eclipse\ProcessorExpert/lib/Kinetis/iofiles" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/EdgeCutter/Sources" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/EdgeCutter/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


