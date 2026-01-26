#include "menu.h"

uint8_t menu_cursor = 0;
int16_t num[3] = {0};
uint8_t chosen = 0;
static uint8_t key1_handled = 0;
static uint8_t key2_handled = 0;
static uint8_t key3_handled = 0;
static uint8_t key4_handled = 0;
void Menu_Init(void)
{
	ips200_set_dir(IPS200_PORTAIT);
    ips200_set_font(IPS200_8X16_FONT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);
    interrupt_global_enable(0);
	ips200_show_string(0,0,"-->");
	ips200_show_string(24,0,"selection1=");ips200_show_int(112,0,num[0],3);
	ips200_show_string(24,16,"selection2=");ips200_show_int(112,16,num[1],3);
	ips200_show_string(24,32,"selection3=");ips200_show_int(112,32,num[2],3);
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
		{if(!key3_handled){
			chosen = 1;key3_handled = 1;}}
		else key3_handled = 0;
	if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
		{if(!key4_handled){
			chosen = 0;key4_handled = 1;}}
		else key4_handled = 0;
	if(!chosen){
	if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
		{if(!key1_handled){
			Menu_MoveCursor(-1,0,32);key1_handled = 1;}}
		else key1_handled = 0;
	if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
		{if(!key2_handled){
			Menu_MoveCursor(1,0,32);key2_handled = 1;}}
		else key2_handled = 0;}
	else{
	if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
		{if(!key1_handled){
			num[menu_cursor/16]++;key1_handled = 1;}}
		else key1_handled = 0;
	if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
		{if(!key2_handled){
		num[menu_cursor/16]--;key2_handled = 1;}}
		else key2_handled = 0;}
	ips200_show_int(112,0,num[0],3);
	ips200_show_int(112,16,num[1],3);
	ips200_show_int(112,32,num[2],3);
}