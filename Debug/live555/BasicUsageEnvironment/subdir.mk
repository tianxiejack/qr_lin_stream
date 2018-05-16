################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../live555/BasicUsageEnvironment/BasicHashTable.cpp \
../live555/BasicUsageEnvironment/BasicTaskScheduler.cpp \
../live555/BasicUsageEnvironment/BasicTaskScheduler0.cpp \
../live555/BasicUsageEnvironment/BasicUsageEnvironment.cpp \
../live555/BasicUsageEnvironment/BasicUsageEnvironment0.cpp \
../live555/BasicUsageEnvironment/DelayQueue.cpp 

OBJS += \
./live555/BasicUsageEnvironment/BasicHashTable.o \
./live555/BasicUsageEnvironment/BasicTaskScheduler.o \
./live555/BasicUsageEnvironment/BasicTaskScheduler0.o \
./live555/BasicUsageEnvironment/BasicUsageEnvironment.o \
./live555/BasicUsageEnvironment/BasicUsageEnvironment0.o \
./live555/BasicUsageEnvironment/DelayQueue.o 

CPP_DEPS += \
./live555/BasicUsageEnvironment/BasicHashTable.d \
./live555/BasicUsageEnvironment/BasicTaskScheduler.d \
./live555/BasicUsageEnvironment/BasicTaskScheduler0.d \
./live555/BasicUsageEnvironment/BasicUsageEnvironment.d \
./live555/BasicUsageEnvironment/BasicUsageEnvironment0.d \
./live555/BasicUsageEnvironment/DelayQueue.d 


# Each subdirectory must supply rules for building sources it contributes
live555/BasicUsageEnvironment/%.o: ../live555/BasicUsageEnvironment/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_50,code=sm_50 -m64 -odir "live555/BasicUsageEnvironment" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


