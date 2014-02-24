################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/geAnimation.cpp \
../src/geApp.cpp \
../src/geAppearance.cpp \
../src/geCamera.cpp \
../src/geGraph.cpp \
../src/geLight.cpp \
../src/gePrimitive.cpp \
../src/geScene.cpp \
../src/geShader.cpp \
../src/geTexture.cpp \
../src/imagetools.cpp \
../src/tinystr.cpp \
../src/tinyxml.cpp \
../src/tinyxmlerror.cpp \
../src/tinyxmlparser.cpp 

C_SRCS += \
../src/stb_image.c 

OBJS += \
./src/geAnimation.o \
./src/geApp.o \
./src/geAppearance.o \
./src/geCamera.o \
./src/geGraph.o \
./src/geLight.o \
./src/gePrimitive.o \
./src/geScene.o \
./src/geShader.o \
./src/geTexture.o \
./src/imagetools.o \
./src/stb_image.o \
./src/tinystr.o \
./src/tinyxml.o \
./src/tinyxmlerror.o \
./src/tinyxmlparser.o 

C_DEPS += \
./src/stb_image.d 

CPP_DEPS += \
./src/geAnimation.d \
./src/geApp.d \
./src/geAppearance.d \
./src/geCamera.d \
./src/geGraph.d \
./src/geLight.d \
./src/gePrimitive.d \
./src/geScene.d \
./src/geShader.d \
./src/geTexture.d \
./src/imagetools.d \
./src/tinystr.d \
./src/tinyxml.d \
./src/tinyxmlerror.d \
./src/tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/xs/workspace-cdt/geEngine/include" -O2 -Wall -Wextra -Wconversion -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/xs/workspace-cdt/geEngine/include" -O2 -Wall -Wextra -Wconversion -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


