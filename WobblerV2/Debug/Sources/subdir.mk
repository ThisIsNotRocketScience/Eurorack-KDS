################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Events.c \
../Sources/Wobbler2.c \
../Sources/exit.c \
../Sources/main.c 

OBJS += \
./Sources/Events.o \
./Sources/Wobbler2.o \
./Sources/exit.o \
./Sources/main.o 

C_DEPS += \
./Sources/Events.d \
./Sources/Wobbler2.d \
./Sources/exit.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fsingle-precision-constant  -g3 -I"C:\Freescale\KDS_v3\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:\Freescale\KDS_v3\eclipse\ProcessorExpert/lib/Kinetis/iofiles" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/WobblerV2/Sources" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/WobblerV2/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


