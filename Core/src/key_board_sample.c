//#include "key_board/key_board_sample.h"
#include "bsp.h"

struct key_pin_t {
    GPIO_TypeDef *port;
    uint16_t pin;
    FlagStatus valid;
    FlagStatus invalid;
};

const struct key_pin_t key_pin_sig[] = {
    {
        .port	 		= (GPIO_TypeDef *)KEY_PORT_RUN,
        .pin 			= KEY_PIN_RUN,
        .valid 		= KEY_PRESS_LEVEL_RUN, 			//按下时电平
        .invalid 	= KEY_RELEASE_LEVEL_RUN			//按键空闲
    },
    {
        .port 		= (GPIO_TypeDef *)KEY_PORT_SW2,
        .pin 		  = KEY_PIN_SW2,
        .valid	 	= KEY_PRESS_LEVEL_SW2,
        .invalid 	= KEY_RELEASE_LEVEL_SW2
    },
    {
        .port 		= (GPIO_TypeDef *)KEY_PORT_SW1,
        .pin 		  = KEY_PIN_SW1,
        .valid	 	= KEY_PRESS_LEVEL_SW1,
        .invalid 	= KEY_RELEASE_LEVEL_SW1
    },
};

#if (USER_KEY_BOARD_MATRIX)
const struct key_pin_t key_pin_ctrl[] = {
    {
        .port = KEY_PORT_J135,
        .pin = KEY_PIN_J135,
        .valid = KEY_CTL_LINE_ENABLE,
        .invalid = KEY_CTL_LINE_DISABLE
    },
    {
        .port = KEY_PORT_J246,
        .pin = KEY_PIN_J246,
        .valid = KEY_CTL_LINE_ENABLE,
        .invalid = KEY_CTL_LINE_DISABLE
    },
};
#endif

static inline bool pin_level_get(const void *desc)
{
    struct key_pin_t *pdesc;

    pdesc = (struct key_pin_t *)desc;
		return gpio_input_bit_get(pdesc->port, pdesc->pin) == pdesc->valid;
//    return HAL_GPIO_ReadPin(pdesc->port, pdesc->pin) == pdesc->valid;
}

#if (USER_KEY_BOARD_MATRIX)
static inline void pin_level_set(const void *desc, bool flag)
{
    struct key_pin_t *pdesc;

    pdesc = (struct key_pin_t *)desc;
    HAL_GPIO_WritePin(pdesc->port, pdesc->pin, flag ? pdesc->valid : pdesc->invalid);
}
#endif

const struct key_public_sig_t key_public_sig[] = {
    KEY_PUBLIC_SIG_DEF(KEY_RUN, &key_pin_sig[0], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KEY_SW2, &key_pin_sig[1], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
		KEY_PUBLIC_SIG_DEF(KEY_SW1, &key_pin_sig[2], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
#if (USER_KEY_BOARD_MATRIX)
    KEY_PUBLIC_SIG_DEF(KEY_LEFT, &key_pin_sig[0], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI),
    KEY_PUBLIC_SIG_DEF(KEY_RIGHT, &key_pin_sig[1], pin_level_get, KEY_FLAG_PRESS_CONTINUOUS | KEY_FLAG_PRESS_LONG | KEY_FLAG_RELEASE_LONG | KEY_FLAG_PRESS_MULTI | KEY_FLAG_RELEASE_MULTI)
#endif
};

#if (USER_KEY_BOARD_MATRIX)
const struct key_public_ctrl_t key_public_ctrl[] = {
    KEY_PUBLIC_CTRL_DEF(&key_pin_ctrl[0], pin_level_set),
    KEY_PUBLIC_CTRL_DEF(&key_pin_ctrl[1], pin_level_set),
};
#endif

/*按键 GPIO 初始化*/
void GPIO_Key_Board_Init(void)
{	
    // GPIO时钟使能
    rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_GPIOC);
    // 配置为内部下拉输入模式
    gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
		gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
		gpio_init(GPIOC, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_4); 	

#if (USER_KEY_BOARD_MATRIX)
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    for(i = 0;i < GET_ARRAY_SIZE(key_pin_ctrl);i++)
    {
        GPIO_InitStruct.Pin   = key_pin_ctrl[i].pin;
        HAL_GPIO_Init(key_pin_ctrl[i].port, &GPIO_InitStruct);
    }
#endif

    key_board_init();
#if (USER_KEY_BOARD_MATRIX)
    key_board_register(KEY_BOARD_MATRIX, key_public_sig, GET_ARRAY_SIZE(key_public_sig), key_public_ctrl, GET_ARRAY_SIZE(key_public_ctrl));
#else
    key_board_register(KEY_BOARD_NORMAL, key_public_sig, GET_ARRAY_SIZE(key_public_sig), NULL, 0);
#endif
}
