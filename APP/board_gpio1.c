/*********************************************************************
 * INCLUDES
 */
#include "gd32f10x.h"

#include "board_gpio1.h"

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
		rcu_periph_clock_enable(RCU_GPIOC);
    // 配置为内部下拉输入模式
    gpio_init(KEY1_GPIO_PORT, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, KEY1_GPIO_PIN);
		gpio_init(KEY2_GPIO_PORT, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, KEY2_GPIO_PIN);
		gpio_init(KEY1_GPIO_PORT, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, KEYRUN_GPIO_PIN); 
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
		else if(KEY2 == keyNum)
		{
				value = gpio_input_bit_get(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
		}
		else
		{
				value = gpio_input_bit_get(KEY1_GPIO_PORT, KEYRUN_GPIO_PIN);
		}
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
    // GPIO时钟使能
		rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_GPIOB);
		rcu_periph_clock_enable(RCU_GPIOC);
	
    // 配置为推挽输出模式
		gpio_init(LEDH_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LEDH_GPIO_PIN);
		gpio_init(LEDL_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LEDL_GPIO_PIN);
		
		gpio_init(AR_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, AR_GPIO_PIN);
		gpio_init(AG_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, AG_GPIO_PIN);
		gpio_init(BR_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, BR_GPIO_PIN);
		gpio_init(BG_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, BG_GPIO_PIN);
		gpio_init(CR_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CR_GPIO_PIN);
		gpio_init(CG_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CG_GPIO_PIN);
		gpio_init(DR_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DR_GPIO_PIN);
		gpio_init(DG_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DG_GPIO_PIN);
		gpio_init(ER_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ER_GPIO_PIN);
		gpio_init(EG_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, EG_GPIO_PIN);
	
    LED_GPIO_Write(LEDL, LED_OFF);
		LED_GPIO_Write(LEDH, LED_OFF);
		LED_GPIO_Write(AG, LED_OFF);
		LED_GPIO_Write(AR, LED_OFF);
		LED_GPIO_Write(BG, LED_OFF);
		LED_GPIO_Write(BR, LED_OFF);
		LED_GPIO_Write(CG, LED_OFF);
		LED_GPIO_Write(CR, LED_OFF);
		LED_GPIO_Write(DG, LED_OFF);
		LED_GPIO_Write(DR, LED_OFF);
		LED_GPIO_Write(EG, LED_OFF);
		LED_GPIO_Write(ER, LED_OFF);		
}

void CHANNEL_GPIO_Init(void)
{
    // GPIO时钟使能
    rcu_periph_clock_enable(RCU_GPIOB);
		// 配置为内部上拉输出模式
    gpio_init(CHANNEL_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CHANNEL1_GPIO_PIN);
		gpio_init(CHANNEL_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CHANNEL2_GPIO_PIN);
		gpio_init(CHANNEL_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CHANNEL3_GPIO_PIN);
		gpio_init(CHANNEL_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CHANNEL4_GPIO_PIN);
}




void LED_GPIO_Write(uint8_t ledNum, FlagStatus ledMode) // FlagStatus
{
		switch(ledNum)
		{
			case LEDH:
					gpio_bit_write(LEDH_GPIO_PORT, LEDH_GPIO_PIN, ledMode);
					break;
			case LEDL:
					gpio_bit_write(LEDL_GPIO_PORT, LEDL_GPIO_PIN, ledMode);
					break;
			case AG:
					gpio_bit_write(AG_GPIO_PORT, AG_GPIO_PIN, ledMode);
					break;
			case AR:
					gpio_bit_write(AR_GPIO_PORT, AR_GPIO_PIN, ledMode);
					break;
			case BG:
					gpio_bit_write(BG_GPIO_PORT, BG_GPIO_PIN, ledMode);
					break;
			case BR:
					gpio_bit_write(BR_GPIO_PORT, BR_GPIO_PIN, ledMode);
					break;
			case CG:
					gpio_bit_write(CG_GPIO_PORT, CG_GPIO_PIN, ledMode);
					break;
			case CR:
					gpio_bit_write(CR_GPIO_PORT, CR_GPIO_PIN, ledMode);
					break;			
			case DG:
					gpio_bit_write(DG_GPIO_PORT, DG_GPIO_PIN, ledMode);
					break;
			case DR:
					gpio_bit_write(DR_GPIO_PORT, DR_GPIO_PIN, ledMode);
					break;
			case EG:
					gpio_bit_write(EG_GPIO_PORT, EG_GPIO_PIN, ledMode);
					break;
			case ER:
					gpio_bit_write(ER_GPIO_PORT, ER_GPIO_PIN, ledMode);
					break;
			default:
					break;
		}
}

uint8_t LED_GPIO_Read(uint8_t ledNum)
{
    uint8_t ledMode = 0;
    if(LEDH == ledNum)
    {
        ledMode = gpio_output_bit_get(LEDH_GPIO_PORT, LEDH_GPIO_PIN);
    }
		else
		{
				ledMode = gpio_output_bit_get(LEDL_GPIO_PORT, LEDL_GPIO_PIN);
		}
    return ledMode;
}


uint8_t KEY_GPIO_Read(uint8_t keyNum)
{
    uint8_t keyMode = 0;
    if(KEY1 == keyNum)
    {
        keyMode = gpio_input_bit_get(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
    }
		else if (KEY2 == keyNum)
		{
				keyMode = gpio_input_bit_get(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
		}
    return keyMode;
}



