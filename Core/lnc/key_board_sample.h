#ifndef __KEY_BOARD_SAMPLE_H
#define __KEY_BOARD_SAMPLE_H

//#include "stm32f1xx_hal.h"
//#include "key_board/key_board.h"

#include "key_board.h"

//如果为矩阵键盘则使能此宏
#define USER_KEY_BOARD_MATRIX 0

/*下拉输入，高有效*/
#define KEY_PORT_RUN           GPIOA
#define KEY_PIN_RUN            GPIO_PIN_6
#define KEY_PRESS_LEVEL_RUN    SET
#define KEY_RELEASE_LEVEL_RUN  RESET

#define KEY_PORT_SW2            GPIOA
#define KEY_PIN_SW2              GPIO_PIN_7
#define KEY_PRESS_LEVEL_SW2      SET
#define KEY_RELEASE_LEVEL_SW2    RESET

#define KEY_PORT_SW1            GPIOC
#define KEY_PIN_SW1             GPIO_PIN_4
#define KEY_PRESS_LEVEL_SW1     SET
#define KEY_RELEASE_LEVEL_SW1   RESET

#if (USER_KEY_BOARD_MATRIX)
/*推挽输出，低使能，高失能*/
#define KEY_PORT_J135           GPIOE
#define KEY_PIN_J135            GPIO_PIN_5
#define KEY_PORT_J246           GPIOE
#define KEY_PIN_J246            GPIO_PIN_6
#define KEY_CTL_LINE_ENABLE     GPIO_PIN_RESET
#define KEY_CTL_LINE_DISABLE    GPIO_PIN_SET
#endif

#define RCC_KEY_BOARD_CLK_ENABLE() do { \
    __HAL_RCC_GPIOA_CLK_ENABLE();       \
    __HAL_RCC_GPIOC_CLK_ENABLE();       \
}while(0)

enum key_id_e {
    KEY_RUN,
    KEY_SW2,
		KEY_SW1,
#if (USER_KEY_BOARD_MATRIX)
    KEY_LEFT,
    KEY_RIGHT
#endif
};

/*按键 GPIO 初始化*/
void GPIO_Key_Board_Init(void);

#endif/*__KEY_BOARD_SAMPLE_H*/
