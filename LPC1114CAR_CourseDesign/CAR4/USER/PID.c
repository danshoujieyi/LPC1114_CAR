#include "PID.h"


// 初始化PID（调用一次即可，建议系统启动时初始化）
void pid_init(PID_HandleTypeDef *pid) {
    pid->error = 0.0f;
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->derivative = 0.0f;
    pid->output = 0.0f;
}


// 计算PID输出
// 参数：pid句柄，当前误差（这里即track_status[0]）
void pid_calculate(PID_HandleTypeDef *pid, float current_error) {
    // 1. 保存当前误差
    pid->error = current_error;

    // 2. 计算积分项（累积误差，同时限制积分范围防止饱和）
    pid->integral += pid->error;  // 简化版（实际应乘以采样时间Ts，这里假设Ts固定并融入KI）
    // 积分限幅
    if (pid->integral > INTEGRAL_MAX) {
        pid->integral = INTEGRAL_MAX;
    } else if (pid->integral < INTEGRAL_MIN) {
        pid->integral = INTEGRAL_MIN;
    }

    // 3. 计算微分项（当前误差 - 上一次误差，反映误差变化率）
    pid->derivative = pid->error - pid->last_error;

    // 4. 计算PID总输出（P+I+D）
    pid->output = KP * pid->error + KI * pid->integral + KD * pid->derivative;

    // 5. 输出限幅（确保转向量在安全范围）
    if (pid->output > PID_OUTPUT_MAX) {
        pid->output = PID_OUTPUT_MAX;
    } else if (pid->output < PID_OUTPUT_MIN) {
        pid->output = PID_OUTPUT_MIN;
    }

    // 6. 保存当前误差，用于下一次计算微分
    pid->last_error = pid->error;
}