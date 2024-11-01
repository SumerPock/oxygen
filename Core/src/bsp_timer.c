

#include "bsp.h"
#include <stdio.h>

void HAL_TIM_PeriodElapsedCallback(timer_parameter_struct *htim);


// 基本定时器初始化
void timer2_init(void) {
//    // 启用 TIMER2 时钟
//    rcu_periph_clock_enable(RCU_TIMER2);

//    // 重置 TIMER2
//    timer_deinit(TIMER2);

//    // 定时器基础参数配置
//    //timer_parameter_struct timer_initpara;
//    timer_struct_para_init(&htim2);

//    // 配置定时器：预分频器和周期
//    // 系统时钟频率 = 72 MHz
//    // 预分频器 = 7199 -> 定时器计数频率 = 72 MHz / (7199 + 1) = 10 kHz
//    // 计数周期 = 10 kHz * 60秒 - 1 = 599999

//    htim2.prescaler         = 7999;  // 预分频器
//    htim2.period            = 999; // 自动重装载值
//    htim2.clockdivision     = TIMER_CKDIV_DIV1;
//    htim2.counterdirection  = TIMER_COUNTER_UP;
//    htim2.repetitioncounter = 0;

//    // 初始化 TIMER2
//    timer_init(TIMER2, &htim2);

//    // 清除更新中断标志
//    timer_interrupt_flag_clear(TIMER2, TIMER_INT_UP);

//    // 使能更新中断
//    timer_interrupt_enable(TIMER2, TIMER_INT_UP);

//    // 启动 TIMER2
//    timer_enable(TIMER2);

//    // 配置中断优先级并使能中断
//    nvic_irq_enable(TIMER2_IRQn, 0, 0);
}

/**
	7200*2500 = 18 000 000， 18 / 72 = 0.25S 也就是250ms进一次中断
	7200个系统时钟后，TIM的时钟跳一次，跳2500次进一次中断

	10800 * 10000 = 108 000 000  108M/ 108M也就是1000ms进一次终端
	配置1S定时器
	htim5.prescaler         = 7999;
		htim5.period            = 99;
		为100ms
**/
void MX_TIM5_Init(void)
{
	//定时250ms进入中断
	//16位定时器，最大数值为65535
	
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


/*1ms 定时器*/
void MX_TIM6_Init(void)
{
	/*
	10800 * 10 = 108000 ，   
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
	static int looptimerpulse[5] = {0, 0, 0, 0, 0};
	static int nswitchloop = 0;
	if(htim->period == htim5.period)//没找到怎么判断中断来源，通过和其他计数器不同的设定值来判断是从哪个中断进入的
	{	
		/**复位计时器**/
//		if(settimedata.nlooptimer[0] <= 3)
//		{
//			settimedata.nlooptimer[0]++;
//			settimedata.nlooptimer2s = 0;
//		}
//		else
//		{
//			settimedata.nlooptimer[0] = 0;
//			settimedata.nlooptimer2s = 1;
//		}

		if(settimedata.nlooptimer[0] <= 4)
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
				
		/*****	10s倒计时 *****/
		if(settimedata.nlooptimer[1] <= 10)
		{
			if(gFlag10s == 1)
				settimedata.nlooptimer[1]++;
		}else{
			settimedata.nlooptimer[1] = 0;
			//settimedata.nlooptimer10s = 1;
		}		
		
		/***** 30MIN分钟倒计时 *****/
		if(settimedata.nlooptimer[5] <= 1800)
		{
			settimedata.nlooptimer[5]++;
		}
		else
		{
			settimedata.nlooptimer[5] = 0;
		}
		
		/*自动流程，每5min启动一个通道*/
		if(gFlagAutoMode == 1)
		{
			//测试条件为20S一个通道
			 nswitchloop = Getflat.Flag_1234switch;
			 nswitchloop = 4 - nswitchloop;
			 gStartO2 = 1;
			if(settimedata.nlooptimer[2] <= AUTOMATICMODETIMEALL)//总时间
			{
				settimedata.nlooptimer[2]++;
				printf("auto Release oxygen Mode time = %d \r\n" , settimedata.nlooptimer[2]);
 				if(settimedata.nlooptimer[2] == AUTOMATICMODETIMENUM1)
				{//第一遍
					switch(Getflat.Flag_1234switch)//释放当前通道
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
				else if(settimedata.nlooptimer[2] == AUTOMATICMODETIMENUM2)
				{//第二遍
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
				else if(settimedata.nlooptimer[2] == AUTOMATICMODETIMENUM3)
				{//第三遍
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
				else if(settimedata.nlooptimer[2] == AUTOMATICMODETIMENUM4)
				{//第四遍
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
				printf("auto Release oxygen Mode is over reset uart0IQR \r\n");
				settimedata.nlooptimer[2] = 0;
				gFlagAutoMode = 0;
				memarynew = 1;			//释放10S，用于下次判断
				gStartO2 = 0;				//结束放氧
				gGetO2DataOK = 0;		//重新看氧气值
				
				setLed_startup(GREEN , 0);		/*关闭启动灯*/
				/**	自动 走完**/
				//Getflat.Flag_1234switch = 0;		//这里可以取一个巧，因为第二遍肯定要从1开始，而第一遍可以漏过当前通道之前的通道
				settimedata.nlooptimer[1] = 0;		//3.启动10S定时器，并判读10S内是否启动按下按下
				gFlag10s = 1;											//启动10s定时器，settimedata.nlooptimer[1] 计数器自加
				usart_interrupt_enable(USART0 , USART_INT_RBNE);	
			}
		}
		
		
		
		/***** 放氧通道1	*****/
		if(gFlagPassageOxygen == 1)
		{
			if(settimedata.nlooptimer[6] < 5)
			{
				settimedata.nlooptimer[6]++;
				if(settimedata.nlooptimer[6] <= 3)
				{
					printf("Channel One the GPIO Set Hight time = %d \r\n" , settimedata.nlooptimer[6]);
					SetPassageOxygen(1 , 1);
				}		
				else if(settimedata.nlooptimer[6] > 3 && settimedata.nlooptimer[6] < 5)
				{
					printf("Channel One the GPIO Set Low  time = %d \r\n", settimedata.nlooptimer[6]);
					SetPassageOxygen(1 , 0);
				}
			}	
			else if(settimedata.nlooptimer[6] >= 5)
			{
				setLed_passage(LEDNUM1 , NONE);	//先关
				setLed_passage(LEDNUM1 , GREEN);//开绿灯
				settimedata.nlooptimer[6] = 0;
				gFlagPassageOxygen = 0;
				
			}
		}


		/***** 放氧通道2	*****/
		if(gFlagPassageOxygen == 2)
		{
			if(settimedata.nlooptimer[6] < 5)
			{
				settimedata.nlooptimer[6]++;
				if(settimedata.nlooptimer[6] <= 3)
				{
					printf("Channel two the GPIO Set Hight time = %d \r\n" , settimedata.nlooptimer[6]);
					SetPassageOxygen(2, 1);
				}		
				else if(settimedata.nlooptimer[6] > 3 && settimedata.nlooptimer[6] < 5)
				{
					printf("Channel two the GPIO Set Low time = %d \r\n" , settimedata.nlooptimer[6]);
					SetPassageOxygen(2 , 0);
				}
			}
			else if(settimedata.nlooptimer[6] >= 5)
			{
				setLed_passage(LEDNUM2 , NONE);	//先关
				setLed_passage(LEDNUM2 , GREEN);
				gFlagPassageOxygen = 0;
				settimedata.nlooptimer[6] = 0;		
			}	
		}

		
		
		/***** 放氧通道3	*****/
		if(gFlagPassageOxygen == 3)
		{
			if(settimedata.nlooptimer[6] < 5)
			{
				settimedata.nlooptimer[6]++;
				if(settimedata.nlooptimer[6] <= 3)
				{
					printf("Channel three the GPIO Set Hight time = %d \r\n" , settimedata.nlooptimer[6]);
					SetPassageOxygen(3 , 1);
				}		
				else if(settimedata.nlooptimer[6] > 3 && settimedata.nlooptimer[6] < 5)
				{
					printf("Channel three the GPIO Set Low time = %d \r\n" , settimedata.nlooptimer[6]);
					SetPassageOxygen(3 , 0);
				}
			}
			else if(settimedata.nlooptimer[6] >= 5)
			{
				setLed_passage(LEDNUM3 , NONE);
				setLed_passage(LEDNUM3 , GREEN);
				gFlagPassageOxygen = 0;
				settimedata.nlooptimer[6] = 0;						
			}
		}

		
		
		/***** 放氧通道4	*****/
		if(gFlagPassageOxygen == 4)
		{
			if(settimedata.nlooptimer[6] < 5)
			{
				settimedata.nlooptimer[6]++;
				if(settimedata.nlooptimer[6] <= 3)
				{
					printf("Channel four the GPIO Set Hight time = %d \r\n" , settimedata.nlooptimer[6]);
					SetPassageOxygen(4 , 1);
				}		
				else if(settimedata.nlooptimer[6] > 3 && settimedata.nlooptimer[6] < 5)
				{
					printf("Channel four the GPIO Set Low time = %d \r\n" , settimedata.nlooptimer[6]);
					SetPassageOxygen(4 , 0);
				}
			}			
			else if(settimedata.nlooptimer[6] >= 5)
			{
				setLed_passage(LEDNUM4 , NONE);
				setLed_passage(LEDNUM4 , GREEN);
				gFlagPassageOxygen = 0;
				settimedata.nlooptimer[6] = 0;	
			}
		}
		
		
		/***** 手动放氧模式	等待10min*****/
		if(gFlagManualMode == 1)
		{
			//if(settimedata.nlooptimer[4] < 600)	，500ms的定时器中断，所以需要2倍时间
			if(settimedata.nlooptimer[4] < MANUALOXYGENRELEASEWAITTIMER)	
			{
				if(gModeO2BigThreshold == 1)
				{
					gFlagStartLedReadFlash = 1;					
				}
				gStartO2 = 1;
				settimedata.nlooptimer[4]++;
				printf("Manual operation Mode time = %d \r\n" , settimedata.nlooptimer[4]);
				if(settimedata.nlooptimer[4] == 10 || settimedata.nlooptimer[4] == 11)
				{//开始放氧
					switch(Getflat.Flag_1234switch)
					{
						case 0:
							printf("one number begin Release oxygen \r\n");
							gFlagPassageOxygen = 1;
							break;
						case 1:
							printf("two number begin Release oxygen \r\n");
							gFlagPassageOxygen = 2;
							break;
						case 2:
							printf("three number begin Release oxygen \r\n");
							gFlagPassageOxygen = 3;
							break;
						case 3:
							printf("four number begin Release oxygen \r\n");
							gFlagPassageOxygen = 4;
							break;
						default:
							break;				
					}					
				}
				if(settimedata.nlooptimer[4] >= (MANUALOXYGENRELEASEWAITTIMER - 60))
				{	//放氧结束后等待10MIN后，改变通道灯和启动灯的颜色
					switch(Getflat.Flag_1234switch)
					{
						case 0:
							printf("one number oxygen is over \r\n");
							setLed_passage(LEDNUM1, READ);
							break;
						
						case 1:
							printf("two number oxygen is over \r\n");
							setLed_passage(LEDNUM2, READ);
							break;
						
						case 2:
							printf("three number oxygen is over \r\n");
							setLed_passage(LEDNUM3, READ);
							break;
						
						case 3:
							printf("four number begin Release oxygen \r\n");
							setLed_passage(LEDNUM4, READ);
							break;
						
						default:
							break;						
					}
					setLed_startup(GREEN , 0);			//关绿灯
					setLed_startup(READ , 1);				//红灯常亮									
				}
			}
			else
			{
				printf("reset Uart0IQR ! \r\n");
				settimedata.nlooptimer[4] = 0;
				gFlagManualMode = 0;
				memarynew = 1;									//释放10S，用于下次判断
				gStartO2 = 0;
				if(gModeO2BigThreshold == 1)
					gFlagStartLedReadFlash = 0;		//进入小于条件时关闭闪烁的运行红灯
				
				setLed_startup(GREEN , 0);			//关绿灯
				//setLed_startup(READ , 1);				//红灯常亮	
				
				/**复位		以便二次进入这个模式时使用**/
				switch(Getflat.Flag_1234switch)
				{
					case 0://点亮一通道绿灯
						setLed_passage(LEDNUM1, NONE);//先关后开
						setLed_passage(LEDNUM2, NONE);
						setLed_passage(LEDNUM3, NONE);
						setLed_passage(LEDNUM4, NONE);
						setLed_passage(LEDNUM1, GREEN);
						break;				
					case 1://点亮二通道绿灯
						setLed_passage(LEDNUM1, NONE);//先关后开
						setLed_passage(LEDNUM2, NONE);
						setLed_passage(LEDNUM3, NONE);
						setLed_passage(LEDNUM4, NONE);
						setLed_passage(LEDNUM2, GREEN);
						break;			
					case 2://三通道绿灯
						setLed_passage(LEDNUM1, NONE);//先关后开
						setLed_passage(LEDNUM2, NONE);
						setLed_passage(LEDNUM3, NONE);
						setLed_passage(LEDNUM4, NONE);
						setLed_passage(LEDNUM3, GREEN);
						break;			
					case 3://四通道绿灯
						setLed_passage(LEDNUM1, NONE);//先关后开
						setLed_passage(LEDNUM2, NONE);
						setLed_passage(LEDNUM3, NONE);
						setLed_passage(LEDNUM4, NONE);
						setLed_passage(LEDNUM4, GREEN);
						break;			
					default:
						break;
				}			
				usart_interrupt_enable(USART0 , USART_INT_RBNE);	
				gGetO2DataOK = 0;		//重新看氧气值			
				/**	手动流程走完**/
				settimedata.nlooptimer[1] = 0;		//3.启动10S定时器，并判读10S内是否启动按下按下
				gFlag10s = 1;											//启动10s定时器，settimedata.nlooptimer[1] 计数器自加
			}
		}
		/***** 手动放氧模式	结束*****/
		
		/***** 拒绝放氧模式	等待2MIN*****/
		if(rejectMode == 1)
		{	
			//拒绝放氧模式
			if(settimedata.nlooptimer[4] < REJECTFREEDOYXGENWAITTIMER)
			{
				settimedata.nlooptimer[4]++;
				printf("enter refuse Release oxygen number = %d \r\n",settimedata.nlooptimer[4]);
			}
			else if(settimedata.nlooptimer[4] >= REJECTFREEDOYXGENWAITTIMER)
			{
				printf("enter refuse Release oxygen number = %d \r\n" , settimedata.nlooptimer[4]);
				settimedata.nlooptimer[4] = 0;
				//通道灯红常亮
				printf("number = %d red led  bright\r\n" , Getflat.Flag_1234switch);
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
				//挡位灯绿常亮
				if(Getflat.Flag_Gear == 0)
				{
					setLed_gear(GREEN_LOW , 1);
				}
				else if(Getflat.Flag_Gear == 1)
				{
					setLed_gear(GREEN_HIGHT , 1);
				}
				printf("enter refuse Release oxygen Mode is over");
				//启动红灯闪烁,如果在这里启动了红灯闪烁那么在哪里去关闭它？
				//gFlagStartLedReadFlash = 1;	
				rejectMode = 0;
				gFlagStartLedGreenFlash = 0;	//关闭,启动绿灯闪烁标志
				memarynew = 1;		//开启下一轮
				gGetO2DataOK = 0;		//重新看氧气值
				gStartO2 = 0;
				/**	自动流程走完/手动流程走完/禁止放氧走完**/
				//memarynew = 1;										//释放10S，用于下次判断
				settimedata.nlooptimer[1] = 0;		//3.启动10S定时器，并判读10S内是否启动按下按下
				gFlag10s = 1;											//启动10s定时器，settimedata.nlooptimer[1] 计数器自加
				usart_interrupt_enable(USART0 , USART_INT_RBNE);	
			}
		}
		/********************************/
		
		
	}
	/*用于记录刷新传感器倒计时*/
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
		key_check(); //按键刷新	

	}
}

