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


void motor_pwm_init(void);

void right_rear_motor(MotorDirection direction, uint8_t duty_cycle);

void left_rear_motor(MotorDirection direction, uint8_t duty_cycle);

void left_front_motor(MotorDirection direction, uint8_t duty_cycle);

void right_front_motor(MotorDirection direction, uint8_t duty_cycle);

#endif
