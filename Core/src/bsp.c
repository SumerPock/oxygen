#include "bsp.h"


/*!
    \brief      toggle GPIO pin
    \param[in]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G) 
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_toggle(uint32_t gpio_periph, uint32_t pin)
{
    // 通过按位异或来翻转管脚状态
    if ((GPIO_OCTL(gpio_periph) & pin) == 0) {
        GPIO_BOP(gpio_periph) = pin;
    } else {
        GPIO_BC(gpio_periph) = pin;
    }
}


void SetPassageOxygen(unsigned char unm, unsigned char onf)
{
	switch(unm)
	{
		case 1:
			if(onf == 1)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_15, SET);
				gpio_bit_write(GPIOB, GPIO_PIN_14, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_13, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_0 , RESET);
			}	
			else if(onf == 0)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_15, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_14, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_13, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_0 , RESET);
			}	
			break;
		
		case 2:
			if(onf == 1)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_15, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_14, SET);
				gpio_bit_write(GPIOB, GPIO_PIN_13, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_0 , RESET);
			}	
			else if(onf == 0)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_15, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_14, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_13, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_0 , RESET);
			}	
			break;
		
		case 3:
			if(onf == 1)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_15, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_14, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_13, SET);
				gpio_bit_write(GPIOB, GPIO_PIN_0 , RESET);
			}	
			else if(onf == 0)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_15, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_14, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_13, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_0 , RESET);
			}	
			break;
		
		case 4:
			if(onf == 1)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_15, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_14, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_13, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_0 , SET);
			}	
			else if(onf == 0)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_15, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_14, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_13, RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_0 , RESET);
			}	
			break;	
		
		default:
			break;		
	}
}









