#ifndef __KEY_BOARD_H
#define __KEY_BOARD_H

//#include "key_board/key_board_config.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define GET_ARRAY_SIZE(x)    (sizeof(x) / sizeof(*(x)))

#define KEY_FLAG_NONE                                 (0u << 0)

#if (KEY_LONG_SUPPORT == KEY_ENABLE)
#define KEY_FLAG_PRESS_LONG                           (1u << 0)
#define KEY_FLAG_RELEASE_LONG                         (1u << 1)
#endif

#if (KEY_MULTI_SUPPORT == KEY_ENABLE)
#define KEY_FLAG_PRESS_MULTI                          (1u << 2)
#define KEY_FLAG_RELEASE_MULTI                        (1u << 3)
#endif

#if (KEY_CONTINUOUS_SUPPORT == KEY_ENABLE)
#define KEY_FLAG_PRESS_CONTINUOUS                     (1u << 4)
#endif

#define KEY_PUBLIC_SIG_DEF(_id, _pin_desc, _get, _function) \
    {                                                       \
        .id = _id,                                          \
        .pin_desc = _pin_desc,                              \
        .get = _get,                                        \
        .function = _function                               \
    }

#define KEY_PUBLIC_CTRL_DEF(_pin_desc, _set)                \
    {                                                       \
        .pin_desc = _pin_desc,                              \
        .set = _set,                                        \
    }

enum key_state_t {
    KEY_NONE                  = 0u << 0,        /*未按下*/
    KEY_RELEASE               = 1u << 0,        /*弹起*/
    KEY_PRESS                 = 1u << 1,        /*按下*/
    KEY_PRESSING              = 1u << 2,        /*任然按下*/
    KEY_PRESS_DEBOUNCING      = 1u << 3,        /*按下消抖中*/
    KEY_PRESSING_DEBOUNCING   = 1u << 4,        /*弹起消抖中*/

#if (KEY_LONG_SUPPORT == KEY_ENABLE)
    KEY_PRESS_LONG            = 1u << 5,        /*长按（达到长按时间后自动的触发）*/
    KEY_RELEASE_LONG          = 1u << 6,        /*长按（达到长按时间且弹起后触发）*/
#endif

#if (KEY_MULTI_SUPPORT == KEY_ENABLE)
    KEY_PRESS_MULTI           = 1u << 7,        /*多击（多次按下）*/
    KEY_RELEASE_MULTI         = 1u << 8,        /*多击（多次弹起）*/
#endif

#if (KEY_CONTINUOUS_SUPPORT == KEY_ENABLE)
    KEY_PRESS_CONTINUOUS      = 1u << 9,        /*连按*/
#endif
};

typedef bool (*pin_level_get_callback)(const void *);
typedef void (*pin_level_set_callback)(const void *, bool);
typedef void (*print_debug_callback)(const char *);

enum key_board_type_t {
    KEY_BOARD_NORMAL,       //普通的单IO控制按键
    KEY_BOARD_MATRIX        //行列扫描的矩阵键盘
};

struct key_public_sig_t {
    unsigned int id;            //引脚id（注意：不能重复！！！最好设置为连续的值）
    const void *pin_desc;       //引脚描述
    pin_level_get_callback get; //信号引脚电平状态获取
    unsigned int function;      //按键功能标志
};

struct key_public_ctrl_t {
    const void *pin_desc;       //引脚描述
    pin_level_set_callback set; //控制引脚电平状态设置
};

struct key_combine_t {
    unsigned int id;            //按键id
    enum key_state_t state;     //按键状态
};

/**
* @brief    初始化key board
* @retval   成功返回0，失败则返回-1
*/
int key_board_init(void);

/**
* @brief    注册一个键盘
* @param    type： 键盘类型
* @param    sig： 信号线的相关信息
* @param    key_sig_n： 信号线的数量
* @param    ctrl： 控制线的相关信息（仅仅类型为矩阵键盘时需要）
* @param    key_ctrl_n： 控制线的数量（仅仅类型为矩阵键盘时需要）
* @retval   注册成功的键盘对象指针，失败时返回NULL
*/
struct key_board_t *key_board_register(enum key_board_type_t type, const struct key_public_sig_t sig[], unsigned int key_sig_n, const struct key_public_ctrl_t ctrl[], unsigned int key_ctrl_n);

/**
* @brief    卸载已注册的键盘
* @param    obj： 已注册的键盘对象
* @retval   void
*/
void key_board_unregister(struct key_board_t *obj);

/**
* @brief    反初始化key board
* @retval   void
*/
void key_board_destroy(void);

/**
* @brief    检查指定的按键状态
* @param    id： 按键的id
* @param    state： 要检查的按键状态
* @retval   如果要检查的按键状态为多击且处于多击状态，则返回当前的多击次数，否则返回0；
            如果要检查的按键状态为其它且处于要检查的状态，则返回1，否则返回0
*/
unsigned int key_check_state(unsigned int id, enum key_state_t state);

/**
* @brief    获取key board当前按下键的计数
* @retval   void
*/
unsigned int key_press_count_get(void);

/**
* @brief    注册指定的组合状态（任意组合）
* @param    c： 要检查的组合信息
* @param    n： 要检查的组合信息组数
* @retval   注册成功返回一个id，失败则返回0
*/
unsigned int key_combine_register(const struct key_combine_t c[], unsigned int n);

/**
* @brief    卸载指定的组合状态
* @retval   void
*/
void key_combine_unregister(unsigned int id);

/**
* @brief    检查指定的组合状态
* @param    id： 要检查的组合id
* @retval   如果要检查的组合状态满足条件，则返回1，否则返回0
*/
unsigned int key_check_combine_state(unsigned int id);

/**
* @brief    键盘扫描函数（1ms一次！！！）
* @retval   void
*/
void key_check(void);

/**
* @brief    注册调试信息输出函数（不使用时最好不要注册，应在所有其它接口之前调用）
* @param    print_debug： 待注册的调试信息输出函数
* @retval   void
*/
void key_board_debug_register(print_debug_callback print_debug);

#endif/*__KEY_BOARD_H*/
