

#include "bsp.h"

void HAL_TIM_PeriodElapsedCallback(timer_parameter_struct *htim);

//void MX_TIM4_Init(void)
//{
//	rcu_periph_clock_enable(RCU_TIMER4);
//	htim4.prescaler         = 7999;
//  htim4.alignedmode       = TIMER_COUNTER_EDGE;
//  htim4.counterdirection  = TIMER_COUNTER_UP;
//	htim4.period            = 999;
//  htim4.clockdivision     = TIMER_CKDIV_DIV1;
//  htim4.repetitioncounter = 0;
//  timer_init(TIMER4 , &htim5);
//	//timer_master_output_trigger_source_select(TIMER5,TIMER_TRI_OUT_SRC_RESET);
//	nvic_irq_enable(TIMER4_IRQn , 0 , 0);
//	timer_interrupt_enable(TIMER4 , TIMER_INT_UP);	
//}

// ������ʱ����ʼ��
void timer2_init(void) {
//    // ���� TIMER2 ʱ��
//    rcu_periph_clock_enable(RCU_TIMER2);

//    // ���� TIMER2
//    timer_deinit(TIMER2);

//    // ��ʱ��������������
//    //timer_parameter_struct timer_initpara;
//    timer_struct_para_init(&htim2);

//    // ���ö�ʱ����Ԥ��Ƶ��������
//    // ϵͳʱ��Ƶ�� = 72 MHz
//    // Ԥ��Ƶ�� = 7199 -> ��ʱ������Ƶ�� = 72 MHz / (7199 + 1) = 10 kHz
//    // �������� = 10 kHz * 60�� - 1 = 599999

//    htim2.prescaler         = 7999;  // Ԥ��Ƶ��
//    htim2.period            = 999; // �Զ���װ��ֵ
//    htim2.clockdivision     = TIMER_CKDIV_DIV1;
//    htim2.counterdirection  = TIMER_COUNTER_UP;
//    htim2.repetitioncounter = 0;

//    // ��ʼ�� TIMER2
//    timer_init(TIMER2, &htim2);

//    // ��������жϱ�־
//    timer_interrupt_flag_clear(TIMER2, TIMER_INT_UP);

//    // ʹ�ܸ����ж�
//    timer_interrupt_enable(TIMER2, TIMER_INT_UP);

//    // ���� TIMER2
//    timer_enable(TIMER2);

//    // �����ж����ȼ���ʹ���ж�
//    nvic_irq_enable(TIMER2_IRQn, 0, 0);
}

/**
	7200*2500 = 18 000 000�� 18 / 72 = 0.25S Ҳ����250ms��һ���ж�
	7200��ϵͳʱ�Ӻ�TIM��ʱ����һ�Σ���2500�ν�һ���ж�

	10800 * 10000 = 108 000 000  108M/ 108MҲ����1000ms��һ���ն�
	����1S��ʱ��
	htim5.prescaler         = 7999;
		htim5.period            = 99;
		Ϊ100ms
**/
void MX_TIM5_Init(void)
{
	//��ʱ250ms�����ж�
	//16λ��ʱ���������ֵΪ65535
	
	rcu_periph_clock_enable(RCU_TIMER5);
	
//	htim5.prescaler         = 7199;
//	htim5.prescaler         = 10799;
	htim5.prescaler         = 7999;
  htim5.alignedmode       = TIMER_COUNTER_EDGE;
  htim5.counterdirection  = TIMER_COUNTER_UP;
//  htim5.period            = 2499;
	htim5.period            = 999 / 2;
  htim5.clockdivision     = TIMER_CKDIV_DIV1;
  htim5.repetitioncounter = 0;
  timer_init(TIMER5,&htim5);
	
	//timer_master_output_trigger_source_select(TIMER5,TIMER_TRI_OUT_SRC_RESET);
	
	nvic_irq_enable(TIMER5_IRQn , 0 , 0);
	timer_interrupt_enable(TIMER5,TIMER_INT_UP);
}


/*1ms ��ʱ��*/
void MX_TIM6_Init(void)
{
	/*
	10800 * 10 = 108000 ��   
	108000/ 108 000 000 = 0.001
	*/
	
	rcu_periph_clock_enable(RCU_TIMER6);
	
//	htim5.prescaler         = 7199;
	htim6.prescaler         = 7999;
  htim6.alignedmode       = TIMER_COUNTER_EDGE;
  htim6.counterdirection  = TIMER_COUNTER_UP;
//  htim5.period            = 2499;
//	htim6.period            = 1;
	htim6.period            = 2;
  htim6.clockdivision     = TIMER_CKDIV_DIV1;
  htim6.repetitioncounter = 0;
  timer_init(TIMER6 , &htim6);
	
	//timer_master_output_trigger_source_select(TIMER5,TIMER_TRI_OUT_SRC_RESET);
	
	nvic_irq_enable(TIMER6_IRQn , 2 , 1);
	timer_interrupt_enable(TIMER6 , TIMER_INT_UP);
}



void TIMER5_IRQHandler(void)
{
	HAL_TIM_PeriodElapsedCallback(&htim5);
	timer_interrupt_flag_clear(TIMER5 , TIMER_INT_UP);
}

void TIMER6_IRQHandler(void)
{
	HAL_TIM_PeriodElapsedCallback(&htim6);
	timer_interrupt_flag_clear(TIMER6 , TIMER_INT_UP);
}



void HAL_TIM_PeriodElapsedCallback(timer_parameter_struct *htim)
{
	static int looptimer5 = 0;
	static int looptimer6 = 0;
	static int looptimer2s = 0;
	static int looptimer10s = 0;
	static int looptimer10min = 0;
	static int looptimer1min = 0;
	static int looptimer5min = 0;	
	static int looptimer8min = 0;
	static int looptimer10swait = 0;	
	static int looptimer2min = 0;	
	static int looptimerpulse[5] = {0, 0, 0, 0, 0};
	
	static int nswitchloop = 0;
	if(htim->period == htim5.period)//û�ҵ���ô�ж��ж���Դ��ͨ����������������ͬ���趨ֵ���ж��Ǵ��ĸ��жϽ����
	{	
		
		/**��λ��ʱ��**/
		if(settimedata.nlooptimer[0] <= 3)
		{
			settimedata.nlooptimer[0]++;
			settimedata.nlooptimer2s = 0;
		}
		else
		{
			settimedata.nlooptimer[0] = 0;
			settimedata.nlooptimer2s = 1;
		}
		
		
		settimedata.nlooptimer1s++;
				
		/*****	10s����ʱ *****/
		if(settimedata.nlooptimer[1] <= 10)
		{
			if(gFlag10s == 1)
				settimedata.nlooptimer[1]++;
		}else{
			settimedata.nlooptimer[1] = 0;
			//settimedata.nlooptimer10s = 1;
		}		
		
		/***** 30MIN���ӵ���ʱ *****/
		if(settimedata.nlooptimer[5] <= 1800)
		{
			settimedata.nlooptimer[5]++;
		}
		else
		{
			settimedata.nlooptimer[5] = 0;
		}
		
		/*�Զ����̣�ÿ5min����һ��ͨ��*/
		if(gFlagAutoMode == 1)
		{
			//��������Ϊ20Sһ��ͨ��
			 nswitchloop = Getflat.Flag_1234switch;
			 nswitchloop = 4 - nswitchloop;
			 gStartO2 = 1;
			if(settimedata.nlooptimer[2] <= 950)
			{
				settimedata.nlooptimer[2]++;
				if(settimedata.nlooptimer[2] == 20)
				{//��һ��
					switch(Getflat.Flag_1234switch)//�ͷŵ�ǰͨ��
					{
						case 0:
							gFlagPassageOxygen = 1;
							break;
						case 1:
							gFlagPassageOxygen = 2;
							break;
						case 2:
							gFlagPassageOxygen = 3;
							break;
						case 3:
							gFlagPassageOxygen = 4;
							break;
						default:
							break;
					}
				}
				else if(settimedata.nlooptimer[2] == 320)
				{//�ڶ���
					if(Getflat.Flag_1234switch == 0)
					{	
						gFlagPassageOxygen = 2;
					}
					else if(Getflat.Flag_1234switch == 1)
					{
						gFlagPassageOxygen = 3;
					}
					else if(Getflat.Flag_1234switch == 2)
					{
						gFlagPassageOxygen = 4;
					}
					else if(Getflat.Flag_1234switch == 3)
					{
						gFlagPassageOxygen = 1;
					}
				}
				else if(settimedata.nlooptimer[2] == 620)
				{//������
					if(Getflat.Flag_1234switch == 0)
					{	
						gFlagPassageOxygen = 3;
					}
					else if(Getflat.Flag_1234switch == 1)
					{
						gFlagPassageOxygen = 4;
					}
					else if(Getflat.Flag_1234switch == 2)
					{
						gFlagPassageOxygen = 1;
					}
					else if(Getflat.Flag_1234switch == 3)
					{
						gFlagPassageOxygen = 2;
					}				
				}
				else if(settimedata.nlooptimer[2] == 920)
				{//���ı�
					if(Getflat.Flag_1234switch == 0)
					{	
						gFlagPassageOxygen = 4;
					}
					else if(Getflat.Flag_1234switch == 1)
					{
						gFlagPassageOxygen = 1;
					}
					else if(Getflat.Flag_1234switch == 2)
					{
						gFlagPassageOxygen = 2;
					}
					else if(Getflat.Flag_1234switch == 3)
					{
						gFlagPassageOxygen = 3;
					}					
				}				
			}
			else
			{
				settimedata.nlooptimer[2] = 0;
				gFlagAutoMode = 0;
				memarynew = 1;			//�ͷ�10S�������´��ж�
				gStartO2 = 0;				//��������
				gGetO2DataOK = 0;		//���¿�����ֵ
			}
		}
		
		
		
		/***** ����ͨ��1	*****/
		if(gFlagPassageOxygen == 1)
		{
			if(settimedata.nlooptimer[6] <= 4)
			{
				settimedata.nlooptimer[6]++;
				if(settimedata.nlooptimer[6] <= 2)
				{
					SetPassageOxygen(1 , 1);
				}		
				else if(settimedata.nlooptimer[6] > 2 && settimedata.nlooptimer[6] < 4)
				{
					SetPassageOxygen(1 , 0);
				}
			}	
			else{
				setLed_passage(LEDNUM1 , GREEN);
				gFlagPassageOxygen = 0;
				settimedata.nlooptimer[6] = 0;
			}
		}


		/***** ����ͨ��2	*****/
		if(gFlagPassageOxygen == 2)
		{
			if(settimedata.nlooptimer[6] <= 4)
			{
				settimedata.nlooptimer[6]++;
				if(settimedata.nlooptimer[6] <= 2)
				{
					SetPassageOxygen(2, 1);
				}		
				else if(settimedata.nlooptimer[6] > 2 && settimedata.nlooptimer[6] < 4)
				{
					SetPassageOxygen(2 , 0);
				}
			}
			else{
				setLed_passage(LEDNUM2 , GREEN);
				gFlagPassageOxygen = 0;
				settimedata.nlooptimer[6] = 0;		
			}	
		}

		
		
		/***** ����ͨ��3	*****/
		if(gFlagPassageOxygen == 3)
		{
			if(settimedata.nlooptimer[6] <= 4)
			{
				settimedata.nlooptimer[6]++;
				if(settimedata.nlooptimer[6] <= 2)
				{
					SetPassageOxygen(3 , 1);
				}		
				else if(settimedata.nlooptimer[6] > 2 && settimedata.nlooptimer[6] < 4)
				{
					SetPassageOxygen(3 , 0);
				}
			}
			else 
			{
				setLed_passage(LEDNUM3 , GREEN);
				gFlagPassageOxygen = 0;
				settimedata.nlooptimer[6] = 0;						
			}
		}

		
		
		/***** ����ͨ��4	*****/
		if(gFlagPassageOxygen == 4)
		{
			if(settimedata.nlooptimer[6] <= 4)
			{
				settimedata.nlooptimer[6]++;
				if(settimedata.nlooptimer[6] <= 2)
				{
					SetPassageOxygen(4 , 1);
				}		
				else if(settimedata.nlooptimer[6] > 2 && settimedata.nlooptimer[6] < 4)
				{
					SetPassageOxygen(4 , 0);
				}
			}			
			else
			{
				setLed_passage(LEDNUM4 , GREEN);
				gFlagPassageOxygen = 0;
				settimedata.nlooptimer[6] = 0;	
			}
		}
		
		
		/***** �ֶ�����ģʽ	�ȴ�10min*****/
		if(gFlagManualMode == 1)
		{
			if(settimedata.nlooptimer[4] < 600)
			{
				if(gModeO2BigThreshold == 1)
				{
					gFlagStartLedReadFlash = 1;					
				}
				gStartO2 = 1;
				settimedata.nlooptimer[4]++;
				if(settimedata.nlooptimer[4] == 10 || settimedata.nlooptimer[4] == 11)
				{//��ʼ����
					switch(Getflat.Flag_1234switch)
					{
						case 0:
							gFlagPassageOxygen = 1;
							break;
						case 1:
							gFlagPassageOxygen = 2;
							break;
						case 2:
							gFlagPassageOxygen = 3;
							break;
						case 3:
							gFlagPassageOxygen = 4;
							break;
						default:
							break;				
					}					
				}
				if(settimedata.nlooptimer[4] >= 550)
				{//����������ȴ�10MIN�󣬸ı�ͨ���ƺ������Ƶ���ɫ
					switch(Getflat.Flag_1234switch)
					{
						case 0:
							setLed_passage(LEDNUM1, READ);
							break;
						case 1:
							setLed_passage(LEDNUM2, READ);
							break;
						case 2:
							setLed_passage(LEDNUM3, READ);
							break;
						case 3:
							setLed_passage(LEDNUM4, READ);
							break;
						default:
							break;						
					}		
					setLed_startup(READ , 1);					
				}
			}
			else 
			{
				settimedata.nlooptimer[4] = 0;
				gFlagManualMode = 0;
				memarynew = 1;									//�ͷ�10S�������´��ж�
				gStartO2 = 0;
				if(gModeO2BigThreshold == 1)
					gFlagStartLedReadFlash = 0;			//����С������ʱ�ر���˸�����к��
				/**��λ		�Ա���ν������ģʽʱʹ��**/
				setLed_passage(LEDNUM1, NONE);	//�ر�4��ͨ����
				setLed_passage(LEDNUM2, NONE);
				setLed_passage(LEDNUM3, NONE);
				setLed_passage(LEDNUM4, NONE);
				setLed_startup(READ , 0);	//�ر����е�
				gGetO2DataOK = 0;		//���¿�����ֵ
			}
		}
		
		
		/***** �ܾ�����ģʽ	�ȴ�2MIN*****/
		if(rejectMode == 1)
		{
			if(settimedata.nlooptimer[4] < 120)
			{
				settimedata.nlooptimer[4]++;
			}
			else
			{
				settimedata.nlooptimer[4] = 0;
				//ͨ���ƺ쳣��
				switch(Getflat.Flag_1234switch)
				{
					case 0:
						setLed_passage(LEDNUM1, READ);
						break;
					case 1:
						setLed_passage(LEDNUM2, READ);
						break;
					case 2:
						setLed_passage(LEDNUM3, READ);
						break;
					case 3:
						setLed_passage(LEDNUM4, READ);
						break;
					default:
						break;						
				}	
				//��λ���̳���
				if(Getflat.Flag_Gear == 0)
				{
					setLed_gear(GREEN_LOW , 1);
				}else if(Getflat.Flag_Gear == 1)
				{
					setLed_gear(GREEN_HIGHT , 1);
				}
				//���������˸,��������������˺����˸��ô������ȥ�ر�����
				//gFlagStartLedReadFlash = 1;	
				rejectMode = 0;
				gFlagStartLedGreenFlash = 0;	//�ر�,�����̵���˸��־
				memarynew = 1;		//������һ��
				gGetO2DataOK = 0;		//���¿�����ֵ
			}
		}
		/********************************/
		
		
	}
	/*���ڼ�¼ˢ�´���������ʱ*/
	else if(htim->period == htim6.period)
	{
		if(settimedata.nlooptimer[9] <= 100)
		{
			settimedata.nlooptimer[9]++;
		}else
		{
			settimedata.nlooptimer[9] = 0;
			settimedata.nlooptimer100ms = 1;
		}
		key_check(); //����ˢ��	

	}
}

