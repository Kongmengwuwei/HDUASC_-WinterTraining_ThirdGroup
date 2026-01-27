#include "menu.h"
#include "flash.h"

uint8_t menu_cursor = 0;												//光标位置
float pidnum[5][3] = {0};												//pid参数数组
uint8_t carmod = 0,mod_selected = 0,pid_edit = 0;						//发车模式，模式已选择标志，pid编辑模式标志
static uint8_t key_handled[4] = {0};									//按键消抖
static const uint8_t pid_rows[PID_COUNT] = {ROW_KP, ROW_KI, ROW_KD};	//pid参数行位置

/*菜单初始化*/
void Menu_Init(void)													//
{
	ips200_set_dir(IPS200_PORTAIT);										//屏幕初始化
	ips200_set_font(IPS200_8X16_FONT);
	ips200_set_color(RGB565_WHITE, RGB565_BLACK);
	ips200_init(IPS200_TYPE);
	
	interrupt_global_enable(0);											//全局中断使能（也许可以放到main里
	
	ips200_show_string(0,0,"-->");										//初始显示
	ips200_show_string(112,ROW_MODE,"Mod Selecting");
	ips200_show_string(24,ROW_MODE,"carmod=");
	ips200_show_string(24,ROW_KP,"kp=");
	ips200_show_string(24,ROW_KI,"ki=");
	ips200_show_string(24,ROW_KD,"kd=");
	
	Flash_Download();													//flash读取pid参数（保存过的）
}

/*光标换位*/
void Menu_MoveCursor(int8_t dir)
{
	ips200_show_string(0, pid_rows[menu_cursor], "   ");				//清除
	
	if (dir == -1) {													//上下移
		if (menu_cursor > 0) {
			menu_cursor--;
		} else {
			menu_cursor = PID_COUNT - 1;
		}
	} else if (dir == 1) { 
		if (menu_cursor < PID_COUNT - 1) {
			menu_cursor++;
		} else {
			menu_cursor = 0;
		}
	}
	
	ips200_show_string(0, pid_rows[menu_cursor], "-->");				//新光标
}

/*发车模式切换*/
void CarMod_Switch(void)
{
	if(key_get_state(KEY_3) == KEY_SHORT_PRESS)							//3键确认
		{if(!key_handled[KEY_3]){
			
			mod_selected = 1;
			ips200_show_string(0, ROW_MODE, "   ");
			ips200_show_string(0, ROW_KP, "-->");
			ips200_show_string(112,ROW_MODE,"             ");
			ips200_show_string(112,ROW_MODE,"Mod Selected");
			
		key_handled[KEY_3] = 1;}}
		else key_handled[KEY_3] = 0;
	/*前后切换*/
	if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
		{if(!key_handled[KEY_1]){
			
			if(carmod > 0)
				carmod--;
			else
				carmod = MODE_COUNT - 1;
			
		key_handled[KEY_1] = 1;}}
		else key_handled[KEY_1] = 0;
		
	if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
		{if(!key_handled[KEY_2]){
			
			if(carmod < MODE_COUNT - 1)
				carmod++;
			else
				carmod = 0;
		
		key_handled[KEY_2] = 1;}}
		else key_handled[KEY_2] = 0;
	
	
}

/*PID编辑*/
void PID_Edit(void)
{
	if(key_get_state(KEY_3) == KEY_SHORT_PRESS)							//3键确认
		{if(!key_handled[KEY_3]&&!pid_edit){
		
		pid_edit = 1;
		ips200_show_string(112,ROW_KP,"       ");ips200_show_string(112,ROW_KP,"Editing");
		
		key_handled[KEY_3] = 1;}}
		else key_handled[KEY_3] = 0;
		
	if(key_get_state(KEY_4) == KEY_SHORT_PRESS)							//4键退出
		{if(!key_handled[KEY_4]){if(pid_edit){
		
		pid_edit = 0;
		ips200_show_string(112,ROW_KP,"       ");ips200_show_string(112,ROW_KP,"Edited");
		Flash_Upload();
		ips200_show_string(112,ROW_KD,"Saved");
		ips200_show_string(112,ROW_KP,"       ");
		}
		
		else {mod_selected = 0;
		ips200_show_string(0, pid_rows[menu_cursor], "   ");
		ips200_show_string(0, ROW_MODE, "-->");
		ips200_show_string(112,ROW_KD,"     ");
		ips200_show_string(112,ROW_MODE,"Mod Selecting");}
		
		key_handled[KEY_4] = 1;}}
		else key_handled[KEY_4] = 0;
	/*变行或增减*/
	if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
		{if(!key_handled[KEY_1]){
			
			if(!pid_edit) Menu_MoveCursor(-1);
			else pidnum[carmod][menu_cursor]+=0.1;
			
		key_handled[KEY_1] = 1;}}
		else key_handled[KEY_1] = 0;
		
	if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
		{if(!key_handled[KEY_2]){
			
			if(!pid_edit) Menu_MoveCursor(1);
			else pidnum[carmod][menu_cursor]-=0.1;
			
		key_handled[KEY_2] = 1;}}
		else key_handled[KEY_2] = 0;
}

/*菜单更新*/
void Menu_Update(void)
{
		if(!mod_selected)
			CarMod_Switch();											//发车模式切换
		else
			PID_Edit();													//PID编辑
		
		ips200_show_uint(80,0,carmod+1,1);								//参数显示
		ips200_show_float(48,16,pidnum[carmod][0],2,1);
		ips200_show_float(48,32,pidnum[carmod][1],2,1);
		ips200_show_float(48,48,pidnum[carmod][2],2,1);
}
