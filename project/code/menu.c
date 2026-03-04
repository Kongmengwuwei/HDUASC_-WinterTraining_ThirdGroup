#include "menu.h"
#include "flash.h"
#include "key_handler.h"
#include "pid.h"
#include "Motor.h"
#include "Path_Recorder.h"

extern uint8 RunFlag, Mode, Recorder_Flag, Tracking_Flag;

int load_flag = 0;
Menu curr_menu = Main;
uint8_t menu_cursor = 0;
uint8_t min_row = 0, max_row = 0;
float parameter[7][3] = {0};
uint8_t pidmode = 0, pid_selected = 0, pid_edit = 0, pid_row = 0;

void Menu_Clear(void)
{
    for(int i = 0; i < 7; i++) 
        ips200_show_string(0, 16*i, "                              ");
}

void Draw_Main(void)
{
    Menu_Clear();
    menu_cursor = 16;
    ips200_show_string(0, 0, "MAIN MENU");
    ips200_show_string(0, 16, "==>PID");
    ips200_show_string(24, 32, "CarCheck");
    min_row = 16; max_row = 32;
}

void Draw_CarCheck(void)
{
    Menu_Clear();
    menu_cursor = 16;
    ips200_show_string(0, 0, "Car Check");
    ips200_show_string(0, 16, "==>Mode = ");
    ips200_show_string(24, 32, "Set");
    ips200_show_string(24, 48, "Record");
    ips200_show_string(24, 64, "Repeat");
    min_row = 16; max_row = 64;
}

void Draw_PID_Select(void)
{
    Menu_Clear();
    menu_cursor = 16;
    ips200_show_string(0, 0, "PID Select");
    ips200_show_string(0, 16, "==>Gyro");
    ips200_show_string(24, 32, "Angle");
    ips200_show_string(24, 48, "Speed");
    ips200_show_string(24, 64, "Turn");
    ips200_show_string(24, 80, "Trace");
    ips200_show_string(24, 96, "Yaw");
    min_row = 16; max_row = 96;
}

void Draw_Gyro(void)
{
    Menu_Clear(); menu_cursor = 16;
    ips200_show_string(0, 0, "Gyro Edit");
    ips200_show_string(0, 16, "==>Kp =");
    ips200_show_string(24, 32, "Ki =");
    ips200_show_string(24, 48, "Kd =");
    min_row = 16; max_row = 48;
}

void Draw_Angle(void)
{
    Menu_Clear(); menu_cursor = 16;
    ips200_show_string(0, 0, "Angle Edit");
    ips200_show_string(0, 16, "==>Kp =");
    ips200_show_string(24, 32, "Ki =");
    ips200_show_string(24, 48, "Kd =");
    min_row = 16; max_row = 48;
}

void Draw_Speed(void)
{
    Menu_Clear(); menu_cursor = 16;
    ips200_show_string(0, 0, "Speed Edit");
    ips200_show_string(0, 16, "==>Kp =");
    ips200_show_string(24, 32, "Ki =");
    ips200_show_string(24, 48, "Kd =");
    min_row = 16; max_row = 48;
}

void Draw_Turn(void)
{
    Menu_Clear(); menu_cursor = 16;
    ips200_show_string(0, 0, "Turn Edit");
    ips200_show_string(0, 16, "==>Kp =");
    ips200_show_string(24, 32, "Ki =");
    ips200_show_string(24, 48, "Kd =");
    min_row = 16; max_row = 48;
}

void Draw_Trace(void)
{
    Menu_Clear(); menu_cursor = 16;
    ips200_show_string(0, 0, "Trace Edit");
    ips200_show_string(0, 16, "==>Kp =");
    ips200_show_string(24, 32, "Ki =");
    ips200_show_string(24, 48, "Kd =");
    min_row = 16; max_row = 48;
}

void Draw_Yaw(void)
{
    Menu_Clear(); menu_cursor = 16;
    ips200_show_string(0, 0, "Yaw Edit");
    ips200_show_string(0, 16, "==>Kp =");
    ips200_show_string(24, 32, "Ki =");
    ips200_show_string(24, 48, "Kd =");
    min_row = 16; max_row = 48;
}

void Draw_PID(int mode)
{
    switch(mode) {
        case 1: Draw_Gyro(); break;
        case 2: Draw_Angle(); break;
        case 3: Draw_Speed(); break;
        case 4: Draw_Turn(); break;
        case 5: Draw_Trace(); break;
        case 6: Draw_Yaw(); break;
    }
}

void Menu_Init(void)
{
    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_font(IPS200_8X16_FONT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);
    Draw_Main();
    Flash_Download();
}

void Menu_MoveCursor(int8_t dir, uint8_t min, uint8 max)
{
    ips200_show_string(0, menu_cursor, "   ");
    if(dir == -1) {
        if(menu_cursor > min) menu_cursor -= ROW_LENGTH;
        else menu_cursor = max;
    } else if(dir == 1) {
        if(menu_cursor < max) menu_cursor += ROW_LENGTH;
        else menu_cursor = min;
    }
    ips200_show_string(0, menu_cursor, "==>");
}

void Menu_Update(void)
{
    key_event_enum k1 = key_event_get(KEY_1);
    key_event_enum k2 = key_event_get(KEY_2);
    key_event_enum k3 = key_event_get(KEY_3);
    key_event_enum k4 = key_event_get(KEY_4);
    
    if(curr_menu == Main)
    {
        if(k1 == KEY_EVENT_CLICK) Menu_MoveCursor(-1, min_row, max_row);
        if(k2 == KEY_EVENT_CLICK) Menu_MoveCursor(1, min_row, max_row);
        if(k3 == KEY_EVENT_CLICK)
        {
            if(menu_cursor == 16) { curr_menu = PID; Draw_PID_Select(); }
            else if(menu_cursor == 32) { curr_menu = CarCheck; Draw_CarCheck(); }
        }
    }
    else if(curr_menu == PID)
    {
        if(k1 == KEY_EVENT_CLICK) Menu_MoveCursor(-1, min_row, max_row);
        if(k2 == KEY_EVENT_CLICK) Menu_MoveCursor(1, min_row, max_row);
        if(k3 == KEY_EVENT_CLICK) { curr_menu = Edit; pidmode = menu_cursor / 16; Draw_PID(pidmode); }
        if(k4 == KEY_EVENT_CLICK) { curr_menu = Main; Draw_Main(); }
    }
    else if(curr_menu == CarCheck)
    {
        static uint8 flag = 0;
        ips200_show_int(80, 16, Mode, 1);
        
        if(!flag)
        {
            if(k1 == KEY_EVENT_CLICK) Menu_MoveCursor(-1, min_row, max_row);
            if(k2 == KEY_EVENT_CLICK) Menu_MoveCursor(1, min_row, max_row);
            if(k3 == KEY_EVENT_CLICK) { ips200_show_string(176, 0, "Locked"); flag = 1; }
            if(k4 == KEY_EVENT_CLICK) { curr_menu = Main; Draw_Main(); }
        }
        else
        {
            // Mode选择
            if(menu_cursor == 16)
            {
                if(k1 == KEY_EVENT_CLICK) { if(Mode > 1) Mode--; else Mode = 5; }
                if(k2 == KEY_EVENT_CLICK) { if(Mode < 5) Mode++; else Mode = 1; }
                if(k4 == KEY_EVENT_CLICK) { Flash_Upload(); ips200_show_string(176, 0, "      "); flag = 0; }
            }
            // Set发车
            else if(menu_cursor == 32)
            {
                if(k3 == KEY_EVENT_CLICK)
                {
                    RunFlag = 1;
                    PID_Init(&GyroPID);
                    PID_Init(&AnglePID);
                    PID_Init(&SpeedPID);
                    PID_Init(&TurnPID);
                    PID_Init(&TracePID);
                    PID_Init(&YAWPID);
                    ips200_show_string(64, 32, "GO    ");
                }
                if(k4 == KEY_EVENT_CLICK) { ips200_show_string(176, 0, "      "); ips200_show_string(64, 32, "      "); flag = 0; }
            }
            // Record记录
            else if(menu_cursor == 48)
            {
                ips200_show_uint(128, 48, path_manager.count, 4);
                
                if(Recorder_Flag)
                {
                    ips200_show_string(80, 48, "Rec...");
                    
                    if(path_manager.count >= MAX_PATH_POINTS)
                    {
                        Recorder_Flag = 0;
                        ips200_show_string(80, 48, "Full! ");
                        system_delay_ms(500);
                        ips200_show_string(80, 48, "Save..");
                        Path_SaveToFlash();
                        ips200_show_string(80, 48, "Saved!");
                    }
                    
                    if(k4 == KEY_EVENT_CLICK)
                    {
                        Recorder_Flag = 0;
                        ips200_show_string(80, 48, "Save..");
                        Path_SaveToFlash();
                        ips200_show_string(80, 48, "Saved!");
                    }
                }
                else
                {
                    if(k3 == KEY_EVENT_CLICK)
                    {
                        PathTracking_Start();
                        Recorder_Flag = 1;
                        Tracking_Flag = 0;
                        RunFlag = 0;
                        ips200_show_string(80, 48, "Start ");
                    }
                    if(k4 == KEY_EVENT_CLICK)
                    {
                        ips200_show_string(176, 0, "      ");
                        ips200_show_string(80, 48, "      ");
                        ips200_show_string(128, 48, "    ");
                        flag = 0;
                    }
                }
            }
            // Repeat回放
            else if(menu_cursor == 64)
            {
                ips200_show_uint(128, 64, path_manager.current_index, 4);
                ips200_show_string(160, 64, "/");
                ips200_show_uint(168, 64, path_manager.count, 4);
                
                if(Tracking_Flag)
                {
                    ips200_show_string(80, 64, "Play..");
                    
                    if(path_manager.current_index >= path_manager.count)
                    {
                        Tracking_Flag = 0;
                        RunFlag = 1;
                        ips200_show_string(80, 64, "Done! ");
                    }
                    
                    if(k4 == KEY_EVENT_CLICK)
                    {
                        Tracking_Flag = 0;
                        RunFlag = 0;
                        Navigation_Speed = 0;
                        Navigation_Turn = 0;
                        ips200_show_string(80, 64, "Stop  ");
                    }
                }
                else
                {
                    if(k3 == KEY_EVENT_CLICK)
                    {
                        if(path_manager.count == 0)
                        {
                            ips200_show_string(80, 64, "Load..");
                            if(Path_LoadFromFlash())
                            {
                                load_flag = 1;
                                ips200_show_string(80, 64, "OK    ");
                                system_delay_ms(300);
                            }
                            else
                            {
                                ips200_show_string(80, 64, "NoData");
                                load_flag = 0;
                            }
                        }
                        else load_flag = 1;
                        
                        if(load_flag && path_manager.count > 0)
                        {
                            PathTracking_Start();    // 计算yaw偏移
                            Tracking_Flag = 1;
                            Recorder_Flag = 0;
                            RunFlag = 1;
                            
                            PID_Init(&GyroPID);
                            PID_Init(&AnglePID);
                            PID_Init(&SpeedPID);
                            PID_Init(&TurnPID);
                            PID_Init(&TracePID);
                            PID_Init(&YAWPID);
                            
                            ips200_show_string(80, 64, "Replay");
                        }
                    }
                    if(k4 == KEY_EVENT_CLICK)
                    {
                        load_flag = 0;
                        ips200_show_string(176, 0, "      ");
                        ips200_show_string(80, 64, "         ");
                        ips200_show_string(128, 64, "         ");
                        flag = 0;
                    }
                }
            }
        }
        parameter[Carmode][0] = Mode;
    }
    else if(curr_menu == Edit)
    {
        static uint8 flag = 0;
        ips200_show_float(64, 16, parameter[pidmode][0], 4, 2);
        ips200_show_float(64, 32, parameter[pidmode][1], 4, 2);
        ips200_show_float(64, 48, parameter[pidmode][2], 4, 2);
        
        if(!flag)
        {
            if(k1 == KEY_EVENT_CLICK) Menu_MoveCursor(-1, min_row, max_row);
            if(k2 == KEY_EVENT_CLICK) Menu_MoveCursor(1, min_row, max_row);
            if(k3 == KEY_EVENT_CLICK) { pid_row = (menu_cursor / 16) - 1; ips200_show_string(176, 0, "Locked"); flag = 1; }
            if(k4 == KEY_EVENT_CLICK) { curr_menu = PID; Draw_PID_Select(); }
        }
        else
        {
            if(k1 == KEY_EVENT_CLICK) parameter[pidmode][pid_row] += 0.1;
            if(k1 == KEY_EVENT_REPEAT) parameter[pidmode][pid_row] += 1;
            if(k2 == KEY_EVENT_CLICK) parameter[pidmode][pid_row] -= 0.1;
            if(k2 == KEY_EVENT_REPEAT) parameter[pidmode][pid_row] -= 1;
            if(k4 == KEY_EVENT_CLICK) { Flash_Upload(); ips200_show_string(176, 0, "      "); flag = 0; }
        }
    }
}