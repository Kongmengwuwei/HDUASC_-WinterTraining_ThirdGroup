#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "zf_common_headfile.h"

extern char BlueSerial_RxPacket[];
extern uint8_t BlueSerial_RxFlag;

void Bluetooth_Init (void);
void BlueSerial_SendByte(uint8_t Byte);
void BlueSerial_SendArray(uint8_t *Array, uint16_t Length);
void BlueSerial_SendString(char *String);
void BlueSerial_SendNumber(uint32_t Number, uint8_t Length);
void BlueSerial_Printf(char *format, ...);
void BlueTooth_Update (void);

#endif
