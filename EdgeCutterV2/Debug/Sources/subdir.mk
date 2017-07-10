################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/EdgeCutter2.c \
../Sources/Events.c \
../Sources/exit.c \
../Sources/main.c 

OBJS += \
./Sources/EdgeCutter2.o \
./Sources/Events.o \
./Sources/exit.o \
./Sources/main.o 

C_DEPS += \
./Sources/EdgeCutter2.d \
./Sources/Events.d \
./Sources/exit.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:\Freescale\KDS_v3\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:\Freescale\KDS_v3\eclipse\ProcessorExpert/lib/Kinetis/iofiles" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/EdgeCutterV2/Sources" -I"C:/Projects/Code/Kinetis2/Eurorack-KDS/EdgeCutterV2/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


