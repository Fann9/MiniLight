################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/peripheral.c \
../APP/peripheral_main.c \
../APP/ws2812.c 

OBJS += \
./APP/peripheral.o \
./APP/peripheral_main.o \
./APP/ws2812.o 

C_DEPS += \
./APP/peripheral.d \
./APP/peripheral_main.d \
./APP/ws2812.d 


# Each subdirectory must supply rules for building sources it contributes
APP/%.o: ../APP/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

