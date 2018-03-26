################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gst_encode.cpp \
../main.cpp 

OBJS += \
./gst_encode.o \
./main.o 

CPP_DEPS += \
./gst_encode.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I"/home/ubuntu/cuda-workspace/git/live555/liveMedia/include" -I"/home/ubuntu/cuda-workspace/git/live555/groupsock/include" -I"/home/ubuntu/cuda-workspace/git/live555/UsageEnvironment/include" -I"/home/ubuntu/cuda-workspace/git/live555/osa/include" -I"/home/ubuntu/cuda-workspace/git/live555/BasicUsageEnvironment/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


