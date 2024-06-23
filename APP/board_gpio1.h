#ifndef _BOARD_GPIO_H_
#define _BOARD_GPIO_H_

/*********************************************************************
 * INCLUDES
 */
#include "gd32f10x_gpio.h"
 
/*********************************************************************
 * DEFINITIONS
 */
/*=========================================================================*/
/*                                   输入                                  */
/*=========================================================================*/
#define KEY1_GPIO_PORT                  GPIOA
#define KEY1_GPIO_PIN                   GPIO_PIN_7
#define KEY_OFF                         	0x01
#define KEY_ON                          	0x00
#define KEY1                            1

#define KEY2_GPIO_PORT                  GPIOC
#define KEY2_GPIO_PIN                   GPIO_PIN_4
#define KEY2                            2
// #define KEY2_Press()										(gpio_input_bit_get(GPIOC,GPIO_PIN_4) == RESET)	// 按键按下
// #define KEY2_Release()									(gpio_input_bit_get(GPIOC,GPIO_PIN_4) == SET)		// 按键弹起

#define KEYRUN_GPIO_PIN                   GPIO_PIN_6
#define KEYRUN                            3

// 

/*=========================================================================*/
/*                                   输出:灯                                  */
/*=========================================================================*/
#define LEDL_GPIO_PORT                  GPIOC
#define LEDL_GPIO_PIN                   GPIO_PIN_5
#define LED_OFF                         0x01
#define LED_ON                          0x00
#define LEDL                            1

#define LEDH_GPIO_PORT                  GPIOC
#define LEDH_GPIO_PIN                   GPIO_PIN_6
#define LEDH                            2

#define AG_GPIO_PORT                 GPIOC
#define AG_GPIO_PIN                  GPIO_PIN_9
#define AG                           3
#define AR_GPIO_PORT                 GPIOA
#define AR_GPIO_PIN                  GPIO_PIN_8
#define AR                           4

#define BG_GPIO_PORT                 GPIOB
#define BG_GPIO_PIN                  GPIO_PIN_3
#define BG                           5
#define BR_GPIO_PORT                 GPIOB
#define BR_GPIO_PIN                  GPIO_PIN_4
#define BR                           6

#define CG_GPIO_PORT                 GPIOC
#define CG_GPIO_PIN                  GPIO_PIN_7
#define CG                           7
#define CR_GPIO_PORT                 GPIOC
#define CR_GPIO_PIN                  GPIO_PIN_8
#define CR                           8

#define DG_GPIO_PORT                 GPIOA
#define DG_GPIO_PIN                  GPIO_PIN_11
#define DG                           9
#define DR_GPIO_PORT                 GPIOA
#define DR_GPIO_PIN                  GPIO_PIN_12
#define DR                           10

#define EG_GPIO_PORT                 GPIOB
#define EG_GPIO_PIN                  GPIO_PIN_5
#define EG                           11
#define ER_GPIO_PORT                 GPIOB
#define ER_GPIO_PIN                  GPIO_PIN_6
#define ER                           12

#define CHANNEL_GPIO_PORT 							GPIOB
#define CHANNEL1_GPIO_PIN               GPIO_PIN_15
#define CHANNEL2_GPIO_PIN               GPIO_PIN_14
#define CHANNEL3_GPIO_PIN               GPIO_PIN_13
#define CHANNEL4_GPIO_PIN               GPIO_PIN_0

/*********************************************************************
 * API FUNCTIONS
 */
void Key_GPIO_Init(void);
uint8_t Key_GPIO_Read(uint8_t keyNum);

void LED_GPIO_Init(void);
void LED_GPIO_Write(uint8_t ledNum, uint8_t ledMode);

void CHANNEL_GPIO_Init(void);

uint8_t LED_GPIO_Read(uint8_t ledNum);
uint8_t KEY_GPIO_Read(uint8_t keyNum);

#endif /* _BOARD_GPIO_H_ */
