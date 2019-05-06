################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/config.cpp \
../src/event_plane.cpp \
../src/flatten.cpp \
../src/main.cpp 

OBJS += \
./src/config.o \
./src/event_plane.o \
./src/flatten.o \
./src/main.o 

CPP_DEPS += \
./src/config.d \
./src/event_plane.d \
./src/flatten.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/dylan/Software/root/root-6.14.06/obj/include -O0 -m64 -pthread -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


