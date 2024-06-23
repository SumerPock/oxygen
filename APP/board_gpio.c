/*********************************************************************
 * INCLUDES
 */
#include "gd32f10x.h"

#include "board_gpio.h"

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/*=========================================================================*/
/*                                   输入                                  */
/*=========================================================================*/
/**
 @brief 按键驱动初始化
 @param 无
 @return 无
*/
void Key_GPIO_Init(void)
{
    // GPIO时钟使能
    rcu_periph_clock_enable(RCU_GPIOA);
    // 配置为内部上拉输入模式
    gpio_init(KEY1_GPIO_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, KEY1_GPIO_PIN);
}

/**
 @brief 获取按键状态
 @param keyNum -[in] 按键编号
 @return 1 - 按下；0 - 松开
*/
uint8_t Key_GPIO_Read(uint8_t keyNum)
{
    uint8_t value = 0;

    if(KEY1 == keyNum)
    {
        value = gpio_input_bit_get(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
    }
//    else if(KEY2 == keyNum)
//    {
//        value = gpio_input_bit_get(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
//    }

    return value;
}

/*=========================================================================*/
/*                                   输出                                  */
/*=========================================================================*/
/**
 @brief LED灯驱动初始化
 @param 无
 @return 无
*/
void LED_GPIO_Init(void)
{
    // PB4管脚默认是NJTRST，要当GPIO，需要重映射
    rcu_periph_clock_enable(RCU_AF);                                    // 管脚复用时钟使能
		gpio_pin_remap_config(GPIO_SWJ_NONJTRST_REMAP, ENABLE);
    
    // GPIO时钟使能
		rcu_periph_clock_enable(RCU_GPIOB);
    // 配置为推挽输出模式
		gpio_init(LED1_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED1_GPIO_PIN);

    LED_GPIO_Write(LED1, LED_OFF);
}

/**
 @brief 配置LED灯工作模式
 @param ledNum -[in] LED灯编号
 @param ledMode -[in] 工作模式
 @return 无
*/
void LED_GPIO_Write(uint8_t ledNum, uint8_t ledMode)
{
    if(LED1 == ledNum)
    {
        gpio_bit_write(LED1_GPIO_PORT, LED1_GPIO_PIN, ledMode);
    }
}

/**
 @brief 获取LED灯工作模式
 @param ledNum -[in] LED灯编号
 @return 工作模式
*/
uint8_t LED_GPIO_Read(uint8_t ledNum)
{
    uint8_t ledMode = 0;
    if(LED1 == ledNum)
    {
        ledMode = gpio_output_bit_get(LED1_GPIO_PORT, LED1_GPIO_PIN);
    }
//    else if(LED2 == ledNum)
//    {
//        ledMode = gpio_output_bit_get(LED2_GPIO_PORT, LED2_GPIO_PIN, &ledMode);
//    }
    return ledMode;
}
