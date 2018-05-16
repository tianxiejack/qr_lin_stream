################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp 

OBJS += \
./main.o 

CPP_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 -gencode arch=compute_50,code=sm_50  -odir "." -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


