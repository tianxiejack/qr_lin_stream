################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../live555/UsageEnvironment/HashTable.cpp \
../live555/UsageEnvironment/UsageEnvironment.cpp \
../live555/UsageEnvironment/strDup.cpp 

OBJS += \
./live555/UsageEnvironment/HashTable.o \
./live555/UsageEnvironment/UsageEnvironment.o \
./live555/UsageEnvironment/strDup.o 

CPP_DEPS += \
./live555/UsageEnvironment/HashTable.d \
./live555/UsageEnvironment/UsageEnvironment.d \
./live555/UsageEnvironment/strDup.d 


# Each subdirectory must supply rules for building sources it contributes
live555/UsageEnvironment/%.o: ../live555/UsageEnvironment/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -O3 -gencode arch=compute_50,code=sm_50  -odir "live555/UsageEnvironment" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -O3 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


