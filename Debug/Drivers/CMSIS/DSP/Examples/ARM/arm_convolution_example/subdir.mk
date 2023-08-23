################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/arm_convolution_example_f32.c \
../Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/math_helper.c 

OBJS += \
./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/arm_convolution_example_f32.o \
./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/math_helper.o 

C_DEPS += \
./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/arm_convolution_example_f32.d \
./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/math_helper.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/%.o Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/%.su Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/%.cyclo: ../Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/%.c Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U575xx -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_NON_SECURE=1 -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I../AZURE_RTOS/App -I../Middlewares/ST/threadx/common/inc -I../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc -I../Drivers/CMSIS/DSP/Include -I../Drivers/CMSIS/DSP/PrivateInclude -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_convolution_example

clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_convolution_example:
	-$(RM) ./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/arm_convolution_example_f32.cyclo ./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/arm_convolution_example_f32.d ./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/arm_convolution_example_f32.o ./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/arm_convolution_example_f32.su ./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/math_helper.cyclo ./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/math_helper.d ./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/math_helper.o ./Drivers/CMSIS/DSP/Examples/ARM/arm_convolution_example/math_helper.su

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_convolution_example

