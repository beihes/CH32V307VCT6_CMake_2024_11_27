################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMyLib/Src/cJSON.c 

OBJS += \
./CMyLib/Src/cJSON.o 

C_DEPS += \
./CMyLib/Src/cJSON.d 


# Each subdirectory must supply rules for building sources it contributes
CMyLib/Src/%.o: ../CMyLib/Src/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\MyCode\VSCode\WCH\CH32V307VCT6_CMake_2024_11_27\Debug" -I"E:\MyCode\VSCode\WCH\CH32V307VCT6_CMake_2024_11_27\Core" -I"E:\MyCode\VSCode\WCH\CH32V307VCT6_CMake_2024_11_27\User" -I"E:\MyCode\VSCode\WCH\CH32V307VCT6_CMake_2024_11_27\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

