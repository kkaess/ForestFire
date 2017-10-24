################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ForestFire.cpp \
../src/ForestFireOpts.cpp \
../src/SimpleStats.cpp 

OBJS += \
./src/ForestFire.o \
./src/ForestFireOpts.o \
./src/SimpleStats.o 

CPP_DEPS += \
./src/ForestFire.d \
./src/ForestFireOpts.d \
./src/SimpleStats.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1z -O3 -Wall -c -fmessage-length=0 -fopenmp -march=native -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


