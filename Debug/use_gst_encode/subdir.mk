################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../use_gst_encode/gst_encode.cpp 

OBJS += \
./use_gst_encode/gst_encode.o 

CPP_DEPS += \
./use_gst_encode/gst_encode.d 


# Each subdirectory must supply rules for building sources it contributes
use_gst_encode/%.o: ../use_gst_encode/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_50,code=sm_50 -m64 -odir "use_gst_encode" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


