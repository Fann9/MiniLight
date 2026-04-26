################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/BLE/HAL/MCU.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/BLE/HAL/RTC.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/BLE/HAL/SLEEP.c 

OBJS += \
./HAL/MCU.o \
./HAL/RTC.o \
./HAL/SLEEP.o 

C_DEPS += \
./HAL/MCU.d \
./HAL/RTC.d \
./HAL/SLEEP.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/MCU.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/BLE/HAL/MCU.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/RTC.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/BLE/HAL/RTC.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/SLEEP.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/BLE/HAL/SLEEP.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

