#include "FollowRoute.h"
#include "pid.h"
#include "mpu6050.h"
#include "sensor.h"
#include "SoundLight.h"
#include "menu.h"

uint8 stat1 = 0, stat2 = 0, stat3 = 0, stat4 = 0;

void Follow_Route(void)
{
    

    stat1 = gpio_get_level(SenSor1);
	stat2 = gpio_get_level(SenSor2);
	stat3 = gpio_get_level(SenSor3);
	stat4 = gpio_get_level(SenSor4);

    /* 模式二 */
    if(mode == 2)
    {
        flag = 1;
        static int cnt2 = 0;
        /* B点判定 */
        if(flag == 1)
        {
            if(stat1 || stat2 || stat3 || stat4)        //任一点检测到黑线
            {
                cnt2++;
                if(cnt2 > 5)             //检测到黑线持续5次
                {
                    SoundLight_On();              //鸣笛并闪灯
                    flag = 2;               //进入状态二
                    cnt2 = 0;
                }
            }
            else   cnt2 = 0;
        }
        /* C点判定 */
        else if(flag == 2)
        {
            if(!(stat1 || stat2 || stat3 || stat4))                  //所有点检测到白线
            {
                cnt2 ++;
                if(cnt2 > 5)               //检测到白线持续5次
                {
                    SoundLight_On();              //鸣笛并闪灯
                    flag = 3;
                    cnt2 = 0;
                }
            }
            else   cnt2 = 0;
        }
        /* D点判定 */
        else if(flag == 3)
        {
            if(stat1 || stat2 || stat3 || stat4)        //任一点检测到黑线
            {
                cnt2++;
                if(cnt2 > 5)                //检测到黑线持续Black_CNT2次
                {
                    SoundLight_On();              //鸣笛并闪灯 
                    flag = 4;
                    cnt2 = 0;
                }
            }
            else   cnt2 = 0;
        }
        /* A点（终点）判定 */
        else if(flag == 4)
        {
            if(!(stat1 || stat2 || stat3 || stat4))                  //所有点检测到白线
            {
                cnt2++;
                if(cnt2 > 5 )                  //检测到白线持续White_CNT4次
                {
                    SoundLight_On();              //鸣笛并闪灯
                    flag = 0;           //一圈结束
                    cnt2 = 0;
                }
            }
            else   cnt2 = 0;
        }
    }
 




    /* 模式三 */
    else if(mode == 3)
    {
        flag = 1;
        for (int i = 0; i < 4; i++)        //循环绕四圈
        {
            static int cnt3 = 0;
            /* C点判定 */
            if(flag == 1)
            {
                if(stat1 || stat2 || stat3 || stat4)        //任一点检测到黑线
                {
                    cnt3++;
                    if(cnt3 > 5)               //检测到黑线持续5次
                    {
                        SoundLight_On();              //鸣笛并闪灯
                        flag = 2;               //进入下一状态
                        cnt3 = 0;
                        return 2;
                    }
                }
                else   cnt3 = 0;
            }
        
            /* B点判定 */
            else if(flag == 2)
            {
                if(!(stat1 || stat2 || stat3 || stat4))              //所有点检测到白线
                {
                    cnt3++;
                    if(cnt3 > 5)                             //检测到白线持续5次
                    {
                        SoundLight_On();              //鸣笛并闪灯
                        flag = 3;
                        cnt3 = 0;
                        return 3;
                    }
                }
            }
        
            /* D点判定 */
            else if(flag == 3)
            {
                if(stat1 || stat2 || stat3 || stat4)        //任一点检测到黑线
                {
                    cnt3++;
                    if(cnt3 > 5)                      //检测到黑线持续5次
                    {
                        SoundLight_On();              //鸣笛并闪灯
                        flag = 4;
                        cnt3 = 0;
                        return 4;
                    }
                }
            }
        
            /* A点判定 */
            else if(flag == 4)
            {
                if(!(stat1 || stat2 || stat3 || stat4))             //所有点检测到白线
                {
                    cnt3++;
                    if(cnt3 > 5)                      //检测到白线持续5次
                    {
                        SoundLight_On();              //鸣笛并闪灯
                        flag = 0;                     //停止
                        cnt3 = 0;
                        return 0;
                    }
                }
            }
        }

    }
 

}
 
 
 



void Control(void)
{

	//姿态解算
	Calculate_Attitude();


    float TargetA, TargetB;                         //目标速度
    float bias1, bias2, bias;                       //巡线、陀螺仪偏差
    /* 模式二、三 */
    if(mode == 2 || mode == 3)
    {
        if(flag == 1)
        {
            SpeedPID.Target = 30;
            TurnPID.Target = 0;
        }

        else if(flag == 2)
        {
            SpeedPID.Target = 30;
            //循迹环PID,相当于TurnPID.Target = TracePID.Out
            Trace_Tweak();
        }

        else if(flag == 3)
        {
            SpeedPID.Target = 30;
            TurnPID.Target = 0;       
        }
        else if(flag == 4)
        {
            SpeedPID.Target = 30;
            //循迹环PID,相当于TurnPID.Target = TracePID.Out
            Trace_Tweak();
        }
    }



    //速度环PID
	Speed_Tweak();

	//角度环PID
	Angle_Tweak();
	
	//角速度环PID	
	Gyro_Tweak();

    //转向环PID
	Turn_Tweak();

	//应用最终输出于电机
	if (RunFlag)
	{	
		LeftPWM = AvePWM + DifPWM / 2;
		RightPWM = AvePWM - DifPWM / 2;
		
		if (LeftPWM > 100) {LeftPWM = 100;}
		else if (LeftPWM < -100) {LeftPWM = -100;}
		if (RightPWM > 100) {RightPWM = 100;} 
		else if (RightPWM < -100) {RightPWM = -100;}
		
		Set_Motor1(LeftPWM);
		Set_Motor2(RightPWM);
	}
	else
	{
		Set_Motor1(0);
		Set_Motor2(0);
	}
}








 


