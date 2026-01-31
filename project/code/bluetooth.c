#include "bluetooth.h"
#include "pid.h"
#include "menu.h"

static fifo_struct bluetooth_fifo;
char BlueSerial_RxPacket[64];
uint8_t BlueSerial_RxFlag;
uint8 data_buffer[32];
uint8 data_len;

void Bluetooth_Init (void)
{
	fifo_init(&bluetooth_fifo, FIFO_DATA_8BIT, BlueSerial_RxPacket, 64);
    gpio_init(BLUETOOTH_CH9141_RTS_PIN, GPI, 1, GPI_PULL_UP);                   // 初始化流控引脚
    uart_init(UART_8, 9600, UART8_TX_D16, UART8_RX_D17);
    uart_rx_interrupt(UART_8, 1);
}

void BlueSerial_SendByte(uint8_t Byte)
{
	uart_write_byte(UART_8, Byte);
}

void BlueSerial_SendArray(uint8_t *Array, uint16_t Length)
{
	uart_write_buffer (UART_8, Array, Length);
}

void BlueSerial_SendString(char *String)
{
	uart_write_string (UART_8, String);
}

uint32_t BlueSerial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void BlueSerial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		BlueSerial_SendByte(Number / BlueSerial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

void BlueSerial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	BlueSerial_SendString(String);
}

void uart_rx_interrupt_handler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if(kLPUART_RxDataRegFullFlag & LPUART_GetStatusFlags(LPUART8))
	{
			// 接收中断
		// wireless_module_uart_handler();
		uint8_t RxData = uart_read_byte (UART_8);
		
		if (RxState == 0)
		{
			if (RxData == '[' && BlueSerial_RxFlag == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
			if (RxData == ']')
			{
				RxState = 0;
				BlueSerial_RxPacket[pRxPacket] = '\0';
				BlueSerial_RxFlag = 1;
			}
			else
			{
				BlueSerial_RxPacket[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
	
	}
	LPUART_ClearStatusFlags(LPUART8, kLPUART_RxOverrunFlag);    // 不允许删除
}

void BlueTooth_Update (void)
{
		if (BlueSerial_RxFlag == 1)								//有接收
		{
			char *Tag = strtok(BlueSerial_RxPacket, ",");		//标签
			
			if (strcmp(Tag, "key") == 0)						//按键
			{
				char *Name = strtok(NULL, ",");
				char *Action = strtok(NULL, ",");
				
			}
			else if (strcmp(Tag, "slider") == 0)				//滑块
			{
				char *Name = strtok(NULL, ",");
				char *Value = strtok(NULL, ",");
				
				if (strcmp(Name, "AngleKp") == 0)
				{
					parameter[1][0] = atof(Value);
				}
				else if (strcmp(Name, "AngleKi") == 0)
				{
					parameter[1][1] = atof(Value);
				}
				else if (strcmp(Name, "AngleKd") == 0)
				{
					parameter[1][2] = atof(Value);
				}
				else if (strcmp(Name, "SpeedKp") == 0)
				{
					parameter[2][0] = atof(Value);
				}
				else if (strcmp(Name, "SpeedKi") == 0)
				{
					parameter[2][1] = atof(Value);
				}
				else if (strcmp(Name, "SpeedKd") == 0)
				{
					parameter[2][2] = atof(Value);
				}
			}
			else if (strcmp(Tag, "joystick") == 0)				//摇杆
			{
				int8_t LH = atoi(strtok(NULL, ","));
				int8_t LV = atoi(strtok(NULL, ","));
				int8_t RH = atoi(strtok(NULL, ","));
				int8_t RV = atoi(strtok(NULL, ","));
				
				SpeedPID.Target = LV / 25.0;
				TurnPID.Target = RH / 25.0;
			}
			
			BlueSerial_RxFlag = 0;
		}
}
