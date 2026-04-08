#ifndef __MOTOR_H
#define __MOTOR_H

#include "LPC11xx.h"
#include "driver_config.h"
#include "timer16.h"
#include "timer32.h"
// 电机方向定义
typedef enum {
    MOTOR_STOP = 0,   // 停止
    MOTOR_FORWARD,    // 正转
    MOTOR_BACKWARD    // 反转
} MotorDirection;



// 初始化电机PWM（需在main中调用）
void motor_pwm_init(void);

void left_motor(int8_t duty_cycle);  // 参数改为int8_t以支持正负值

void right_motor(int8_t duty_cycle);  // 参数改为int8_t以支持正负值
	

// 控制右后轮（定时器16B1）
void right_rear_motor(MotorDirection direction, uint8_t duty_cycle);

// 控制左后轮（定时器16B0）
void left_rear_motor(MotorDirection direction, uint8_t duty_cycle);


/**
 * 左前轮控制逻辑（CT32B1，10kHz PWM）
*/
void left_front_motor(MotorDirection direction, uint8_t duty_cycle);


/**
 * 右前轮控制逻辑（CT32B0，10kHz PWM）
 * direction: 方向（正转/反转/停止）
 * duty_cycle: 占空比（0-100）
 */
void right_front_motor(MotorDirection direction, uint8_t duty_cycle);

#endif
