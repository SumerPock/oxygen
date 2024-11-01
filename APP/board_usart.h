#ifndef _BOARD_USART_H_
#define _BOARD_USART_H_

/*********************************************************************
 * INCLUDES
 */
 
/*********************************************************************
 * DEFINITIONS
 */

/*********************************************************************
 * API FUNCTIONS
 */
 
void USART_DEBUG_Init(void);
void USART_Init(void);
void UART_Write(uint8_t *pData, uint32_t dataLen);
void UART_Write_String(uint8_t *ch);
void UARTDeBug_Write(uint8_t *pData, uint32_t dataLen);


#endif /* _BOARD_USART_H_ */
