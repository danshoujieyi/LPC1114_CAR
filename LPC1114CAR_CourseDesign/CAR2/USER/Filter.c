#include "Filter.h"

float raw_filter_date = 1;
/**
 * 函数功能：中位数滤波
 * 描述：通过多次温度测量并采用中位数滤波算法，去除极端值，得到更加平稳的温度值。
 * 返回值：
 *      返回去除极端值后的温度值（单位：摄氏度）
 */
float MedianFilter(void) {
    float measurements[7]; // 用于存储7次温度测量值
    float temp;
    
    // 进行7次温度测量
    for (unsigned int i = 0; i < 7; i++) {
        measurements[i] = raw_filter_date; // 读取传感器温度值
    }

    // 冒泡排序，找出中位数
    for (unsigned int i = 0; i < 7 - 1; i++) {
        for (unsigned int j = 0; j < 7 - i - 1; j++) {
            if (measurements[j] > measurements[j + 1]) {
                // 交换
                temp = measurements[j];
                measurements[j] = measurements[j + 1];
                measurements[j + 1] = temp;
            }
        }
    }
    
    // 去掉最大值和最小值，计算中间5个值的平均值
    float sum = 0;
    for (unsigned int i = 1; i < 7 - 1; i++) { // 从1到N-2，即中间5个数
        sum += measurements[i];
    }
    
    // 计算并返回平均值
    return (sum / (7 - 2)); // 去掉2个值，所以用(N - 2)
}