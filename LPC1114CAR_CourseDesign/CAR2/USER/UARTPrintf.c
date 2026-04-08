////#include "UARTPrintf.h"
////#include <stdio.h>
////#include "driver_config.h"
////#include "uart.h"



////extern volatile uint8_t  UARTBuffer[BUFSIZE];
////extern volatile uint32_t UARTCount;

/////******************************************************************************/
/////******************************************************************
//// * 函数名: UARTInit
//// * 描述: 初始化 UART0 端口，设置选中引脚、时钟，校验、停止位、FIFO、等等。
//// * 参数: UART 波特率
//// * 返回值: 无
////******************************************************************/
////  void UARTPrintf_Init(uint32_t baudrate)
////  {
////    UARTInit(baudrate);  // 直接调用驱动中的初始化函数
////  }


/////************************************************/
/////* 函数功能：串口接收字节数据  */
/////************************************************/
////uint8_t UARTPrintf_Recive(void)
////{  
////    uint8_t data;
////    while (UARTCount == 0);  // 等待缓冲区有数据（中断会填充缓冲区）
////    
////    // 读取缓冲区数据
////    data = UARTBuffer[0];
////    
////    // 缓冲区数据前移（处理环形缓冲）
////    for (uint32_t i = 0; i < UARTCount - 1; i++)
////    {
////        UARTBuffer[i] = UARTBuffer[i + 1];
////    }
////    UARTCount--;  // 缓冲区计数减1
////    
////    return data;
////}

/////************************************************/
/////* 函数功能：串口发送字节数据  */
/////************************************************/
////void UARTPrintf_Send_Byte(uint8_t byte)
////{
////    UARTSend(&byte, 1);  // 调用驱动的多字节发送函数，发送单个字节

////}

/////******************************************************************
//// * 函数名: UARTSend
//// * 描述: 根据数据长度发送一串数据到 UART 0 端口。
//// * 参数: buffer pointer, and data length
//// * 返回值: 无
////******************************************************************/
//// void UARTPrintf_Send(uint8_t *BufferPtr, uint32_t Length)
//// {
////    UARTSend(BufferPtr, Length);  // 直接调用驱动中的发送函数
//// }
/////******************************************************************************/
//// /**
//// * 函数功能：重定向printf到UART
//// * 描述：此函数实现了printf的底层重定向，使得所有的printf输出都通过UART进行发送。
//// * 参数：
//// *      ch - 要输出的字符
//// *      f  - 文件指针（对于此函数不使用，仅作为printf的标准接口）
//// * 返回值：
//// *      返回输出的字符（保持原始格式）
//// */
////int fputc(int ch, FILE *f)
////{
////    UARTPrintf_Send_Byte(ch);  // 将字符通过UART发送
////    return ch;           // 返回输出字符
////}
//#include "UARTPrintf.h"
//#include <stdio.h>
///******************************************************************************/
///******************************************************************
// * 函数名: UARTInit
// * 描述: 初始化 UART0 端口，设置选中引脚、时钟，校验、停止位、FIFO、等等。
// * 参数: UART 波特率
// * 返回值: 无
//******************************************************************/
//  void UART_Init(uint32_t baudrate)
//  {
//  	uint32_t Fdiv; // 分频值
//  	uint32_t regVal = regVal; // 临时变量
//  	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);  // 使能 IOCON 模块的时钟
//  	LPC_IOCON->PIO1_6 &= ~0x07;  // UART I/O 配置，设置 PIO1_6 为 UART RXD 
//  	LPC_IOCON->PIO1_6 |= 0x01; 
//  	LPC_IOCON->PIO1_7 &= ~0x07; // 设置 PIO1_7 为 UART TXD 
//  	LPC_IOCON->PIO1_7 |= 0x01; 
//  	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 16); // 关闭 IOCON 模块的时钟
//  	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12); // 使能 UART 时钟
//  	LPC_SYSCON->UARTCLKDIV = 0x1; // 分频值 1
//  	LPC_UART->LCR = 0x83; // 设置 UART 线控制寄存器 (LCR)，配置为 8 位数据，奇校验位，1 停止位 	
//  	Fdiv = (SystemCoreClock / 16) / baudrate; // 计算波特率分频值
//  	LPC_UART->DLM = Fdiv / 256; // 设置波特率低字节和高字节
//  	LPC_UART->DLL = Fdiv % 256; 
//  	LPC_UART->LCR = 0x03; // 重置 LCR，禁用 DLAB 
//  	LPC_UART->FCR = 0x07; // 使能并复位 TX 和 RX FIFO	
//  	regVal = LPC_UART->LSR; // 读取并清除行状态寄存器 (LSR)
//  	// 确保正常开始，TX 和 RX FIFO 中没有数据
//  	while ((LPC_UART->LSR & (0x20 | 0x40)) != (0x20 | 0x40));
//  	while (LPC_UART->LSR & 0x01)
//  	{
//  		regVal = LPC_UART->RBR; // 从 RX FIFO 中转储数据
//  	}
//	LPC_UART->IER |=1<<0; //使能接受中断
//    NVIC_EnableIRQ(UART_IRQn); //启动中断
//  }


///************************************************/
///* 函数功能：串口接收字节数据  */
///************************************************/
//uint8_t UART_Recive(void)
//{  
//  while(!(LPC_UART->LSR & (1<<0)));   //等待接收到数据
//  return(LPC_UART->RBR);   //读出数据
//}

///************************************************/
///* 函数功能：串口发送字节数据  */
///************************************************/
//void UART_Send_Byte(uint8_t byte)
//{
//    LPC_UART->THR = byte;
//  while ( !(LPC_UART->LSR & (1<<5)) );//等待发送完

//}

///******************************************************************
// * 函数名: UARTSend
// * 描述: 根据数据长度发送一串数据到 UART 0 端口。
// * 参数: buffer pointer, and data length
// * 返回值: 无
//******************************************************************/
// void UART_Send(uint8_t *BufferPtr, uint32_t Length)
// {
// 	while (Length != 0) // 判断数据长度，不为0则发送数据
// 	{
// 		while (!(LPC_UART->LSR & (1 << 5))); // 等待直到发送数据寄存器空
// 		LPC_UART->THR = *BufferPtr; // 发送当前字节
// 		BufferPtr++; // 移动到下一个字节
// 		Length--; // 递减数据长度
// 	}
// }
///******************************************************************************/
// /**
// * 函数功能：重定向printf到UART
// * 描述：此函数实现了printf的底层重定向，使得所有的printf输出都通过UART进行发送。
// * 参数：
// *      ch - 要输出的字符
// *      f  - 文件指针（对于此函数不使用，仅作为printf的标准接口）
// * 返回值：
// *      返回输出的字符（保持原始格式）
// */
//int fputc(int ch, FILE *f)
//{
//    UART_Send_Byte(ch);  // 将字符通过UART发送
//    return ch;           // 返回输出字符
//}
