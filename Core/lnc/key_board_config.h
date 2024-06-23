#ifndef __KEY_BOARD_CONFIG_H
#define __KEY_BOARD_CONFIG_H

#define KEY_ENABLE    (1)
#define KEY_DISABLE   (0)

//\\\\\\\\\\\\\\\\\\\\\\\\\配置项\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//最大键盘数
#define KEY_BOARD_MAX_NUM                             (3)

//最大按键数
#define KEY_MAX_NUM                                   (7)

//默认长按触发时间（ms）
#define KEY_DEFAULT_LONG_TRRIGER_TIME                 (2000)

//默认多击时连续两次点击之间的允许间隔时间（ms）
#define KEY_DEFAULT_MULTI_INTERVAL_TIME               (500)

//默认连按初始触发时间（ms）
#define KEY_DEFAULT_CONTINUOUS_INIT_TRRIGER_TIME      (1000)

//默认连按周期触发时间（ms）
#define KEY_DEFAULT_CONTINUOUS_PERIOD_TRRIGER_TIME    (500)

//默认组合时连续两次触发之间的允许间隔时间（ms）
#define KEY_DEFAULT_COMBINE_INTERVAL_TIME             (500)

//默认消抖时间（ms），为0时无消抖功能
#define KEY_DEFAULT_DEBOUNCE_TIME                     (10)

//长按检测支持
#define KEY_LONG_SUPPORT                              KEY_ENABLE

//多击支持
#define KEY_MULTI_SUPPORT                             KEY_ENABLE

//连按支持
#define KEY_CONTINUOUS_SUPPORT                        KEY_ENABLE

//组合支持
#define KEY_COMBINE_SUPPORT                           KEY_ENABLE

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#endif/*__KEY_BOARD_CONFIG_H*/
