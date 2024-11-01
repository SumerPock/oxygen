/*********************************************************************
 * INCLUDES
 */
#include <stdio.h>
#include "gd32f10x.h"

#include "board_usart.h"


/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/**
 @brief 串口驱动初始化
 @param 无
 @return 无
*/
void USART_Init(void)
{
    // GPIO时钟使能
    rcu_periph_clock_enable(RCU_GPIOA);
    // USART时钟使能
    rcu_periph_clock_enable(RCU_USART0);
    
    // 配置TX为推挽复用模式
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    // 配置RX为浮空输入模式
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    
    // 配置串口的工作参数
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 9600U);                        	// 波特率
    usart_word_length_set(USART0, USART_WL_8BIT);               // 帧数据字长
    usart_stop_bit_set(USART0, USART_STB_1BIT);                 // 停止位
    usart_parity_config(USART0, USART_PM_NONE);                 // 奇偶校验位
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);		// 硬件流控制RTS
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);		// 硬件流控制CTS
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);         // 使能接收
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);       // 使能发送
    usart_enable(USART0);                                       // 使能串口    
    
    // 使能串口接收中断
    usart_interrupt_enable(USART0, USART_INT_RBNE);
		// 使能USART中断
    nvic_irq_enable(USART0_IRQn, 2, 2);
}

/**调试串口
*/
void USART_DEBUG_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
		rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_UART4);
	
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	
    usart_deinit(UART4);
    usart_baudrate_set(UART4, 921600U);                        	// 波特率
    usart_word_length_set(UART4, USART_WL_8BIT);               // 帧数据字长
    usart_stop_bit_set(UART4, USART_STB_1BIT);                 // 停止位
    usart_parity_config(UART4, USART_PM_NONE);                 // 奇偶校验位
    usart_hardware_flow_rts_config(UART4, USART_RTS_DISABLE);		// 硬件流控制RTS
    usart_hardware_flow_cts_config(UART4, USART_CTS_DISABLE);		// 硬件流控制CTS
    usart_receive_config(UART4, USART_RECEIVE_DISABLE); 
    usart_transmit_config(UART4, USART_TRANSMIT_ENABLE);       // 使能发送
    usart_enable(UART4);                                       // 使能串口    
    
    // 使能串口接收中断
    usart_interrupt_enable(UART4, USART_INT_RBNE);
		// 使能USART中断
    nvic_irq_enable(UART4_IRQn, 3, 2);
}

void UARTDeBug_Write(uint8_t *pData, uint32_t dataLen)
{
    uint8_t i;	
    for(i = 0; i < dataLen; i++)
    {
        usart_data_transmit(UART4, pData[i]);                  // 发送一个字节数据
        while(RESET == usart_flag_get(UART4, USART_FLAG_TBE)); // 发送完成判断
    }
}

/**
 @brief 串口写入数据
 @param pData -[in] 写入数据
 @param dataLen -[in] 写入数据长度
 @return 无
*/
void UART_Write(uint8_t *pData, uint32_t dataLen)
{
    uint8_t i;	
    for(i = 0; i < dataLen; i++)
    {
        usart_data_transmit(USART0, pData[i]);                  // 发送一个字节数据
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE)); // 发送完成判断
    }
}

void UART_Write_String(uint8_t *ch)
{
		uint32_t k = 0;
		while(*(ch+k) != '\0')
		{
				UART_Write(*(ch+k), 1);
			k++;
		}
}

/**
  * @brief 重定向c库函数printf到USARTx
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(UART4, (uint8_t)ch);
    while(RESET == usart_flag_get(UART4, USART_FLAG_TBE));
    return ch;
}

/**
  * @brief 重定向c库函数getchar,scanf到USARTx
  * @retval None
  */
int fgetc(FILE *f)
{
    uint8_t ch = 0;
    ch = usart_data_receive(UART4);
    return ch;
}




/****************************************************END OF FILE****************************************************/
