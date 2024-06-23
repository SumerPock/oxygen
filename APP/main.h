#ifndef MAIN_H
#define MAIN_H
#include <stdint.h>
#include "gd32f10x.h"
#include "systick.h"
#include "board_gpio1.h"
#include "board_usart.h"



uint8_t SWRunCheck(void);
uint8_t SW1Check(void);
uint8_t SW2Check(void);
uint8_t SW2Pressed(uint8_t time);
void longPressSwitch1(void);
void longPressSwitch2(void);
void deal_rec_buff(void);		// 串口通信协议hex数据帧缓存处理
void USART0_IRQHandler(void);
void OxygenBiggerThreshold(void);
void OxygenSmallThreshold(void);
#endif

