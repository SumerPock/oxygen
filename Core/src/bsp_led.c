#include "bsp.h"


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ����LED����
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: 
*	����˵��: 1,2,3,4 ���ͨ��LED����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SET_OutputChannel_LEDInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	
	rcu_periph_clock_enable(RCU_AF);
	/*pb3 �ܽ���ӳ��*/
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
*	�� �� ��: 
*	����˵��: Run LED����
*	��    ��: ��
*	�� �� ֵ: ��
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


/*ͨ����
1-
2-
3-
4-
******
1-��ɫ
2-��ɫ
3-��ɫ,�ص�
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
					gpio_bit_write(GPIOA, GPIO_PIN_8,  SET); 	 //���
					gpio_bit_write(GPIOC, GPIO_PIN_9,  RESET); //�̿�							
				break;
				case 2:
					gpio_bit_write(GPIOA, GPIO_PIN_8,  RESET); //���
					gpio_bit_write(GPIOC, GPIO_PIN_9,  SET); //�̹�							
				break;
				case 3:
					gpio_bit_write(GPIOA, GPIO_PIN_8,  SET); //���
					gpio_bit_write(GPIOC, GPIO_PIN_9,  SET); //�̹�						
				break;
			}
			break;
		
		case 2:
			switch(nclocer)
			{
				case 1:
				gpio_bit_write(GPIOB, GPIO_PIN_4,  SET); //���
				gpio_bit_write(GPIOB, GPIO_PIN_3,  RESET); //�ƿ�						
				break;
				case 2:
				gpio_bit_write(GPIOB, GPIO_PIN_4,  RESET);//�쿪
				gpio_bit_write(GPIOB, GPIO_PIN_3,  SET); 	//�̹�							
				break;
				case 3:
				gpio_bit_write(GPIOB, GPIO_PIN_4,  SET); 	 //���
				gpio_bit_write(GPIOB, GPIO_PIN_3,  SET); 	 //�̹�							
				break;
			}		
			break;
		
		case 3:
			switch(nclocer)
			{
				case 1:
					gpio_bit_write(GPIOC, GPIO_PIN_8,  SET);		//���
					gpio_bit_write(GPIOC, GPIO_PIN_7,  RESET);	//�̿�						
				break;
				case 2:
					gpio_bit_write(GPIOC, GPIO_PIN_8,  RESET);	//�쿪
					gpio_bit_write(GPIOC, GPIO_PIN_7,  SET);		//�̹�							
				break;
				case 3:
					gpio_bit_write(GPIOC, GPIO_PIN_8,  SET);		//���
					gpio_bit_write(GPIOC, GPIO_PIN_7,  SET);		//�̹�			
				break;
			}	
			break;
		
		case 4:
			switch(nclocer)
			{
				case 1:
					gpio_bit_write(GPIOA, GPIO_PIN_12, SET);		//���
					gpio_bit_write(GPIOA, GPIO_PIN_11, RESET);	//�̿�					
				break;
				case 2:
					gpio_bit_write(GPIOA, GPIO_PIN_12, RESET);	//�쿪
					gpio_bit_write(GPIOA, GPIO_PIN_11, SET);		//�̹�							
				break;
				case 3:
					gpio_bit_write(GPIOA, GPIO_PIN_12, SET);		//���
					gpio_bit_write(GPIOA, GPIO_PIN_11, SET);		//�̹�			
				break;
			}		
			break;
	}
}

/*��λ��
��1
��2
*/
void setLed_gear(unsigned char number, unsigned char onf)
{
	if(number == 1)
	{
		if(onf == 1)
		{
			gpio_bit_write(GPIOC, GPIO_PIN_5,  RESET);//�߿�
			gpio_bit_write(GPIOC, GPIO_PIN_6,  SET);	//�͹�				
		}else if(onf == 0)
		{
			gpio_bit_write(GPIOC, GPIO_PIN_5,  SET);	//�߹�
			gpio_bit_write(GPIOC, GPIO_PIN_6,  SET);	//�͹�						
		}else if(onf == 2)
		{
			gpio_toggle(GPIOC, GPIO_PIN_5);						//�ߵ���˸
			gpio_bit_write(GPIOC, GPIO_PIN_6,  SET);	//�͹�
		}
	}else if(number == 2)
	{
		if(onf == 1)
		{
			gpio_bit_write(GPIOC, GPIO_PIN_5,  SET);	//�߹�
			gpio_bit_write(GPIOC, GPIO_PIN_6,  RESET);//�Ϳ�			
		}else if(onf == 0)
		{
			gpio_bit_write(GPIOC, GPIO_PIN_5,  SET);//�߹�
			gpio_bit_write(GPIOC, GPIO_PIN_6,  SET);//�ֹ�			
		}
		else if(onf == 2)
		{
			gpio_toggle(GPIOC, GPIO_PIN_6);						//����˸
			gpio_bit_write(GPIOC, GPIO_PIN_5,  SET);	//�߹�
		}
	}
}


/*������*/
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
