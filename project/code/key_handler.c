#include "key_handler.h"

static key_event_enum key_event[4];				//按键事件
static key_state_enum key_prev_state[4];		//上个状态
static uint8 key_long_triggered[4];				//长按标识
static uint16 key_repeat_timer[4];				//重复计时
static uint32 scan_period = 10;					//扫描周期

void key_handler_init(void)					//按键处理初始化
{
	uint8 i;
	for (i = 0; i < 4; i++) {
		key_event[i] = KEY_EVENT_NONE;
		key_prev_state[i] = KEY_RELEASE;
		key_long_triggered[i] = 0;
		key_repeat_timer[i] = 0;
	}
}

void key_event_scan(void)						//按键时间扫描
{
	uint8 i;
	key_state_enum curr_state;
	
	key_scanner();
	
	for (i = 0; i < 4; i++) {
		curr_state = key_get_state(i);
		
		switch (curr_state) {
			case KEY_RELEASE:								//不按
				key_long_triggered[i] = 0;
				key_repeat_timer[i] = 0;
				break;

			case KEY_SHORT_PRESS:							//短按
				key_event[i] = KEY_EVENT_CLICK;
				key_clear_state(i);
				break;

			case KEY_LONG_PRESS:							//长按
				if (!key_long_triggered[i]) {
					key_event[i] = KEY_EVENT_LONG;
					key_long_triggered[i] = 1;
					key_repeat_timer[i] = 0;
				} else {									//已经长按
					key_repeat_timer[i] += scan_period;
					if (key_repeat_timer[i] >= KEY_REPEAT_INTERVAL) {
						key_repeat_timer[i] = 0;
						key_event[i] = KEY_EVENT_REPEAT;
					}
				}
				break;
		}
		key_prev_state[i] = curr_state;
	}
}

key_event_enum key_event_get(key_index_enum key_n)
{
	key_event_enum event = key_event[key_n];
	key_event[key_n] = KEY_EVENT_NONE;
	return event;
}

uint8 key_is_hold(key_index_enum key_n)
{
	key_state_enum state = key_get_state(key_n);
	return (state == KEY_LONG_PRESS) ? 1 : 0;
}
