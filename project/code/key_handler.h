#ifndef __KEY_HANDLER_H
#define __KEY_HANDLER_H

#include "zf_common_headfile.h"

typedef enum {
	KEY_EVENT_NONE = 0,
	KEY_EVENT_CLICK,
	KEY_EVENT_LONG,
	KEY_EVENT_REPEAT,
	KEY_EVENT_HOLD,
} key_event_enum;

#define KEY_REPEAT_INTERVAL    (100)					//÷ÿ∏¥º‰∏Ù

void key_handler_init (void);
void key_event_scan (void);
key_event_enum key_event_get   (key_index_enum key_n);
uint8 key_is_hold              (key_index_enum key_n);

#endif