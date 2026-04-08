#include "Track.h"
#include "driver_config.h"
#include "gpio.h"
#include "LED.h"




// ȫ�ֱ������壨�����.c�ļ��У�����.h����extern������
volatile float track_status[1] = {0};
volatile float track_status_backup[1][20] = {0};
volatile uint32_t track_status_worse = 0;
volatile _track_state track_state;




void track_init(void)
{
		// 2. �� PIO1_5 ��Ϊ��ͨ GPIO ����
    LPC_IOCON->PIO1_5 &= ~0x07;   // FUNC = 0 �� GPIO

    // 3. ���÷���Ϊ���
    GPIOSetDir(PORT1, 5, 1);

    // 4. ����ߵ�ƽ���� IR_PWM ʼ��Ϊ 3.3V
    GPIOSetValue(PORT1, 5, 1);
	    // ���� 5 ·ѭ���ź�Ϊ����
    GPIOSetDir(PORT2, 7, 0);   // XJ1
    GPIOSetDir(PORT2, 8, 0);   // XJ2
    GPIOSetDir(PORT2, 9, 0);   // XJ3
    GPIOSetDir(PORT2, 10, 0);   // XJ4
    GPIOSetDir(PORT2, 11, 0);   // XJ5
}


/***************************************************
������: void track_check(void)
˵��:    5·ѭ��������״̬������߼�����
         1. ��ȡ������״̬�����µ�λ��
         2. ����ѭ��ƫ��ֵ��������ʷ״̬
         3. ���ݴ�����״̬���ƶ�Ӧָʾ��
����:    ��
����:    ��
��ע:    ���߼��Ϊ1���׵�Ϊ0��ƫ��ֵ����ƫΪ������ƫΪ�����м�Ϊ0
����:    �޸���
****************************************************/
void track_check(void)
{
    // ��ȡ5·ѭ��������״̬�����µ�λ��ṹ��
    track_state.track.bit1 = GPIOGetValue(PORT2, 11);    // XJ5״̬
    track_state.track.bit2 = GPIOGetValue(PORT2, 8);    // XJ2״̬
    track_state.track.bit3 = GPIOGetValue(PORT2, 7);    // XJ1���м䣩״̬
    track_state.track.bit4 = GPIOGetValue(PORT2, 9);   // XJ3״̬
    track_state.track.bit5 = GPIOGetValue(PORT2, 10);   // XJ4״̬

    // ������ʷ״̬���������20��ֵ��
    for (uint16_t i = 19; i > 0; i--)
    {
        track_status_backup[0][i] = track_status_backup[0][i - 1];
    }
    track_status_backup[0][0] = track_status[0];  // ����ֵ���뱸����λ��

    // ���ݴ��������״̬����ѭ��ƫ��ֵ
	switch (track_state.state)
	{
		case 0x01:  // 00001����XJ1��⵽���ߣ�����
			track_status[0] = -20;
			track_status_worse /= 2;  // ״̬�����������쳣����
			break;
		case 0x03:  // 00011��XJ1��XJ2��⵽����
			track_status[0] = -15;
			track_status_worse /= 2;
			break;
		case 0x02:  // 00010����XJ2��⵽����
			track_status[0] = -10;
			track_status_worse /= 2;
			break;
		case 0x07:  // 00111��XJ1��XJ2��XJ3��⵽���ߣ����У�
			track_status[0] = -25;  // ����Χ��ƫ��ȡ�м�ƫ��ֵ
			track_status_worse /= 2;
			break;
		case 0x0F:  // 01111��XJ1��XJ2��XJ3��XJ4��⵽���ߣ������ң�
			track_status[0] = -30;  // ����Χ��ƫ������123��12֮��
			track_status_worse /= 2;
			break;
		case 0x06:  // 00110��XJ2��XJ3��⵽����
			track_status[0] = -5;
			track_status_worse /= 2;
			break;
		case 0x04:  // 00100����XJ3��⵽���ߣ��м䣩
			track_status[0] = 0;
			track_status_worse /= 2;
			break;
		case 0x0C:  // 01100��XJ3��XJ4��⵽����
			track_status[0] = 5;
			track_status_worse /= 2;
			break;
		case 0x08:  // 01000����XJ4��⵽����
			track_status[0] = 10;
			track_status_worse /= 2;
			break;
		case 0x1E:  // 11110��XJ2��XJ3��XJ4��XJ5��⵽���ߣ������ң�
			track_status[0] = 30;  // ����Χ��ƫ������345��45֮��
			track_status_worse /= 2;
			break;
		case 0x1C:  // 11100��XJ3��XJ4��XJ5��⵽���ߣ��е��ң�
			track_status[0] = 25;  // ����Χ��ƫ��ȡ�м�ƫ��ֵ
			track_status_worse /= 2;
			break;
		case 0x18:  // 11000��XJ4��XJ5��⵽����
			track_status[0] = 15;
			track_status_worse /= 2;
			break;
		case 0x10:  // 10000����XJ5��⵽���ߣ����ң�
			track_status[0] = 20;
			track_status_worse /= 2;
			break;
		case 0x1F:  // 11111��XJ1-XJ5ȫ��⵽���ߣ�ȫ��Χ��
			track_status[0] = 50;  // ��Ϊ���У�������ߣ�
			track_status_worse /= 2;
			break;
		case 0x00:  // 00000���޴�������⵽���ߣ��쳣��
			track_status[0] = track_status_backup[0][0];  // ������ʷֵ
			track_status_worse++;  // �����쳣����
			break;
		default:    // ����δ����״̬���쳣��
			track_status[0] = track_status_backup[0][0];  // ������ʷֵ
			track_status_worse++;  // �����쳣����
			break;
	}

    // ���ݴ�����״̬���ƶ�Ӧָʾ�ƣ����ߡ��������׵ס�����
    if (track_state.track.bit1 == 1) LeftFrontLight_On();
    else LeftFrontLight_Off();

    if (track_state.track.bit2 == 1) LeftRearLight_On();
    else LeftRearLight_Off();

    if (track_state.track.bit3 == 1) LED_On();  // �м��
    else LED_Off();

    if (track_state.track.bit4 == 1) RightFrontLight_On();
    else RightFrontLight_Off();

    if (track_state.track.bit5 == 1) RightRearLight_On();
    else RightRearLight_Off();
}


