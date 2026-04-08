#ifndef __TRACK_H
#define __TRACK_H
#include "LPC11xx.h"


// 循迹状态位域结构体，对应5路循迹传感器
typedef struct
{
    uint8_t bit1 : 1;  // 对应XJ1
    uint8_t bit2 : 1;  // 对应XJ2
    uint8_t bit3 : 1;  // 对应XJ3（中间）
    uint8_t bit4 : 1;  // 对应XJ4
    uint8_t bit5 : 1;  // 对应XJ5
} track_flags;

// 循迹状态联合体（位域与整体状态互转）
typedef union
{
    track_flags track;
    uint8_t state;  // 5路状态组合（0~31）
}_track_state;



void track_init(void);
void track_check(void);

#endif