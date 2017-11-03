################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/znp_userApp.c 

OBJS += \
./App/znp_userApp.o 

C_DEPS += \
./App/znp_userApp.d 


# Each subdirectory must supply rules for building sources it contributes
App/%.o: ../App/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/kon/linux-learning/znpTest/znptest/App" -I"/home/kon/linux-learning/znpTest/znptest" -I"/home/kon/linux-learning/znpTest/znptest/Profile" -I"/home/kon/linux-learning/znpTest/znptest/ZDO" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


