################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../live555/osa/osa.c \
../live555/osa/osa_buf.c \
../live555/osa/osa_eth_client.c \
../live555/osa/osa_eth_server.c \
../live555/osa/osa_event.c \
../live555/osa/osa_file.c \
../live555/osa/osa_i2c.c \
../live555/osa/osa_mbx.c \
../live555/osa/osa_msgq.c \
../live555/osa/osa_mutex.c \
../live555/osa/osa_pipe.c \
../live555/osa/osa_prf.c \
../live555/osa/osa_que.c \
../live555/osa/osa_rng.c \
../live555/osa/osa_sem.c \
../live555/osa/osa_thr.c \
../live555/osa/osa_tsk.c 

OBJS += \
./live555/osa/osa.o \
./live555/osa/osa_buf.o \
./live555/osa/osa_eth_client.o \
./live555/osa/osa_eth_server.o \
./live555/osa/osa_event.o \
./live555/osa/osa_file.o \
./live555/osa/osa_i2c.o \
./live555/osa/osa_mbx.o \
./live555/osa/osa_msgq.o \
./live555/osa/osa_mutex.o \
./live555/osa/osa_pipe.o \
./live555/osa/osa_prf.o \
./live555/osa/osa_que.o \
./live555/osa/osa_rng.o \
./live555/osa/osa_sem.o \
./live555/osa/osa_thr.o \
./live555/osa/osa_tsk.o 

C_DEPS += \
./live555/osa/osa.d \
./live555/osa/osa_buf.d \
./live555/osa/osa_eth_client.d \
./live555/osa/osa_eth_server.d \
./live555/osa/osa_event.d \
./live555/osa/osa_file.d \
./live555/osa/osa_i2c.d \
./live555/osa/osa_mbx.d \
./live555/osa/osa_msgq.d \
./live555/osa/osa_mutex.d \
./live555/osa/osa_pipe.d \
./live555/osa/osa_prf.d \
./live555/osa/osa_que.d \
./live555/osa/osa_rng.d \
./live555/osa/osa_sem.d \
./live555/osa/osa_thr.d \
./live555/osa/osa_tsk.d 


# Each subdirectory must supply rules for building sources it contributes
live555/osa/%.o: ../live555/osa/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -O3 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_50,code=sm_50 -m64 -odir "live555/osa" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -DALLOW_RTSP_SERVER_PORT_REUSE=1 -I/usr/include/glib-2.0 -I/usr/include/gstreamer-1.0 -I/usr/lib/aarch64-linux-gnu/include -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I../use_gst_encode -I../use_net_encode_data/inc -I../live555/osa/include -I../live555/liveMedia/include -I../live555/groupsock/include -I../live555/UsageEnvironment/include -I../live555/BasicUsageEnvironment/include -O3 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


