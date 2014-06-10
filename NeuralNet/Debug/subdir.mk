################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ImageConverter.c \
../NeuralNetRes.c \
../NeuralNetworkLearn.c 

OBJS += \
./ImageConverter.o \
./NeuralNetRes.o \
./NeuralNetworkLearn.o 

C_DEPS += \
./ImageConverter.d \
./NeuralNetRes.d \
./NeuralNetworkLearn.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/data/home/race/workspace/NeuralNet/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


