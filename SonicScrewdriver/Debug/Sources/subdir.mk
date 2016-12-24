################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/DAC.c \
../Sources/Events.c \
../Sources/PatternGen.c \
../Sources/exit.c \
../Sources/main.c 

OBJS += \
./Sources/DAC.o \
./Sources/Events.o \
./Sources/PatternGen.o \
./Sources/exit.o \
./Sources/main.o 

C_DEPS += \
./Sources/DAC.d \
./Sources/Events.d \
./Sources/PatternGen.d \
./Sources/exit.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:\Freescale\KDS_3.0.0\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:\Freescale\KDS_3.0.0\eclipse\ProcessorExpert/lib/Kinetis/iofiles" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/SonicScrewdriver/Sources" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/SonicScrewdriver/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


