################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../znp_struct.c \
../znptest.c 

OBJS += \
./znp_struct.o \
./znptest.o 

C_DEPS += \
./znp_struct.d \
./znptest.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/kon/linux-learning/znpTest/znptest/App" -I"/home/kon/linux-learning/znpTest/znptest" -I"/home/kon/linux-learning/znpTest/znptest/Profile" -I"/home/kon/linux-learning/znpTest/znptest/ZDO" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


