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
#define KEY1_GPIO_PIN                   GPIO_PIN_1
#define KEY_OFF                         0x01
#define KEY_ON                          0x00
#define KEY1                            1
#define KEY2                            2

/*=========================================================================*/
/*                                   输出                                  */
/*=========================================================================*/
#define LED1_GPIO_PORT                  GPIOB
#define LED1_GPIO_PIN                   GPIO_PIN_4
#define LED_OFF                         0x01
#define LED_ON                          0x00
#define LED1                            1
#define LED2                            2

/*********************************************************************
 * API FUNCTIONS
 */
void Key_GPIO_Init(void);
uint8_t Key_GPIO_Read(uint8_t keyNum);

void LED_GPIO_Init(void);
void LED_GPIO_Write(uint8_t ledNum, uint8_t ledMode);
uint8_t LED_GPIO_Read(uint8_t ledNum);

#endif /* _BOARD_GPIO_H_ */
