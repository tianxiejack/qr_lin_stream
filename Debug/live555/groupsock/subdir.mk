################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../live555/groupsock/GroupEId.cpp \
../live555/groupsock/Groupsock.cpp \
../live555/groupsock/GroupsockHelper.cpp \
../live555/groupsock/IOHandlers.cpp \
../live555/groupsock/NetAddress.cpp \
../live555/groupsock/NetInterface.cpp 

C_SRCS += \
../live555/groupsock/inet.c 

OBJS += \
./live555/groupsock/GroupEId.o \
./live555/groupsock/Groupsock.o \
./live555/groupsock/GroupsockHelper.o \
./live555/groupsock/IOHandlers.o \
./live555/groupsock/NetAddress.o \
./live555/groupsock/NetInterface.o \
./live555/groupsock/inet.o 

C_DEPS += \
./live555/groupsock/inet.d 

CPP_DEPS += \
./live555/groupsock/GroupEId.d \
./live555/groupsock/Groupsock.d \
./live555/groupsock/GroupsockHelper.d \
./live555/groupsock/IOHandlers.d \
./live555/groupsock/NetAddress.d \
./live555/groupsock/NetInterface.d 


# Each subdirectory must supply rules for building sources it contributes
live555/groupsock/%.o: ../live555/groupsock/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 -gencode arch=compute_50,code=sm_50  -odir "live555/groupsock" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

live555/groupsock/%.o: ../live555/groupsock/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 -gencode arch=compute_50,code=sm_50  -odir "live555/groupsock" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -DDEBUG1=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -G -g -O0 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


