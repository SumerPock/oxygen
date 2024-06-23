#ifndef BSP_H
#define BSP_H

#include "bsp_timer.h"
#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "bsp_led.h"
#include "bsp_key.h"


//#include "key_board"
#include "key_board_config.h"
#include "key_board_sample.h"
#include "key_board.h"
#include "key_board_config.h"

#include <stdint.h>
#include "gd32f10x.h"


//extern timer_parameter_struct htim4;

extern timer_parameter_struct htim5;
extern timer_parameter_struct htim6;

extern timer_parameter_struct htim2;

typedef struct
{
  __IO uint32_t CRL;
  __IO uint32_t CRH;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
} GPIO_TypeDef;

typedef struct 
{
	unsigned int nlooptimer[10];
	
	unsigned char nlooptimer100ms;
	unsigned char nlooptimer200ms;
	unsigned char nlooptimer500ms;
	unsigned char nlooptimer1s;
	unsigned char nlooptimer2s;
	unsigned char nlooptimer3s;
	unsigned char nlooptimer10s;
	unsigned char nlooptimer1min;
	unsigned char nlooptimer5min;
	unsigned char nlooptimer10min;	
}TimerSet;

extern TimerSet settimedata;

typedef struct
{
	unsigned int Flag_uart;							/*串口解包数据	*/
	unsigned int Flag_renewsensor;			/*传感器数据	刷新*/
	unsigned int Flag_Gear : 1;					/*抵挡/高档 	刷新*/
	unsigned int Flag_1234switch : 2;		/*1234切换*/
	unsigned int Flag_run : 2;					/*运行按键	*/
	unsigned int Flag_GearRedflashing : 1; 	/*挡位红灯闪烁*/
	unsigned int Flag_RunRedflashing : 1;	 	/*启动红灯闪烁*/
	unsigned int Flag_RunCountdown10S : 1; 	/*10S内是否按下启动按键*/
	unsigned int Flag_RunCountdown2S : 1;    /* 二次启动倒计时 */
	unsigned int Flag_RunCountdown10Min : 1;
	unsigned int Flag_RunCountdown2Min : 1;
	unsigned int Flag_Wait10S : 1;
	unsigned int Flag_Over : 1;
}FLAG;

extern FLAG Getflat;

void gpio_toggle(uint32_t gpio_periph, uint32_t pin);
void SetPassageOxygen(unsigned char unm, unsigned char onf);

extern int gFlagAutoMode;
extern int gFlagPassageOxygen;
extern int gO2Data;
extern int gFlagPassLedToolg1S;
extern int gFlag10s;
extern int gFlagManualMode;
extern int rejectMode;
extern int gFlagStartLedReadFlash;
extern int gStartO2;
extern int gFlagStartLedGreenFlash;
extern  int memarynew;
extern int gGetO2DataOK;	
extern int gModeO2BigThreshold;
extern int gModeO2SmallThreshold;


#endif


