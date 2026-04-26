################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_adc.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_clk.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_flash.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_gpio.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_pwr.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_spi0.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_sys.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_timer0.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_uart1.c \
D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_uart3.c 

OBJS += \
./StdPeriphDriver/CH57x_adc.o \
./StdPeriphDriver/CH57x_clk.o \
./StdPeriphDriver/CH57x_flash.o \
./StdPeriphDriver/CH57x_gpio.o \
./StdPeriphDriver/CH57x_pwr.o \
./StdPeriphDriver/CH57x_spi0.o \
./StdPeriphDriver/CH57x_sys.o \
./StdPeriphDriver/CH57x_timer0.o \
./StdPeriphDriver/CH57x_uart1.o \
./StdPeriphDriver/CH57x_uart3.o 

C_DEPS += \
./StdPeriphDriver/CH57x_adc.d \
./StdPeriphDriver/CH57x_clk.d \
./StdPeriphDriver/CH57x_flash.d \
./StdPeriphDriver/CH57x_gpio.d \
./StdPeriphDriver/CH57x_pwr.d \
./StdPeriphDriver/CH57x_spi0.d \
./StdPeriphDriver/CH57x_sys.d \
./StdPeriphDriver/CH57x_timer0.d \
./StdPeriphDriver/CH57x_uart1.d \
./StdPeriphDriver/CH57x_uart3.d 


# Each subdirectory must supply rules for building sources it contributes
StdPeriphDriver/CH57x_adc.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_adc.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_clk.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_clk.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_flash.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_flash.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_gpio.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_gpio.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_pwr.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_pwr.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_spi0.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_spi0.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_sys.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_sys.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_timer0.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_timer0.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_uart1.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_uart1.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH57x_uart3.o: D:/Word/Code/Gitcode/MRS_DATA/CH573EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_uart3.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -DCH573 -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Startup" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\Profile\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\StdPeriphDriver\inc" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\HAL\include" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\Ld" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\LIB" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\SRC\RVMSIS" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\app_drv_fifo" -I"D:\Word\Code\Gitcode\MRS_DATA\CH573EVT\EVT\EXAM\BLE\BLE_UART_TEST0\APP\ble_uart_service" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

