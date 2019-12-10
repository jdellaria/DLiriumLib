################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Audio.cpp \
../DLog.cpp \
../Directory.cpp \
../File.cpp \
../LinuxCommand.cpp \
../TCPSocketClient.cpp \
../TCPSocketListener.cpp \
../UDPSocketClient.cpp \
../UDPSocketListener.cpp \
../md5.cpp \
../tinystr.cpp \
../tinyxml.cpp \
../tinyxmlerror.cpp \
../tinyxmlparser.cpp 

O_SRCS += \
../LinuxCommand.o \
../tinystr.o 

OBJS += \
./Audio.o \
./DLog.o \
./Directory.o \
./File.o \
./LinuxCommand.o \
./TCPSocketClient.o \
./TCPSocketListener.o \
./UDPSocketClient.o \
./UDPSocketListener.o \
./md5.o \
./tinystr.o \
./tinyxml.o \
./tinyxmlerror.o \
./tinyxmlparser.o 

CPP_DEPS += \
./Audio.d \
./DLog.d \
./Directory.d \
./File.d \
./LinuxCommand.d \
./TCPSocketClient.d \
./TCPSocketListener.d \
./UDPSocketClient.d \
./UDPSocketListener.d \
./md5.d \
./tinystr.d \
./tinyxml.d \
./tinyxmlerror.d \
./tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


