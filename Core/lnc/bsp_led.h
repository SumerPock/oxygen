


#ifndef BSP_LED_H
#define BSP_LED_H

enum{
	GREEN_HIGHT = 1,
	GREEN_LOW = 2,
};


enum{
	LEDNUM1 = 1,
	LEDNUM2 = 2,
	LEDNUM3 = 3,
	LEDNUM4 = 4,
};

enum{
	GREEN = 1,
	READ = 2,
	NONE = 3,
};
void setLed_startup(unsigned char nclocer, unsigned char onf);
void setLed_gear(unsigned char number, unsigned char onf);
void setLed_passage(unsigned char nnumber, unsigned char nclocer);
void SET_Onboard_LEDInit(void);
void SET_OutputChannel_LEDInit(void);
void SET_Run_LEDInit(void);


#endif


