#include "menu.h"

uint8_t menu_cursor = 0;
float num[3] = {0};
uint8_t chosen = 0;
static uint8_t key_handled[4] = {0};

void Menu_Init(void)
{
	ips200_set_dir(IPS200_PORTAIT);
	ips200_set_font(IPS200_8X16_FONT);
	ips200_set_color(RGB565_WHITE, RGB565_BLACK);
	ips200_init(IPS200_TYPE);
	interrupt_global_enable(0);
	ips200_show_string(0,0,"-->");
	ips200_show_string(24,0,"kp=");
	ips200_show_string(24,16,"ki=");
	ips200_show_string(24,32,"kd=");
	Flash_Download();
}
void Menu_MoveCursor(int8_t dir, uint8_t min_row, uint8_t max_row)
{
	uint8_t new_cursor = menu_cursor;

	if (dir == -1) { 
		if (new_cursor > min_row) {
			new_cursor-=16;
		} else {
			new_cursor = max_row;
		}
	} else if (dir == 1) { 
		if (new_cursor < max_row) {
			new_cursor+=16;
		} else {
			new_cursor = min_row;
		}
	}
	ips200_show_string(0, menu_cursor, "   ");
	ips200_show_string(0, new_cursor, "-->");
	menu_cursor = new_cursor;
}

void Menu_Update(void)
{
	if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
		{if(!key_handled[KEY_3]&&!chosen){chosen = 1;key_handled[KEY_3] = 1;}}
		else key_handled[KEY_3] = 0;
		
	if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
		{if(!key_handled[KEY_4]&&chosen){chosen = 0;Flash_Upload();key_handled[KEY_4] = 1;}}
		else key_handled[KEY_4] = 0;
		
	if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
		{if(!key_handled[KEY_1]){if(!chosen) Menu_MoveCursor(-1,min,max);
			else num[menu_cursor/16]+=0.1;
		key_handled[KEY_1] = 1;}}
		else key_handled[KEY_1] = 0;
		
	if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
		{if(!key_handled[KEY_2]){if(!chosen) Menu_MoveCursor(1,min,max);
			else num[menu_cursor/16]-=0.1;
		key_handled[KEY_2] = 1;}}
		else key_handled[KEY_2] = 0;
		
		ips200_show_float(48,0,num[0],2,1);
		ips200_show_float(48,16,num[1],2,1);
		ips200_show_float(48,32,num[2],2,1);
}
