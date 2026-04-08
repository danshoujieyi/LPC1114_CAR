#ifndef __PID_H
#define __PID_H
#include "LPC11xx.h"

// PID参数宏定义（需根据实际调试调整）
#define KP  1.1f    // 比例系数（主要控制响应速度）
#define KI  0.19f    // 积分系数（消除静态误差，避免过大）
#define KD  0.0005f    // 微分系数（抑制超调，增强稳定性）

// PID输出限制（对应转向幅度范围，与之前的-4~4等级对应）
#define PID_OUTPUT_MIN  -70.0f
#define PID_OUTPUT_MAX   70.0f

// 积分项限制（防止积分饱和）
#define INTEGRAL_MIN    -15.0f
#define INTEGRAL_MAX     15.00

// PID状态结构体（保存历史信息）
typedef struct {
    float error;          // 当前误差（输入：track_status[0]）
    float last_error;     // 上一次误差（用于计算微分）
    float integral;       // 积分项累积
    float derivative;     // 微分项
    float output;         // PID输出（转向控制量）
} PID_HandleTypeDef;


void pid_init(PID_HandleTypeDef *pid);
void pid_calculate(PID_HandleTypeDef *pid, float current_error);

#endif