
//2024年6月23日 初定定稿版本

#include "main.h"
#include "bsp.h"
#include "SEGGER_RTT.h"
#include <stdio.h>

unsigned char recv_buff_len  = 0;
unsigned char recv_buff[64] = {0};
unsigned char FRAMEHEAD = 0X4F;
unsigned char FRAMETAIL = 0x0A;

#define GERO2MODE
//#define O2DATAMOREGEASS		//强制氧气含量大于阈值
//#define O2DATALESSGEASS		//强制氧气含量小于阈值
#define AUTOMODE						//自动模式

//#define HIGHTO2		2470			//高档位氧气阈值	
#define HIGHTO2		1470			//高档位氧气阈值	
#define LOWO2			1240			//低档位氧气阈值


// 获取氧气含量
int oxyge = 0;

unsigned char lunxun[5] = {'M', ' ', '1', '\r', '\n'};
unsigned char  askArray[3] = {'%', '\r', '\n'};

timer_parameter_struct htim5;
timer_parameter_struct htim6;
timer_parameter_struct htim2;

TimerSet settimedata;

FLAG Getflat = {
	0 
, 0 
, 0  //  0 - 低挡(默认) ， 1 - 高档;
, 0	 //  0 - 1 (默认), ..... 3 - 4
, 0	 //  0 - 不运行(默认)	... 1 - 运行
};

int gO2Data = 0;										//用于存储传感器采集到的氧气数据
int gFlagPassLedToolg1S = 0;				//
int gFlag10s = 0;										//
int gFlagPassageOxygen = 0;					//
int gFlagAutoMode = 0;							//当氧气值小于阈值时自动模式标值位
int gFlagStartLedGreenFlash = 0;		//启动绿灯闪烁
int gFlagStartLedReadFlash = 0;			//启动红灯闪烁
int gFlagManualMode = 0;						//手动模式
int gFlagPress10SInner = 0;					//10s内按下了按键
int rejectMode = 0;
int memarynew = 1;
int gStartO2 = 0;										//开启放氧标志位，用于表示开始放氧
int gModeO2BigThreshold = 0;				//传感器氧气值大于设定阈值
int gModeO2SmallThreshold = 0;			//传感器氧气值小于设定阈值
int gGetO2DataOK = 0;								//传感器正确读到了氧气值


/*
    \brief      
    \param[in]  none
    \param[out] none
    \retval     
*/
int main(void)
{
	systick_config();
	
	#ifdef DEBUG
	SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	SEGGER_RTT_SetTerminal(1);
	SEGGER_RTT_printf(0, "SEGGER_RTT_GetKey \r\n");	
	#endif
	
	GPIO_Key_Board_Init();        //按键配置
	SET_Onboard_LEDInit();				//挡位指示灯配置
	SET_OutputChannel_LEDInit();	//输出通道指示灯配置
	SET_Run_LEDInit();						//运行LED配置
	CHANNEL_GPIO_Init();					//输出通道配置
	USART_Init();
	
	MX_TIM6_Init();			//主要用于按键更新
	MX_TIM5_Init();			//主要用于循环功能的实现
	timer_enable(TIMER5);
	timer_enable(TIMER6);

	int Geass_threshold = 1240;	//阈值
//	int Geass_threshold = 2240;		//阈值
	/*默认通道1		绿色灯点亮*/
	setLed_passage(LEDNUM1 , GREEN);
	/*默认挡位低	绿色灯点亮*/
	setLed_gear(GREEN_LOW, 1);
	/*默认运行灯	绿色红色熄灭*/
	setLed_startup(GREEN, 0);
	USART_DEBUG_Init();
	printf("程序开始运行 \r\n");
	
	/**上电后发送M 1 /r/n **/
	unsigned char arrbuffer[] = "M 1\r\n";
	UART_Write(arrbuffer, 4);
	delay_1ms(5);
	UART_Write(arrbuffer, 4);
	delay_1ms(5);
	UART_Write(arrbuffer, 4);
	delay_1ms(5);
	UART_Write(arrbuffer, 4);
	delay_1ms(5);
	UART_Write(arrbuffer, 4);
	delay_1ms(5);

	Getflat.Flag_uart = 1;
	Getflat.Flag_renewsensor = 1;
	Getflat.Flag_Gear = 0;				//0,代表抵挡 1,代表高档
	Getflat.Flag_1234switch = 0;	//0,代表1通道
	Getflat.Flag_run = 0;					//0,代表未触发
	
	while(1){		
		/*1.判断挡位*/	
		if(key_check_state(KEY_SW2, KEY_PRESS))
		{
			Getflat.Flag_Gear++;
			if(Getflat.Flag_Gear == 0)
			{
				setLed_gear(GREEN_LOW, 1);				//抵挡
				Geass_threshold = LOWO2;
			}
			else if(Getflat.Flag_Gear == 1)
			{
				setLed_gear(GREEN_HIGHT, 1);			//高档
				Geass_threshold = HIGHTO2;
			}				
		}
		
		/*2.判断通道*/
		if(key_check_state(KEY_SW1, KEY_PRESS))
		{
			Getflat.Flag_1234switch++;	
			/*气压值大于设定值 通道绿色灯切*/
			if(gModeO2BigThreshold == 1 && gModeO2SmallThreshold == 0)
			{
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
			}
			/*气压值小于设定值 通道红色灯切*/
			else if(gModeO2BigThreshold == 0 && gModeO2SmallThreshold == 1)
			{
				switch(Getflat.Flag_1234switch)
				{
					case 0:
						setLed_passage(LEDNUM1, NONE);//先关后开
						setLed_passage(LEDNUM2, NONE);
						setLed_passage(LEDNUM3, NONE);
						setLed_passage(LEDNUM4, NONE);
						setLed_passage(LEDNUM1, READ);
						break;
					case 1:
						setLed_passage(LEDNUM1, NONE);//先关后开
						setLed_passage(LEDNUM2, NONE);
						setLed_passage(LEDNUM3, NONE);
						setLed_passage(LEDNUM4, NONE);
						setLed_passage(LEDNUM2, READ);
						break;
					case 2:
						setLed_passage(LEDNUM1, NONE);//先关后开
						setLed_passage(LEDNUM2, NONE);
						setLed_passage(LEDNUM3, NONE);
						setLed_passage(LEDNUM4, NONE);
						setLed_passage(LEDNUM3, READ);
						break;
					case 3:
						setLed_passage(LEDNUM1, NONE);//先关后开
						setLed_passage(LEDNUM2, NONE);
						setLed_passage(LEDNUM3, NONE);
						setLed_passage(LEDNUM4, NONE);
						setLed_passage(LEDNUM4, READ);
						break;
					default:
						break;						
				}					
			}
		}
	
		/*3.氧气气压值与阈值做判读，只有当正确接出传感器的值后才进行判断*/	
		#ifdef GERO2MODE
				#ifdef O2DATAMOREGEASS
					gGetO2DataOK = 1;
					gO2Data = 2000;	
				#endif

				#ifdef O2DATALESSGEASS
					gGetO2DataOK = 1;
					gO2Data = 1000;		
				#endif
		
				#ifdef AUTOMODE	
				#endif
		#endif
		/*3.氧气气压值与阈值做判读，只有当正确接出传感器的值后才进行判断*/	
		if(gGetO2DataOK == 1)
		{
			if(gO2Data >= Geass_threshold)
			{
				OxygenBiggerThreshold();
				gModeO2BigThreshold = 1;
				gModeO2SmallThreshold = 0;
			}
			else
			{
				OxygenSmallThreshold();
				gModeO2BigThreshold = 0;
				gModeO2SmallThreshold = 1;
			}
		}
	
		/**气压值小于设定挡位阈值**/
		if(gModeO2SmallThreshold == 1)
		{		
			/*1.挡位灯1S闪一下*/
			if(gFlagPassLedToolg1S == 1)
			{
				if(settimedata.nlooptimer1s >= 1)
				{
					settimedata.nlooptimer1s = 0;
					if(Getflat.Flag_Gear == 0)
						setLed_gear(GREEN_LOW, 2);
					else if(Getflat.Flag_Gear == 1)
						setLed_gear(GREEN_HIGHT, 2);
				}
			}
			//2.相应的通道灯红色灯亮,在OxygenSmallThreshold()中实现
			
			//3.启动10S定时器，并判读10S内是否启动按下按下
			if(settimedata.nlooptimer[1] <= 10)
			{//10s内按下
				memarynew = 2;
				if(key_check_state(KEY_RUN, KEY_PRESS))
				{
					gFlagPress10SInner = 1;				
					settimedata.nlooptimer[0] = 0;
					settimedata.nlooptimer2s = 0;
					Getflat.Flag_run++;
					gFlag10s = 0;	//关闭10S定时器
					settimedata.nlooptimer[1] = 0;
				}
			}
			else if(settimedata.nlooptimer[1] > 10 && settimedata.nlooptimer[1] < 20)//超过了10s
			{
				//4.10S内未按下，进入自动模式,关闭10S定时器
				gFlag10s = 0;
				//5.启动灯绿色亮
				setLed_startup(GREEN , 1);
				//6.依次放氧所有的通道，首先判断是否为通道1，或4
				gFlagAutoMode = 1;
				printf("进入自动放氧模式 \r\n");
				usart_interrupt_disable(USART0 , USART_INT_RBNE);
				settimedata.nlooptimer[1] = 21; //避免重复进入
			}	
			/*2S时间到连续按下运行检测，及双击检测*/
			if(settimedata.nlooptimer2s == 1)
			{
				settimedata.nlooptimer2s = 0;
				Getflat.Flag_run = 0; 					//时间到了之后复位连续按下状态
				//setLed_startup(GREEN , 0);			//关闭运行灯，红绿同时关
			}
			
			//10S按下后是否第二次按下的判断
			if(gFlagPress10SInner == 1)
			{
				if(key_check_state(KEY_RUN, KEY_PRESS))
				{
					Getflat.Flag_run++;
				}
				if (Getflat.Flag_run == 1)//第一次按下，启动绿灯闪烁2S
				{
					gFlagStartLedGreenFlash = 1;	
					if(settimedata.nlooptimer[0] >= 2)
					{
						#ifdef DEBUG
						SEGGER_RTT_SetTerminal(0);
						SEGGER_RTT_printf(0, "nonoO2mode \r\n");
						#endif
						gFlagStartLedGreenFlash = 0;	//关闭启动绿灯闪烁
						setLed_startup(GREEN , 0);		//开绿灯
						rejectMode = 1;								//进入拒绝放氧模式
						usart_interrupt_disable(USART0, USART_INT_RBNE);
						printf("进入拒绝放氧模式 \r\n");
						Getflat.Flag_run = 0;//避免重复进入
					}
				}
				else if(Getflat.Flag_run == 2)//第二次按下,进入手动放氧模式
				{	
					gFlagStartLedGreenFlash = 0;	//关闭启动绿灯闪烁
					//setLed_startup(GREEN , 1);	//关绿灯
					setLed_startup(GREEN , 0);		//关绿灯
					setLed_startup(READ  , 1);		//开红灯
					//gFlagStartLedReadFlash = 1;	//启动红灯闪烁
					gFlagPress10SInner = 0;
					#ifdef DEBUG
					SEGGER_RTT_SetTerminal(0);
					SEGGER_RTT_printf(0, "gFlagManualMode \r\n");
					#endif
					gFlagManualMode = 1;//进入手动放氧模式
					usart_interrupt_disable(USART0, USART_INT_RBNE);
					printf("进入手动放氧模式 \r\n");
				}			
			}
			/**第一次按下启动后，启动绿灯闪烁**/
			if(gFlagStartLedGreenFlash == 1)
			{
				if(settimedata.nlooptimer100ms == 1)
				{
					settimedata.nlooptimer100ms = 0;
					setLed_startup(GREEN , 2);
				}
			}	
			
		}

		/***	以下添加为气压值大于设定挡位阈值	***/
		if(gModeO2BigThreshold == 1)
		{
			/**启动红灯闪烁**/
			if(gFlagStartLedReadFlash == 1)
			{
				if(settimedata.nlooptimer100ms == 1)
				{
					settimedata.nlooptimer100ms = 0;
					setLed_startup(READ , 2);
				}
			}				
		}else if(gModeO2SmallThreshold == 1){
			/**启动红灯闪烁**/
			if(gFlagStartLedReadFlash == 1)
			{
				if(settimedata.nlooptimer100ms == 1)
				{
					settimedata.nlooptimer100ms = 0;
					setLed_startup(READ , 2);
				}
			}					
		}		
		
	}
}

/*
    \brief      氧气气压值大于阈值
    \param[in]  none
    \param[out] none
    \retval     
*/
void OxygenBiggerThreshold(void)
{
	/*3.判断运行*/
	if(key_check_state(KEY_RUN, KEY_RELEASE))
	{
		printf("氧气值大于设定值 , 第一次按下运行按键 \r\n");
		gFlagStartLedReadFlash = 1;//启动红灯闪烁
		Getflat.Flag_run++;	
		#ifdef DEBUG
		SEGGER_RTT_SetTerminal(0);
		SEGGER_RTT_printf(0, "Getflat.Flag_run = [%d] \r\n", Getflat.Flag_run);
		#endif
		//0 -> 1 第一次按下
		//1 -> 2 第二次按下
		//2 -> 3 第三次按下，屏蔽掉
		if(Getflat.Flag_run == 1)
		{
			settimedata.nlooptimer[0] = 0; //按下后复位3S			
		}

		switch(Getflat.Flag_run)
		{
			case 1://第一次按下
			if(settimedata.nlooptimer100ms == 1)
			{
				settimedata.nlooptimer100ms = 0;
				setLed_startup(READ , 2);						//红灯闪烁
				//if(settimedata.nlooptimer2s == 1)
				//gFlagStartLedReadFlash = 0;					//关闭运行红灯闪烁				
			}
			break;

			case 2://第二次按下
				printf("氧气值大于设定值 , 第二次按下运行按键 \r\n");
				usart_interrupt_disable(USART0, USART_INT_RBNE);
				printf("关闭串口中断，避免干扰 \r\n");
//				setLed_startup(GREEN , 1);	//绿灯开启
				gFlagManualMode = 1;  				//手动放氧模式
				gFlagStartLedReadFlash = 1; 	//启动红灯闪烁
			break;

			default:
			break;
		}	
	}
	
	if(Getflat.Flag_run == 1)
	{
		/*2S时间到连续按下运行检测*/
		if(settimedata.nlooptimer2s == 1)
		{
			if(Getflat.Flag_run == 1)
			{
				gFlagStartLedReadFlash = 0;			//关闭运行红灯闪烁
			}
			settimedata.nlooptimer2s = 0;
			Getflat.Flag_run = 0; 					//时间到了之后复位连续按下状态
			setLed_startup(GREEN , 0);			//关闭运行灯，红绿同时关	
			printf("氧气值大于设定值 , 但是没有双击按下复位 \r\n");
		}				
	}
	
	
}
	


/*
    \brief      氧气气压值小于阈值
    \param[in]  none
    \param[out] none
    \retval     
*/
void OxygenSmallThreshold(void)
{
	//1.挡位灯闪烁1S一下
	gFlagPassLedToolg1S  = 1;
	//2.相应的通道灯红色灯亮,按下按键后不进入这里
	if(gStartO2 == 0)
	{
		//一旦开始放氧后就不可以进入此条件，否则会干扰放氧指示灯，此处会强制刷红色
		switch(Getflat.Flag_1234switch)
		{
			case 0://点亮一通道红灯
				setLed_passage(LEDNUM1, READ);
				setLed_passage(LEDNUM2, NONE);
				setLed_passage(LEDNUM3, NONE);
				setLed_passage(LEDNUM4, NONE);
				break;
			case 1://点亮二通道红灯
				setLed_passage(LEDNUM2, READ);
				setLed_passage(LEDNUM1, NONE);
				setLed_passage(LEDNUM3, NONE);
				setLed_passage(LEDNUM4, NONE);
				break;
			case 2://点亮三通道红灯
				setLed_passage(LEDNUM3, READ);
				setLed_passage(LEDNUM1, NONE);
				setLed_passage(LEDNUM2, NONE);
				setLed_passage(LEDNUM4, NONE);
				break;
			case 3://点亮四通道红灯
				setLed_passage(LEDNUM4, READ);
				setLed_passage(LEDNUM1, NONE);
				setLed_passage(LEDNUM2, NONE);
				setLed_passage(LEDNUM3, NONE);
				break;
			default:
				break;	
		}		
	}
	/**	自动流程走完/手动流程走完/禁止放氧走完**/
	if(memarynew == 1)//必须运行一遍任意的一个流程后才能够去启动它
	{
		settimedata.nlooptimer[1] = 0;				//3.启动10S定时器，并判读10S内是否启动按下按下
		gFlag10s = 1;
		//settimedata.nlooptimer10s = 0;		
	}
	
}


/*
    \brief      将ASCII字符串解析为float数据
    \param[in]  none
    \param[out] none
    \retval     
*/
float parseFloatFromAscii(unsigned char* asciiArray, size_t length) {
    // 寻找第一个数字的位置
    size_t startIndex = 0;
    for (; startIndex < length; startIndex++) {
        if (asciiArray[startIndex] >= '0' && asciiArray[startIndex] <= '9') {
            break;
        }
    }
    // 提取数字部分的字符串
    char floatString[32]; // 假设数字部分最多占用32个字符
    strncpy(floatString, (const char*)(asciiArray + startIndex), sizeof(floatString));
    floatString[sizeof(floatString) - 1] = '\0'; // 添加终止符

    // 将字符串转换为float数据
    float result = strtof(floatString, NULL);
    return result;
}




/*
    \brief      串口中断
    \param[in]  none
    \param[out] none
    \retval     
*/
void USART0_IRQHandler(void)
{
    char data = 0;
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
		{
				usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
        data = usart_data_receive(USART0);
        if (data == FRAMEHEAD)
				{
					recv_buff_len = 0;
					recv_buff[recv_buff_len] = data;
					recv_buff_len++;
				}
				else if(data == FRAMETAIL)
				{
					if (recv_buff_len >= 9)
					{
						recv_buff[recv_buff_len] = data;
						float o2data = parseFloatFromAscii(&recv_buff[0], recv_buff_len + 1) * 10;
						gO2Data = o2data;
						printf("当前传感器氧气值 = %d \r\n" , gO2Data);
//						int nO2Data = o2data * 10;
						//mbar			
						gGetO2DataOK = 1;						
//						SEGGER_RTT_SetTerminal(0);
//						SEGGER_RTT_printf(0, "nO2Data = [%d] \r\n", nO2Data);	
						
					}
				}
				else
				{
					if (recv_buff_len > 0 && recv_buff_len < 10-1)
					{
							recv_buff[recv_buff_len] = data;
							recv_buff_len++;
					}
				}
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));//发送完成判断
    }
}

