################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PhysicalWorld.cpp \
../bullet_test.cpp \
../scene_drawings.cpp 

OBJS += \
./PhysicalWorld.o \
./bullet_test.o \
./scene_drawings.o 

CPP_DEPS += \
./PhysicalWorld.d \
./bullet_test.d \
./scene_drawings.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/bullet/ -I/usr/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


