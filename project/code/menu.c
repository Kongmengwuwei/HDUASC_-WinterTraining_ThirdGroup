#include "menu.h"
#include "flash.h"
#include "key_handler.h"
#include "pid.h"
#include "Motor.h"

Menu curr_menu = Main;													//初始菜单
uint8_t menu_cursor = 0;												//光标位置
uint8_t min_row = 0,max_row = 0;										//上下行限
float parameter[5][3] = {0};											//参数数组

uint8_t pidmode = 0,pid_selected = 0, pid_edit = 0, pid_row = 0;		//PID选择

void Menu_Clear (void)
{
	for(int i = 0;i < 6;i++) ips200_show_string(0,16*i,"                             ");
}

void Draw_Main (void)
{
	Menu_Clear();menu_cursor = 16;
	ips200_show_string(0,0,"MAIN MENU");
	ips200_show_string(0,16,"==>PID");
	ips200_show_string(24,32,"CarCheck");
	min_row = 16;max_row = 32;
}

void Draw_CarCheck (void)
{
	Menu_Clear();menu_cursor = 16;
	ips200_show_string(0,0,"Car Check");
	ips200_show_string(0,16,"==>Mode = ");
	ips200_show_string(24,32,"Set");
	min_row = 16;max_row = 32;
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
	
	Draw_Main();
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

/*菜单更新*/
void Menu_Update(void)
{
	key_event_enum k1 = key_event_get(KEY_1);
	key_event_enum k2 = key_event_get(KEY_2);
	key_event_enum k3 = key_event_get(KEY_3);
	key_event_enum k4 = key_event_get(KEY_4);
	
	if(curr_menu == Main)
	{
		if(k1 == KEY_EVENT_CLICK)Menu_MoveCursor(-1,min_row,max_row);
		if(k2 == KEY_EVENT_CLICK)Menu_MoveCursor(1,min_row,max_row);
		if(k3 == KEY_EVENT_CLICK)
		{
			if(menu_cursor == 16){curr_menu = PID;Draw_PID_Select();}
			else if(menu_cursor == 32){curr_menu = CarCheck;Draw_CarCheck();}
		}
	}
	else if(curr_menu == PID)
	{
		if(k1 == KEY_EVENT_CLICK)Menu_MoveCursor(-1,min_row,max_row);
		if(k2 == KEY_EVENT_CLICK)Menu_MoveCursor(1,min_row,max_row);
		if(k3 == KEY_EVENT_CLICK){curr_menu = Edit;pidmode = menu_cursor/16;Draw_PID(pidmode);}
		if(k4 == KEY_EVENT_CLICK){curr_menu = Main;Draw_Main();}
	}
	else if(curr_menu == CarCheck)
	{
		static uint8 flag = 0,carmode = 1;
		ips200_show_int(80,16,carmode,1);
		if(!flag)
		{
			if(k1 == KEY_EVENT_CLICK)Menu_MoveCursor(-1,min_row,max_row);
			if(k2 == KEY_EVENT_CLICK)Menu_MoveCursor(1,min_row,max_row);
			if(k3 == KEY_EVENT_CLICK){ips200_show_string(176,0,"Locked");flag = 1;}
			if(k4 == KEY_EVENT_CLICK){curr_menu = Main;Draw_Main();}
		}
		else
		{
			if(menu_cursor == 16)
			{
				if(k1 == KEY_EVENT_CLICK){if(carmode < 5) carmode++; else carmode = 1;}
				if(k2 == KEY_EVENT_CLICK){if(carmode > 1) carmode--; else carmode = 5;}
				if(k4 == KEY_EVENT_CLICK){Flash_Upload();ips200_show_string(176,0,"      ");flag = 0;}
			}
			else if(menu_cursor == 32)
			{
				if(k3 == KEY_EVENT_CLICK){RunFlag = 1;ips200_show_string(64,32,"GO    ");ips200_show_string(64,32,"  Over");}
				if(k4 == KEY_EVENT_CLICK){ips200_show_string(176,0,"      ");flag = 0;}
			}
		}
		parameter[Carmode][0] = carmode;
	}
	else if(curr_menu == Edit)
	{
		static uint8 flag = 0;
		ips200_show_float(64,16,parameter[pidmode][0],2,2);
		ips200_show_float(64,32,parameter[pidmode][1],2,2);
		ips200_show_float(64,48,parameter[pidmode][2],2,2);
		if(!flag)
		{
			if(k1 == KEY_EVENT_CLICK)Menu_MoveCursor(-1,min_row,max_row);
			if(k2 == KEY_EVENT_CLICK)Menu_MoveCursor(1,min_row,max_row);
			if(k3 == KEY_EVENT_CLICK){pid_row = (menu_cursor/16) - 1;ips200_show_string(176,0,"Locked");flag = 1;}
			if(k4 == KEY_EVENT_CLICK){curr_menu = PID;Draw_PID_Select();}
		}
		else
		{
			if(k1 == KEY_EVENT_CLICK)parameter[pidmode][pid_row]+=0.01;
			if(k1 == KEY_EVENT_REPEAT)parameter[pidmode][pid_row]+=0.1;
			if(k2 == KEY_EVENT_CLICK)parameter[pidmode][pid_row]-=0.01;
			if(k2 == KEY_EVENT_REPEAT)parameter[pidmode][pid_row]-=0.1;
			if(k4 == KEY_EVENT_CLICK){Flash_Upload();ips200_show_string(176,0,"      ");flag = 0;}
		}
	}
}
