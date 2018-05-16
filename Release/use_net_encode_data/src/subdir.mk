################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../use_net_encode_data/src/app_bitsInterfaces.cpp 

OBJS += \
./use_net_encode_data/src/app_bitsInterfaces.o 

CPP_DEPS += \
./use_net_encode_data/src/app_bitsInterfaces.d 


# Each subdirectory must supply rules for building sources it contributes
use_net_encode_data/src/%.o: ../use_net_encode_data/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -O3 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_50,code=sm_50 -m64 -odir "use_net_encode_data/src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -O3 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


