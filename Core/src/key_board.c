//#include "key_board/key_board.h"
#include "bsp.h"

#if ((KEY_LONG_SUPPORT != KEY_ENABLE) && (KEY_LONG_SUPPORT != KEY_DISABLE))
    #error "KEY_LONG_SUPPORT can be only set to KEY_ENABLE or KEY_DISABLE."
#endif

#if ((KEY_MULTI_SUPPORT != KEY_ENABLE) && (KEY_MULTI_SUPPORT != KEY_DISABLE))
    #error "KEY_MULTI_SUPPORT can be only set to KEY_ENABLE or KEY_DISABLE."
#endif

#if ((KEY_CONTINUOUS_SUPPORT != KEY_ENABLE) && (KEY_CONTINUOUS_SUPPORT != KEY_DISABLE))
    #error "KEY_CONTINUOUS_SUPPORT can be only set to KEY_ENABLE or KEY_DISABLE."
#endif

#if ((KEY_COMBINE_SUPPORT != KEY_ENABLE) && (KEY_COMBINE_SUPPORT != KEY_DISABLE))
    #error "KEY_COMBINE_SUPPORT can be only set to KEY_ENABLE or KEY_DISABLE."
#endif

#if (KEY_BOARD_MAX_NUM <= 0)
    #error "The value of KEY_BOARD_MAX_NUM must be greater than 0."
#endif

#if (KEY_MAX_NUM <= 0)
    #error "The value of KEY_MAX_NUM must be greater than 0."
#endif

#if (KEY_LONG_SUPPORT == KEY_ENABLE)
    #if (KEY_DEFAULT_LONG_TRRIGER_TIME <= 0)
        #error "The value of KEY_DEFAULT_LONG_TRRIGER_TIME must be greater than 0."
    #endif
#endif

#if (KEY_MULTI_SUPPORT == KEY_ENABLE)
    #if (KEY_DEFAULT_MULTI_INTERVAL_TIME <= 0)
        #error "The value of KEY_DEFAULT_MULTI_INTERVAL_TIME must be greater than 0."
    #elif (KEY_DEFAULT_MULTI_INTERVAL_TIME <= KEY_DEFAULT_DEBOUNCE_TIME)
        #warning "It is strongly recommended that the value of KEY_DEFAULT_MULTI_INTERVAL_TIME is greater than the value of KEY_DEFAULT_DEBOUNCE_TIME."
    #endif
#endif

#if (KEY_CONTINUOUS_SUPPORT == KEY_ENABLE)
    #if (KEY_DEFAULT_CONTINUOUS_INIT_TRRIGER_TIME <= 0)
        #error "The value of KEY_DEFAULT_CONTINUOUS_INIT_TRRIGER_TIME must be greater than 0."
    #endif

    #if (KEY_DEFAULT_CONTINUOUS_PERIOD_TRRIGER_TIME <= 0)
        #error "The value of KEY_DEFAULT_CONTINUOUS_PERIOD_TRRIGER_TIME must be greater than 0."
    #endif
#endif

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
    #if (KEY_DEFAULT_COMBINE_INTERVAL_TIME <= 0)
        #error "The value of KEY_DEFAULT_COMBINE_INTERVAL_TIME must be greater than 0."
    #elif (KEY_DEFAULT_COMBINE_INTERVAL_TIME <= KEY_DEFAULT_DEBOUNCE_TIME)
        #warning "It is strongly recommended that the value of KEY_DEFAULT_COMBINE_INTERVAL_TIME is greater than the value of KEY_DEFAULT_DEBOUNCE_TIME."
    #endif
#endif

#if (KEY_DEFAULT_DEBOUNCE_TIME < 0)
    #error "The value of KEY_DEFAULT_DEBOUNCE_TIME must be greater than or equal to 0."
#endif

#define KEY_MAX_TIME        ((1u << ((sizeof(unsigned int) << 3) - 1)) - 1)
#define KEY_TIME_CHECK(x)   ((x) < KEY_MAX_TIME ? (x) : KEY_MAX_TIME - 1)

struct event_cnt_t {
    unsigned int rd_cnt    : 16,
                 wr_cnt    : 16;
};

struct key_private_t {
    struct key_public_sig_t *property;
    enum key_state_t state;                    //按键状态
    bool last_level;                           //按键上次电平（有效/无效）
    bool this_level;                           //按键本次电平（有效/无效）

    struct event_cnt_t release_event;          //弹起事件计数
    struct event_cnt_t press_event;            //按下事件计数
    struct event_cnt_t pressing_event;         //任然按下事件计数

    unsigned int init_debounce_time;           //初始消抖时间
    unsigned int debounce_time;                //消抖计时

#if (KEY_LONG_SUPPORT == KEY_ENABLE)
    struct {
        unsigned int trriger_time;             //触发时间
        unsigned int press_time;               //已按下时间
        bool trriger_flag;                     //触发标志
    }long_press, long_release;

    struct event_cnt_t press_long_event;       //按下长按事件计数
    struct event_cnt_t release_long_event;     //弹起长按事件计数
#endif

#if (KEY_MULTI_SUPPORT == KEY_ENABLE)
    struct {
        unsigned int count;                    //多击次数
        unsigned int timecnt;                  //记录距上次点击的时间
        unsigned int interval;                 //连续两次点击的最大允许间隔时间
    }multi_click_press, multi_click_release;

    struct event_cnt_t press_multi_event;      //按下多击事件计数
    struct event_cnt_t release_multi_event;    //弹起多击事件计数
#endif

#if (KEY_CONTINUOUS_SUPPORT == KEY_ENABLE)
    struct {
        unsigned int trriger_time;             //触发时间
        unsigned int trriger_period;           //触发周期
        unsigned int press_time;               //已按下时间
    }continuous;

    struct event_cnt_t press_continuous_event; //连按事件计数
#endif
};

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
struct {
    unsigned int timecnt;                      //记录距上次触发的时间
    unsigned int interval;                     //连续两次触发的最大允许间隔时间

    struct match_list_t {
        const struct key_combine_t *match;     //需要匹配的组合状态
        unsigned int n;                        //记录组合状态的位数
        unsigned int match_count;              //已匹配成功计数

        struct match_list_t *next;             //用于链接下一个注册的组合状态

        struct event_cnt_t combine_event;      //组合事件计数
    }head;                                     //注册的组合状态的链表头
}static combine;
#endif

struct key_board_t {
    enum key_board_type_t type;                //键盘类型

    unsigned int sig_num;                      //信号线的数量
    struct key_private_t *sig;                 //信号线的相关信息

    //仅用于矩阵键盘
    unsigned int ctrl_num;                     //控制线的数量
    unsigned int sig_per_ctrl;                 //每根控制线上信号线的数量
    struct key_public_ctrl_t *ctrl;            //控制线的相关信息
};

static struct key_board_t *key_board[KEY_BOARD_MAX_NUM];
static unsigned int key_hash_map[KEY_MAX_NUM]; //用于根据id快速查找按键的对象（线性探测法）
static unsigned int key_press_count; //用于记录当前按下键的计数
static print_debug_callback internal_print_debug; //调试信息输出
static unsigned int key_tick_ms; //tick

static inline void debug(const char *func, int line, const char *message)
{
    #include <stdio.h>

    static char buff[128];

    sprintf(buff, "func:%s, line:%u, %s\n", func, line, message);

    if(internal_print_debug)
    {
        internal_print_debug(buff);
    }
}

int key_board_init(void)
{
    memset(key_board, 0, sizeof(key_board));
    for(unsigned int i= 0;i < GET_ARRAY_SIZE(key_hash_map);i++)
    {
        key_hash_map[i] = (unsigned int)-1;
    }
    key_press_count = 0;

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
    memset(&combine, 0, sizeof(combine));
    combine.head.next = NULL;
#endif

    key_tick_ms = 0;

    return 0;
}

struct key_board_t *key_board_register(enum key_board_type_t type, const struct key_public_sig_t sig[], unsigned int key_sig_n, const struct key_public_ctrl_t ctrl[], unsigned int key_ctrl_n)
{
    struct key_board_t *obj;
    unsigned int handle_no_use;
    unsigned int i;

    for(handle_no_use = 0;handle_no_use < GET_ARRAY_SIZE(key_board) && key_board[handle_no_use];handle_no_use++);
    if(handle_no_use >= GET_ARRAY_SIZE(key_board))
    {
        debug(__FUNCTION__, __LINE__, "exceed the max configure value [KEY_BOARD_MAX_NUM]");
        return NULL;
    }
    obj = malloc(sizeof(struct key_board_t));
    if(!obj)
    {
        debug(__FUNCTION__, __LINE__, "malloc failed");
        return NULL;
    }
    memset(obj, 0, sizeof(struct key_board_t));
    key_board[handle_no_use] = obj;
    obj->type = type;
    obj->sig_num = key_sig_n;
    obj->sig = malloc(key_sig_n * sizeof(struct key_private_t));
    if(!obj->sig)
    {
        debug(__FUNCTION__, __LINE__, "malloc failed");
        return NULL;
    }
    memset(obj->sig, 0, key_sig_n * sizeof(struct key_private_t));
    for(i = 0;i < key_sig_n;i++)
    {
        obj->sig[i].property = (struct key_public_sig_t *)&sig[i];

        obj->sig[i].init_debounce_time = KEY_TIME_CHECK(KEY_DEFAULT_DEBOUNCE_TIME);

#if (KEY_LONG_SUPPORT == KEY_ENABLE)
        obj->sig[i].long_press.trriger_time = KEY_TIME_CHECK(KEY_DEFAULT_LONG_TRRIGER_TIME);
        obj->sig[i].long_release.trriger_time = KEY_TIME_CHECK(KEY_DEFAULT_LONG_TRRIGER_TIME);
#endif

#if (KEY_MULTI_SUPPORT == KEY_ENABLE)
        obj->sig[i].multi_click_press.interval = KEY_TIME_CHECK(KEY_DEFAULT_MULTI_INTERVAL_TIME);
        obj->sig[i].multi_click_release.interval = KEY_TIME_CHECK(KEY_DEFAULT_MULTI_INTERVAL_TIME);
#endif

#if (KEY_CONTINUOUS_SUPPORT == KEY_ENABLE)
        obj->sig[i].continuous.trriger_time = KEY_TIME_CHECK(KEY_DEFAULT_CONTINUOUS_INIT_TRRIGER_TIME);
        obj->sig[i].continuous.trriger_period = KEY_TIME_CHECK(KEY_DEFAULT_CONTINUOUS_PERIOD_TRRIGER_TIME);
#endif

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
        combine.interval = KEY_TIME_CHECK(KEY_DEFAULT_COMBINE_INTERVAL_TIME);
#endif

        //通过取模得到插入位置
        unsigned int idx, backup;

        backup = idx = obj->sig[i].property->id % GET_ARRAY_SIZE(key_hash_map);
        while(key_hash_map[idx] != (unsigned int)-1)
        {
            if(obj->sig[key_hash_map[idx]].property->id == obj->sig[i].property->id)
            {
                debug(__FUNCTION__, __LINE__, "the key id is already register");
                //id 已存在
                return NULL;
            }
            idx = (idx + 1) % GET_ARRAY_SIZE(key_hash_map);
            if(idx == backup)
            {
                debug(__FUNCTION__, __LINE__, "exceed the max configure value [KEY_MAX_NUM]");
                //无空位置
                return NULL;
            }
        }
        key_hash_map[idx] = i;
    }

    if(obj->type == KEY_BOARD_MATRIX)
    {
        obj->ctrl_num = key_ctrl_n;
        obj->sig_per_ctrl = key_sig_n / key_ctrl_n;
        obj->ctrl = (struct key_public_ctrl_t *)ctrl;
        for(i = 0;i < key_ctrl_n;i++)
        {
            //将控制线拉到无效电平
            ctrl[i].set(ctrl[i].pin_desc, false);
        }
    }

    return obj;
}

void key_board_unregister(struct key_board_t *obj)
{
    unsigned int i;

    for(i = 0;i < GET_ARRAY_SIZE(key_board) && key_board[i];i++)
    {
        if(obj == key_board[i])
        {
            free(obj->sig);
            obj->sig = NULL;
            free(obj);
            key_board[i] = NULL;
        }
    }
}

void key_board_destroy(void)
{
    struct key_board_t *obj;
    unsigned int i;

    for(i = 0;i < GET_ARRAY_SIZE(key_board) && key_board[i];i++)
    {
        obj = key_board[i];
        free(obj->sig);
        obj->sig = NULL;
        free(obj);
        key_board[i] = NULL;
    }
}

static inline unsigned int key_event(struct event_cnt_t *ev)
{
    unsigned int ret;

    ret = 0;
    if(ev->wr_cnt != ev->rd_cnt)
    {
        ++ev->rd_cnt;
        ret = 1;
    }
    return ret;
}

unsigned int key_check_state(unsigned int id, enum key_state_t state)
{
    struct key_board_t *obj;
    unsigned int i;
    unsigned int idx, backup;

    for(i = 0;i < GET_ARRAY_SIZE(key_board) && key_board[i];i++)
    {
        obj = key_board[i];

        //如果要查找的id对应在hash表中向后查找已经有空位了那这个id肯定不存在
        backup = idx = id % GET_ARRAY_SIZE(key_hash_map);
        while(key_hash_map[idx] != (unsigned int)-1)
        {
            if(obj->sig[key_hash_map[idx]].property->id == id)
            {
                switch(state)
                {
                    case KEY_RELEASE:
                        return key_event(&obj->sig[key_hash_map[idx]].release_event);
                    case KEY_PRESS:
                        return key_event(&obj->sig[key_hash_map[idx]].press_event);
                    case KEY_PRESSING:
                        return key_event(&obj->sig[key_hash_map[idx]].pressing_event);
#if (KEY_LONG_SUPPORT == KEY_ENABLE)
                    case KEY_PRESS_LONG:
                        return key_event(&obj->sig[key_hash_map[idx]].press_long_event);
                    case KEY_RELEASE_LONG:
                        return key_event(&obj->sig[key_hash_map[idx]].release_long_event);
#endif
#if (KEY_MULTI_SUPPORT == KEY_ENABLE)
                    case KEY_PRESS_MULTI:
                        return key_event(&obj->sig[key_hash_map[idx]].press_multi_event) ? obj->sig[key_hash_map[idx]].multi_click_press.count : 0;
                    case KEY_RELEASE_MULTI:
                        return key_event(&obj->sig[key_hash_map[idx]].release_multi_event) ? obj->sig[key_hash_map[idx]].multi_click_release.count : 0;
#endif
#if (KEY_CONTINUOUS_SUPPORT == KEY_ENABLE)
                    case KEY_PRESS_CONTINUOUS:
                        return key_event(&obj->sig[key_hash_map[idx]].press_continuous_event);
#endif
                    default :
                        return 0;
                }
            }
            idx = (idx + 1) % GET_ARRAY_SIZE(key_hash_map);
            if(idx == backup)
            {
                //未找到
                break;
            }
        }
    }

    return 0;
}

unsigned int key_press_count_get(void)
{
    return key_press_count;
}

static inline void key_press_count_stats(struct key_private_t *sig)
{
    if(sig->state & KEY_PRESS)
    {
        ++key_press_count;
    }
    else if(sig->state & KEY_RELEASE)
    {
        --key_press_count;
    }
}

unsigned int key_combine_register(const struct key_combine_t c[], unsigned int n)
{
#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
    struct match_list_t *item;

    item = malloc(sizeof(struct match_list_t));
    if(!item)
    {
        debug(__FUNCTION__, __LINE__, "malloc failed");
        return 0;
    }

    item->match = c;
    item->n = n;
    item->match_count = 0;
    item->next = combine.head.next;
    item->combine_event.rd_cnt = 0;
    item->combine_event.wr_cnt = 0;
    combine.head.next = item;

    return (unsigned int)item;
#else
    return 0;
#endif
}

void key_combine_unregister(unsigned int id)
{
#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
    struct match_list_t *item;
    struct match_list_t *del;

    item = &combine.head;
    while(item->next && (item->next != (struct match_list_t *)id))
    {
        item = item->next;
    }

    if(item->next)
    {
        del = item->next;
        item->next = item->next->next;
        free(del);
    }
#endif
}

unsigned int key_check_combine_state(unsigned int id)
{
#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
    struct match_list_t *item;

    item = (struct match_list_t *)id;

    return key_event(&item->combine_event);
#else
    return 0;
#endif
}

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
static inline void key_check_combine_pre(void)
{
    struct match_list_t *item;

    if(combine.timecnt && (key_tick_ms - combine.timecnt < KEY_MAX_TIME))
    {
        combine.timecnt = 0;

        //两次触发间隔超时
        item = combine.head.next;
        while(item)
        {
            item->match_count = 0;

            item = item->next;
        }
    }
}

static inline void key_check_combine(struct key_private_t *sig)
{
    struct match_list_t *item;
    bool flag;

    item = combine.head.next;
    while(item)
    {
        if(item->match[item->match_count].state & sig->state)
        {
            flag = (item->match[item->match_count].id == sig->property->id);
            if(!flag && item->match_count)
            {
                //本次匹配失败，但是之前已经有成功匹配的，则重新匹配
                item->match_count = 0;
                flag = (item->match[item->match_count].id == sig->property->id) &&
                       (item->match[item->match_count].state & sig->state);
                if(flag)
                {
                    ++item->match_count;
                }
            }
            else if(flag)
            {
                //重新计时
                combine.timecnt = combine.interval + key_tick_ms;

                //匹配成功
                ++item->match_count;
            }
        }

        if(item->match_count == item->n)
        {
            item->match_count = 0;
            ++item->combine_event.wr_cnt;
        }

        item = item->next;
    }
}
#endif

#if (KEY_LONG_SUPPORT == KEY_ENABLE)
static inline void key_check_long(struct key_private_t *sig)
{
    if(sig->state & KEY_PRESS)
    {
        sig->long_press.press_time = sig->long_press.trriger_time + key_tick_ms;
        sig->long_release.press_time = sig->long_release.trriger_time + key_tick_ms;
        sig->long_press.trriger_flag = false;
    }
    else if(sig->state & (KEY_PRESSING | KEY_PRESSING_DEBOUNCING))
    {
        if((sig->property->function & KEY_FLAG_PRESS_LONG) && (key_tick_ms - sig->long_press.press_time < KEY_MAX_TIME))
        {
            if(!sig->long_press.trriger_flag)
            {
                //仅触发一次
                sig->long_press.trriger_flag = true;

                sig->state |= KEY_PRESS_LONG;
                ++sig->press_long_event.wr_cnt;
            }
        }
    }
    else if(sig->state & KEY_RELEASE)
    {
        if((sig->property->function & KEY_FLAG_RELEASE_LONG) && (key_tick_ms - sig->long_release.press_time < KEY_MAX_TIME))
        {
            sig->state |= KEY_RELEASE_LONG;
            ++sig->release_long_event.wr_cnt;
        }
    }
}
#endif

#if (KEY_MULTI_SUPPORT == KEY_ENABLE)
static inline void key_check_multi(struct key_private_t *sig)
{
    if((sig->property->function & KEY_FLAG_PRESS_MULTI) && (sig->state & KEY_PRESS))
    {
        //标记按下次数
        ++sig->multi_click_press.count;
        if(sig->multi_click_press.count >= 2)
        {
            sig->state |= KEY_PRESS_MULTI;
            ++sig->press_multi_event.wr_cnt;
        }
        sig->multi_click_press.timecnt = sig->multi_click_press.interval + key_tick_ms;
    }
    if(sig->multi_click_press.count)
    {
        if(key_tick_ms - sig->multi_click_press.timecnt < KEY_MAX_TIME)
        {
            sig->multi_click_press.count = 0;
        }
    }

    if((sig->property->function & KEY_FLAG_RELEASE_MULTI) && (sig->state & KEY_RELEASE))
    {
        //标记弹起次数
        ++sig->multi_click_release.count;
        if(sig->multi_click_release.count >= 2)
        {
            sig->state |= KEY_RELEASE_MULTI;
            ++sig->release_multi_event.wr_cnt;
        }
        sig->multi_click_release.timecnt = sig->multi_click_release.interval + key_tick_ms;
    }
    if(sig->multi_click_release.count)
    {
        if(key_tick_ms - sig->multi_click_release.timecnt < KEY_MAX_TIME)
        {
            sig->multi_click_release.count = 0;
        }
    }
}
#endif

#if (KEY_CONTINUOUS_SUPPORT == KEY_ENABLE)
static inline void key_check_continuous(struct key_private_t *sig)
{
    if(sig->state & KEY_PRESS)
    {
        sig->continuous.press_time = sig->continuous.trriger_time + key_tick_ms;
    }
    else if(sig->state & (KEY_PRESSING | KEY_PRESSING_DEBOUNCING))
    {
        if((sig->property->function & KEY_FLAG_PRESS_CONTINUOUS) && (key_tick_ms - sig->continuous.press_time < KEY_MAX_TIME))
        {
            sig->state |= KEY_PRESS_CONTINUOUS;
            ++sig->press_continuous_event.wr_cnt;

            sig->continuous.press_time = sig->continuous.trriger_period + key_tick_ms;
        }
    }
}
#endif

void key_check(void)
{
    struct key_board_t *obj;
    unsigned int i, j;
    unsigned int last, next;

    ++key_tick_ms;

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
    key_check_combine_pre();
#endif

    for(i = 0;i < GET_ARRAY_SIZE(key_board) && key_board[i];i++)
    {
        obj = key_board[i];

        last = obj->ctrl_num - 1;
        next = 0;
        for(j = 0;j < obj->sig_num;j++)
        {
            if((obj->type == KEY_BOARD_MATRIX) && ((j % obj->sig_per_ctrl) == 0))
            {
                //上次扫描的控制线拉到无效电平
                obj->ctrl[last].set(obj->ctrl[last].pin_desc, false);

                last = next = j / obj->sig_per_ctrl;

                //即将扫描的控制线拉到有效电平
                obj->ctrl[next].set(obj->ctrl[next].pin_desc, true);
            }

            obj->sig[j].this_level = obj->sig[j].property->get(obj->sig[j].property->pin_desc);
            switch((obj->sig[j].this_level << 4) | (obj->sig[j].last_level))
            {
                case 0x00:
                    ////清0，解决多次消抖产生的BUG
                    obj->sig[j].debounce_time = 0;

                    obj->sig[j].state = KEY_NONE;
                    break;
                case 0x01:
                    if((0 == obj->sig[j].init_debounce_time) ||
                       (obj->sig[j].debounce_time && (key_tick_ms - obj->sig[j].debounce_time < KEY_MAX_TIME)))
                    {
                        //不需要消抖或处于消抖状态并且达到消抖计时
                        obj->sig[j].state = KEY_RELEASE;
                        ++obj->sig[j].release_event.wr_cnt;
                    }
                    else if(0 == obj->sig[j].debounce_time)
                    {
                        obj->sig[j].state = KEY_PRESSING_DEBOUNCING;
                        //置消抖计时
                        obj->sig[j].debounce_time = obj->sig[j].init_debounce_time + key_tick_ms;
                    }
                    break;
                case 0x10:
                    if((0 == obj->sig[j].init_debounce_time) ||
                       (obj->sig[j].debounce_time && (key_tick_ms - obj->sig[j].debounce_time < KEY_MAX_TIME)))
                    {
                        obj->sig[j].state = KEY_PRESS;
                        ++obj->sig[j].press_event.wr_cnt;
                    }
                    else if(0 == obj->sig[j].debounce_time)
                    {
                        obj->sig[j].state = KEY_PRESS_DEBOUNCING;
                        obj->sig[j].debounce_time = obj->sig[j].init_debounce_time + key_tick_ms;
                    }
                    break;
                case 0x11:
                    //清0，解决多次消抖产生的BUG
                    obj->sig[j].debounce_time = 0;

                    obj->sig[j].state = KEY_PRESSING;
                    ++obj->sig[j].pressing_event.wr_cnt;
                    break;
            }

            //如果处于消抖过程则不保存本次电平状态
            if(!(obj->sig[j].state & (KEY_PRESS_DEBOUNCING | KEY_PRESSING_DEBOUNCING)))
            {
                obj->sig[j].last_level = obj->sig[j].this_level;
            }

            key_press_count_stats(&obj->sig[j]);

#if (KEY_LONG_SUPPORT == KEY_ENABLE)
            key_check_long(&obj->sig[j]);
#endif

#if (KEY_MULTI_SUPPORT == KEY_ENABLE)
            key_check_multi(&obj->sig[j]);
#endif

#if (KEY_CONTINUOUS_SUPPORT == KEY_ENABLE)
            key_check_continuous(&obj->sig[j]);
#endif

#if (KEY_COMBINE_SUPPORT == KEY_ENABLE)
            key_check_combine(&obj->sig[j]);
#endif
        }
    }
}

void key_board_debug_register(print_debug_callback print_debug)
{
    internal_print_debug = print_debug;
}
