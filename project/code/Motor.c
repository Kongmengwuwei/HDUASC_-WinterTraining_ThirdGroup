#include <Motor.h>

void Motor_Init()
{
		gpio_init(MOTOR1_1, GPO, GPIO_HIGH, GPO_PUSH_PULL);               // GPIO 初始化为输出 默认上拉输出高
		gpio_init(MOTOR1_2, GPO, GPIO_HIGH, GPO_PUSH_PULL);              
		pwm_init(MOTOR1_PWM, 17000, 0);                                   // PWM 通道初始化频率 17KHz 占空比初始为 0
	
		gpio_init(MOTOR2_1, GPO, GPIO_HIGH, GPO_PUSH_PULL);               // GPIO 初始化为输出 默认上拉输出高
		gpio_init(MOTOR2_2, GPO, GPIO_HIGH, GPO_PUSH_PULL);
		pwm_init(MOTOR2_PWM, 17000, 0);                                   	// PWM 通道初始化频率 17KHz 占空比初始为 0
	
		encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);             // 初始化编码器模块与引脚 正交解码编码器模式
		encoder_quad_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);             // 初始化编码器模块与引脚 正交解码编码器模式
}

void Set_Moter1(int16 pwm1)							//设置1号电机（？轮）
{
	if(pwm1>=0)
	{
		gpio_set_level(MOTOR1_1, GPIO_HIGH);
		gpio_set_level(MOTOR1_2, GPIO_LOW);
		pwm_set_duty(MOTOR1_PWM, pwm1 * (PWM_DUTY_MAX / 100));
	}else
	{
		gpio_set_level(MOTOR1_1, GPIO_LOW);
		gpio_set_level(MOTOR1_2, GPIO_HIGH);
		pwm_set_duty(MOTOR1_PWM, pwm1 * (PWM_DUTY_MAX / 100));	
	}
}

void Set_Moter2(int16 pwm2)							//设置2号电机（？轮）
{
	if(pwm2>=0)
	{
		gpio_set_level(MOTOR2_1, GPIO_HIGH);
		gpio_set_level(MOTOR2_2, GPIO_LOW);
		pwm_set_duty(MOTOR1_PWM, pwm2 * (PWM_DUTY_MAX / 100));
	}else
	{
		gpio_set_level(MOTOR2_1, GPIO_LOW);
		gpio_set_level(MOTOR2_2, GPIO_HIGH);
		pwm_set_duty(MOTOR1_PWM, pwm2 * (PWM_DUTY_MAX / 100));	
	}
}
