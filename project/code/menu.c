#include "menu.h"
#include "flash.h"
#include "key_handler.h"

Menu curr_menu = Carmode;												//初始菜单
uint8_t menu_cursor = 0;												//光标位置
uint8_t min_row = 0,max_row = 0;										//上下行限

float pidnum[5][4][3] = {0};											//PID参数数组
uint8_t carmode = 0,mode_selected = 0;									//发车模式选择
uint8_t pidmode = 0,pid_selected = 0, pid_edit = 0, pid_row = 0;		//PID选择

void Menu_Clear (void)
{
	for(int i = 0;i < 6;i++) ips200_show_string(0,16*i,"                             ");
}

void Draw_Carmode (void)
{
	Menu_Clear();menu_cursor = 16;
	ips200_show_string(0,0,"Car Mode Select");
	ips200_show_string(0,16,"==>Mode 1");
	ips200_show_string(24,32,"Mode 2");
	ips200_show_string(24,48,"Mode 3");
	ips200_show_string(24,64,"Mode 4");
	ips200_show_string(24,80,"Mode 5");
	min_row = 16;max_row = 80;
}

void Draw_PID_Select (void)
{
	Menu_Clear();menu_cursor = 16;
	ips200_show_string(0,0,"PID Select");
	ips200_show_string(0,16,"==>Angle");
	ips200_show_string(24,32,"Speed");
	ips200_show_string(24,48,"Turn");
	ips200_show_string(24,64,"Position");
	min_row = 16;max_row = 64;
}

void Draw_Angle (void)
{
	Menu_Clear();menu_cursor = 16;
	ips200_show_string(0,0,"Angle Edit");
	ips200_show_string(0,16,"==>Kp =");
	ips200_show_string(24,32,"Ki =");
	ips200_show_string(24,48,"Kd =");
	min_row = 16;max_row = 48;
}

void Draw_Speed (void)
{
	Menu_Clear();menu_cursor = 16;
	ips200_show_string(0,0,"Speed Edit");
	ips200_show_string(0,16,"==>Kp =");
	ips200_show_string(24,32,"Ki =");
	ips200_show_string(24,48,"Kd =");
	min_row = 16;max_row = 48;
}

void Draw_Turn (void)
{
	Menu_Clear();menu_cursor = 16;
	ips200_show_string(0,0,"Turn Edit");
	ips200_show_string(0,16,"==>Kp =");
	ips200_show_string(24,32,"Ki =");
	ips200_show_string(24,48,"Kd =");
	min_row = 16;max_row = 48;
}

void Draw_Position (void)
{
	Menu_Clear();menu_cursor = 16;
	ips200_show_string(0,0,"Position Edit");
	ips200_show_string(0,16,"==>Kp =");
	ips200_show_string(24,32,"Ki =");
	ips200_show_string(24,48,"Kd =");
	min_row = 16;max_row = 48;
}

void Draw_PID (int mode)
{
	switch (mode){
		case 1: Draw_Angle(); break;
		case 2: Draw_Speed(); break;
		case 3: Draw_Turn(); break;
		default: Draw_Position();
	}
}

/*菜单初始化*/
void Menu_Init(void)													//
{
	ips200_set_dir(IPS200_PORTAIT);										//屏幕初始化
	ips200_set_font(IPS200_8X16_FONT);
	ips200_set_color(RGB565_WHITE, RGB565_BLACK);
	ips200_init(IPS200_TYPE);
	
	Draw_Carmode();
	Flash_Download();
}

/*光标换位*/
void Menu_MoveCursor(int8_t dir, uint8_t min, uint8 max)
{
	ips200_show_string(0, menu_cursor, "   ");					//清除
	if (dir == -1) {														//上下移
		if (menu_cursor > min) {
			menu_cursor -= ROW_LENGTH;
		} else {
			menu_cursor = max;
		}
	} else if (dir == 1) { 
		if (menu_cursor < max) {
			menu_cursor += ROW_LENGTH;
		} else {
			menu_cursor = min;
		}
	}
	ips200_show_string(0, menu_cursor, "==>");					//新光标
}

void PID_Show (void)
{
	ips200_show_float(64,16,pidnum[carmode][pidmode][0],3,1);
	ips200_show_float(64,32,pidnum[carmode][pidmode][1],3,1);
	ips200_show_float(64,48,pidnum[carmode][pidmode][2],3,1);
}

/*菜单更新*/
void Menu_Update(void)
{
	key_event_enum k1 = key_event_get(KEY_1);
	key_event_enum k2 = key_event_get(KEY_2);
	key_event_enum k3 = key_event_get(KEY_3);
	key_event_enum k4 = key_event_get(KEY_4);
	
	if(!mode_selected)
	{
		if(k1 == KEY_EVENT_CLICK)Menu_MoveCursor(-1,min_row,max_row);
		if(k2 == KEY_EVENT_CLICK)Menu_MoveCursor(1,min_row,max_row);
		
		if(k3 == KEY_EVENT_CLICK)
		{
			mode_selected = 1;
			carmode = menu_cursor/16;Draw_PID_Select();
		}
	}
	else if(!pid_selected)
	{
		if(k1 == KEY_EVENT_CLICK)Menu_MoveCursor(-1,min_row,max_row);
		if(k2 == KEY_EVENT_CLICK)Menu_MoveCursor(1,min_row,max_row);
		
		if(k3 == KEY_EVENT_CLICK)
		{
			pid_selected = 1;
			pidmode = menu_cursor/16;Draw_PID(pidmode);
		}
		if(k4 == KEY_EVENT_CLICK)
		{
			mode_selected = 0;
			Draw_Carmode();
		}
	}
	else if(!pid_edit)
	{
		PID_Show();

		if(k1 == KEY_EVENT_CLICK)Menu_MoveCursor(-1,min_row,max_row);
		if(k2 == KEY_EVENT_CLICK)Menu_MoveCursor(1,min_row,max_row);
		
		if(k3 == KEY_EVENT_CLICK)
		{
			pid_edit = 1;
			pid_row = (menu_cursor/16) - 1;
			ips200_show_string(176,0,"Locked");
		}
		
		if(k4 == KEY_EVENT_CLICK)
		{
			pid_selected = 0;Draw_PID_Select();
		}
	}
	else
	{
		PID_Show();
		
		if(k1 == KEY_EVENT_CLICK)pidnum[carmode][pidmode][pid_row]+=0.1;
		if(k1 == KEY_EVENT_REPEAT)pidnum[carmode][pidmode][pid_row]+=0.5;
		if(k2 == KEY_EVENT_CLICK)pidnum[carmode][pidmode][pid_row]-=0.1;
		if(k2 == KEY_EVENT_REPEAT)pidnum[carmode][pidmode][pid_row]-=0.5;
		
		if(k4 == KEY_EVENT_CLICK)
		{
			Flash_Upload();ips200_show_string(176,0,"      ");ips200_show_string(0,80,"SAVED");
			pid_edit = 0;system_delay_ms(250);ips200_show_string(0,80,"     ");
		}
	}
}
