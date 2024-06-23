#include "bsp.h"


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 板载LED配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SET_Onboard_LEDInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_5);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_6);	
	
	gpio_bit_write(GPIOC, GPIO_PIN_5, SET);
	gpio_bit_write(GPIOC, GPIO_PIN_6, SET);	
}


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 1,2,3,4 输出通道LED配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SET_OutputChannel_LEDInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	
	rcu_periph_clock_enable(RCU_AF);
	/*pb3 管脚重映射*/
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);

	//1
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_bit_write(GPIOA, GPIO_PIN_8, SET);
	gpio_bit_write(GPIOC, GPIO_PIN_9, SET);		
	
	//2
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	gpio_bit_write(GPIOB, GPIO_PIN_4, SET);
	gpio_bit_write(GPIOB, GPIO_PIN_3, SET);		
	
	//3
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	gpio_bit_write(GPIOC, GPIO_PIN_8, SET);
	gpio_bit_write(GPIOC, GPIO_PIN_7, SET);	
	
	//4
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	gpio_bit_write(GPIOA, GPIO_PIN_12, SET);
	gpio_bit_write(GPIOA, GPIO_PIN_11, SET);		
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: Run LED配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SET_Run_LEDInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
	gpio_bit_write(GPIOB, GPIO_PIN_6, SET);
	gpio_bit_write(GPIOB, GPIO_PIN_5, SET);		
}


/*通道灯
1-
2-
3-
4-
******
1-绿色
2-红色
3-无色,关灯
******
*/
void setLed_passage(unsigned char nnumber, unsigned char nclocer)
{
	switch(nnumber)
	{
		case 1:
			switch(nclocer)
			{
				case 1:
					gpio_bit_write(GPIOA, GPIO_PIN_8,  SET); 	 //红关
					gpio_bit_write(GPIOC, GPIO_PIN_9,  RESET); //绿开							
				break;
				case 2:
					gpio_bit_write(GPIOA, GPIO_PIN_8,  RESET); //红关
					gpio_bit_write(GPIOC, GPIO_PIN_9,  SET); //绿关							
				break;
				case 3:
					gpio_bit_write(GPIOA, GPIO_PIN_8,  SET); //红关
					gpio_bit_write(GPIOC, GPIO_PIN_9,  SET); //绿关						
				break;
			}
			break;
		
		case 2:
			switch(nclocer)
			{
				case 1:
				gpio_bit_write(GPIOB, GPIO_PIN_4,  SET); //红关
				gpio_bit_write(GPIOB, GPIO_PIN_3,  RESET); //灯开						
				break;
				case 2:
				gpio_bit_write(GPIOB, GPIO_PIN_4,  RESET);//红开
				gpio_bit_write(GPIOB, GPIO_PIN_3,  SET); 	//绿关							
				break;
				case 3:
				gpio_bit_write(GPIOB, GPIO_PIN_4,  SET); 	 //红关
				gpio_bit_write(GPIOB, GPIO_PIN_3,  SET); 	 //绿关							
				break;
			}		
			break;
		
		case 3:
			switch(nclocer)
			{
				case 1:
					gpio_bit_write(GPIOC, GPIO_PIN_8,  SET);		//红关
					gpio_bit_write(GPIOC, GPIO_PIN_7,  RESET);	//绿开						
				break;
				case 2:
					gpio_bit_write(GPIOC, GPIO_PIN_8,  RESET);	//红开
					gpio_bit_write(GPIOC, GPIO_PIN_7,  SET);		//绿关							
				break;
				case 3:
					gpio_bit_write(GPIOC, GPIO_PIN_8,  SET);		//红关
					gpio_bit_write(GPIOC, GPIO_PIN_7,  SET);		//绿关			
				break;
			}	
			break;
		
		case 4:
			switch(nclocer)
			{
				case 1:
					gpio_bit_write(GPIOA, GPIO_PIN_12, SET);		//红关
					gpio_bit_write(GPIOA, GPIO_PIN_11, RESET);	//绿开					
				break;
				case 2:
					gpio_bit_write(GPIOA, GPIO_PIN_12, RESET);	//红开
					gpio_bit_write(GPIOA, GPIO_PIN_11, SET);		//绿关							
				break;
				case 3:
					gpio_bit_write(GPIOA, GPIO_PIN_12, SET);		//红关
					gpio_bit_write(GPIOA, GPIO_PIN_11, SET);		//绿关			
				break;
			}		
			break;
	}
}

/*挡位灯
高1
低2
*/
void setLed_gear(unsigned char number, unsigned char onf)
{
	if(number == 1)
	{
		if(onf == 1)
		{
			gpio_bit_write(GPIOC, GPIO_PIN_5,  RESET);//高开
			gpio_bit_write(GPIOC, GPIO_PIN_6,  SET);	//低关				
		}else if(onf == 0)
		{
			gpio_bit_write(GPIOC, GPIO_PIN_5,  SET);	//高关
			gpio_bit_write(GPIOC, GPIO_PIN_6,  SET);	//低关						
		}else if(onf == 2)
		{
			gpio_toggle(GPIOC, GPIO_PIN_5);						//高挡闪烁
			gpio_bit_write(GPIOC, GPIO_PIN_6,  SET);	//低关
		}
	}else if(number == 2)
	{
		if(onf == 1)
		{
			gpio_bit_write(GPIOC, GPIO_PIN_5,  SET);	//高关
			gpio_bit_write(GPIOC, GPIO_PIN_6,  RESET);//低开			
		}else if(onf == 0)
		{
			gpio_bit_write(GPIOC, GPIO_PIN_5,  SET);//高关
			gpio_bit_write(GPIOC, GPIO_PIN_6,  SET);//抵关			
		}
		else if(onf == 2)
		{
			gpio_toggle(GPIOC, GPIO_PIN_6);						//低闪烁
			gpio_bit_write(GPIOC, GPIO_PIN_5,  SET);	//高关
		}
	}
}


/*启动灯*/
void setLed_startup(unsigned char nclocer, unsigned char onf)
{
	switch(onf)
	{
		case 0:
			if(nclocer == GREEN)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_5,  SET);
				gpio_bit_write(GPIOB, GPIO_PIN_6,  SET);
			}else if(nclocer == READ)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_5,  SET);
				gpio_bit_write(GPIOB, GPIO_PIN_6,  SET);
			}
			break;
		case 1:
			if(nclocer == GREEN)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_5,  RESET);
				gpio_bit_write(GPIOB, GPIO_PIN_6,  SET);
			}else if(nclocer == READ)
			{
				gpio_bit_write(GPIOB, GPIO_PIN_5,  SET);
				gpio_bit_write(GPIOB, GPIO_PIN_6,  RESET);
			}			
			break;
		case 2:
			if(nclocer == GREEN)
			{
					gpio_toggle(GPIOB, GPIO_PIN_5);
			}else if(nclocer == READ)
			{
					gpio_toggle(GPIOB, GPIO_PIN_6);
			}			
			break;
		
	}
}
