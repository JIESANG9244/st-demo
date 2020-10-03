# ST-DEMO
#####################################################################
## 野火STM32F1系列开发板学习案例
1. 野火F1开发板STM32-USART使用                2020-10-02
2. 野火F1开发板STM32案例-外部中断(按键)使用    2020-10-03
3. 更新....

#####################################################################


# 野火F1开发板STM32-USART使用
## 硬件平台
1. 野火STM32F103ZET6 霸道V2开发板
2. 正点原子F1开发板
3. STM32蓝色板
![野火STM32产品](https://img-blog.csdnimg.cn/20201002210908329.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)

## 软件平台
1. Keil MDK 5.31
2. 串口调试助手
## 串口配置步骤
1. 使能RX和TX引脚GPIO时钟和USART时钟；
2. 初始化 GPIO，并将GPIO复用到USART.上；
3. 配置 USART参数；
4. 配置中断控制器并使能USART接收中断；
5. 使 能USART；
6. 在USART接收中断服务函数实现数据接收和发送。
## 工程模板结构
![工程模板结构](https://img-blog.csdnimg.cn/20201002204307734.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
文件夹 STExtend外设扩展文件，USER用户自定义函数库
工程文件下载链接<https://download.csdn.net/download/weixin_43599390/12909901>
```c
//bsp_xx.h板载第三方库文件
#include "stm32f10x.h"
#include "bsp_led.h"  
#include "bsp_key.h" 
#include "bsp_beep.h"
#include "bsp_exit.h"
#include "timer.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "bsp_test.h"
#include "STM32F103xxxx_USART.H"  
```

## STM32F103ZET6 串口引脚列表
![串口引脚列表](https://img-blog.csdnimg.cn/20201002205012211.png#pic_center)

### STM32F103xxxx_USART.C

```c
//==================================================================================================                                  // 包含全局头文件
#include"STM32F103xxxx_USART.H"                     // 包含内设头文件 USART-USART通信总线

//--------------------------------------------------------------------------------------------------
//  宏自定义声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  定义引用变量    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  硬件端口定义    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  引用函数声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//--------------------------------------------------------------------------------------------------
void USART_Configure(void);
void USART_SendChar(USART_TypeDef* USARTx, unsigned char Char);
unsigned char USART_ReceiveChar(USART_TypeDef* USARTx);
void USART_SendString(USART_TypeDef* USARTx, unsigned char *String);
void USART_SendArray(USART_TypeDef* USARTx, unsigned char *Array, unsigned char Count);
void USART_SendLine(USART_TypeDef* USARTx);
void USART_SendNumber_SignedInteger(USART_TypeDef* USARTx, signed short int Number, unsigned char Count);
void USART_SendNumber_UnsignedInteger(USART_TypeDef* USARTx, unsigned short int Number, unsigned char Count);
void USART_SendNumber_Float(USART_TypeDef* USARTx, float Number, unsigned char Count1, unsigned char Count2);

//==================================================================================================
//  函数功能: USART 发送字节
//  函数标记: 内设驱动函数
//  函数说明: 本函数没有考虑发送9位数据可能性
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//            Byte - 字节  取值范文 - ASCII字符
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void USART_SendChar(USART_TypeDef* USARTx, unsigned char Char)
{
    while((USARTx -> SR & 0x40) == 0);              // 等待上次发送完成   
    USARTx -> DR = Char;                            // 装载本次发送数据    
}

//==================================================================================================
//  函数功能: USART 接收字符
//  函数标记: 内设驱动函数
//  函数说明: 
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//  输出参量: Char - 待接收字节  取值范围 - ASCII字符
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
unsigned char USART_ReceiveChar(USART_TypeDef* USARTx)
{
    return USARTx->DR;
}

//==================================================================================================
//  函数功能: USART 发送字符串
//  函数标记: 内设驱动函数
//  函数说明: 无
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//            String - 待发送字符串指针  取值范文 - ASCII字符串
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void USART_SendString(USART_TypeDef* USARTx,unsigned char *String)
{
    while(*String != '\0')                          // \0 表示字符串结束标志，通过检测是否字符串末尾
    {
        USART_SendChar(USARTx, *String);
        String++;
    }
}

//==================================================================================================
//  函数功能: USART 发送数组
//  函数标记: 内设驱动函数
//  函数说明: 无
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//            Array - 待发送数组的指针  取值范围 - 指针类型
//            Count - 待发送数组的数量  取值范围 - 0~255
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void USART_SendArray(USART_TypeDef* USARTx, unsigned char *Array, unsigned char Count)
{
    unsigned char i = 0;                            // 定义局部变量 用于函数循环
    for(i=0; i<Count; i++)
    {
        USART_SendChar(USARTx, Array[i]);    
    }
}

//==================================================================================================
//  函数功能: USART 发送回车换行
//  函数标记: 内设驱动函数
//  函数说明: 无
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void USART_SendLine(USART_TypeDef* USARTx)
{
    USART_SendChar(USARTx, 0x0D);                   // 换行
    USART_SendChar(USARTx, 0x0A);                   // 回车
}

//==================================================================================================
//  函数功能: USART 发送有符号整型数字
//  函数标记: 内设驱动函数
//  函数说明: 无
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//            Number - 待发送整数  取值范围 - 整型类型 -99999~+99999
//            Count - 显示的位数  取值范围 - 1~5
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void USART_SendNumber_SignedInteger(USART_TypeDef* USARTx, signed short int Number, unsigned char Count)
{
    unsigned char NumberArray[5] = {0};             // 定义局部数组，用于数据存储
    
    // 判断 整型数字正负
    if(Number<0)
    {
        Number = 0 - Number;                        // 负数转整数
        USART_SendChar(USARTx,'-');
    }
    else
    {
        USART_SendChar(USARTx,'+');
    }
        
    // 计算各位数值的字符
    if(Count>4) NumberArray[0]=(Number/10000) % 10 + 0x30; 
    if(Count>3) NumberArray[1]=(Number/1000 ) % 10 + 0x30; 
    if(Count>2) NumberArray[2]=(Number/100  ) % 10 + 0x30; 
    if(Count>1) NumberArray[3]=(Number/10   ) % 10 + 0x30; 
    if(Count>0) NumberArray[4]=(Number/1    ) % 10 + 0x30; 
   
    // 发送串口数据
    USART_SendArray(USARTx,&NumberArray[5-Count],Count);
}

//==================================================================================================
//  函数功能: USART 发送无符号整型数字
//  函数标记: 内设驱动函数
//  函数说明: 无
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//            Number - 待发送整数  取值范围 - 整型类型 0~99999
//            Count - 显示的位数  取值范围 - 1~5
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void USART_SendNumber_UnsignedInteger(USART_TypeDef* USARTx, unsigned short int Number, unsigned char Count)
{
    unsigned char NumberArray[5] = {0};             // 定义局部数组，用于数据存储
    
    // 断言检查参数
    
    
    // 执行相关操作
    // 计算各位数值的字符
    if(Count>4) NumberArray[0]=(Number/10000) % 10 + 0x30;
    if(Count>3) NumberArray[1]=(Number/1000 ) % 10 + 0x30;
    if(Count>2) NumberArray[2]=(Number/100  ) % 10 + 0x30;
    if(Count>1) NumberArray[3]=(Number/10   ) % 10 + 0x30;
    if(Count>0) NumberArray[4]=(Number/1    ) % 10 + 0x30;
    
    // 发送串口数据
    USART_SendArray(USARTx,&NumberArray[5-Count],Count);
}

//==================================================================================================
//  函数功能: USART 发送数字 浮点类型
//  函数标记: 内设驱动函数
//  函数说明: 本函数打印浮点数字与实际输入数字存在偏差
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//            Number - 待发送浮点型数字  取值范围 - -99999.99999~99999.99999
//            Count1 - 整数显示位数  取值范围 - 0~5
//            Count2 - 小数显示位数  取值范围 - 0~5
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void USART_SendNumber_Float(USART_TypeDef* USARTx, float Number, unsigned char Count1, unsigned char Count2)
{
    unsigned char Number_Integer_Array[5];          // 定义局部数组 用于存储整数位各位数据
    unsigned char Number_Decimal_Array[5];          // 定义局部数组 用于存储小数位各位数据

    unsigned long int  Number_Integer = 0;          // 定义局部变量 表示浮点数的整数部分
    unsigned long int  Number_Decimal = 0;          // 定义局部变量 表示浮点数的小数部分

    // 判断 浮点数字正负
    if(Number < 0)
    {
        Number = 0 - Number;
        USART_SendChar(USARTx, '-');
    }
    else
    {
        USART_SendChar(USARTx, '+');
    }
    
    // 限制 发送位数
    // 由于由于float型的有效十进制数值最多为7位，即整数位 + 小数位 数量一定小于等于7
    while((Count1 + Count2 > 7 ))
    {
        if((Count1 > 5) && (Count1 != 0))
        {
            --Count1;
        }
        else
        {
            --Count2;
        }
    }
    
    // 分离 整数位与小数位
    // 取整数部分
    Number_Integer = (unsigned long int)(Number);  
    // 取小数部分 1e5科学计数法表示10000
    Number_Decimal = (unsigned long int)((Number - Number_Integer + 0.000005) * 1e5);
        
    // 计算各位数值的字符
    if(Count1>4) Number_Integer_Array[0]=(Number_Integer/10000) % 10 + 0x30; 
    if(Count1>3) Number_Integer_Array[1]=(Number_Integer/1000 ) % 10 + 0x30; 
    if(Count1>2) Number_Integer_Array[2]=(Number_Integer/100  ) % 10 + 0x30; 
    if(Count1>1) Number_Integer_Array[3]=(Number_Integer/10   ) % 10 + 0x30; 
    if(Count1>0) Number_Integer_Array[4]=(Number_Integer/1    ) % 10 + 0x30; 
    
    if(Count2>0) Number_Decimal_Array[0]=(Number_Decimal/10000) % 10 + 0x30; 
    if(Count2>1) Number_Decimal_Array[1]=(Number_Decimal/1000 ) % 10 + 0x30; 
    if(Count2>2) Number_Decimal_Array[2]=(Number_Decimal/100  ) % 10 + 0x30; 
    if(Count2>3) Number_Decimal_Array[3]=(Number_Decimal/10   ) % 10 + 0x30; 
    if(Count2>4) Number_Decimal_Array[4]=(Number_Decimal/1    ) % 10 + 0x30; 
   
    USART_SendArray(USARTx, &Number_Integer_Array[5-Count1], Count1);       // 显示整数位
    USART_SendChar(USARTx, '.');
    USART_SendArray(USARTx, &Number_Decimal_Array[0],        Count2);       // 显示小数位
}

//==================================================================================================
//  函数功能: USART 发送数字 二进制数字
//  函数标记: 内设驱动函数
//  函数说明: 无
//--------------------------------------------------------------------------------------------------
//  输入参量: USARTx - 通道号  取值范围 - USARTx1/USARTx2/USARTx3/UARTx4/UARTx5
//            Number - 待发送有符号长整型数字  取值范围 - -99999.99999~99999.99999
//            Count - 整数显示位数  取值范围 - 0~5
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void USART_SendNumber_Binary(USART_TypeDef* USARTx,int Number,unsigned char Count)
{
    unsigned char i = 0;
    
    for(i=Count; i>0; i--)
    {
        if(Number & ( 1 << (Count-1) ))
        {
            USART_SendChar(USARTx,'1');
        }
        else
        {
            USART_SendChar(USARTx,'0');
        }
        Number <<= 1;
    }
}

//==================================================================================================
//  函数功能: USART 预配置
//  函数标记: 内设驱动函数
//  函数说明: 无
//--------------------------------------------------------------------------------------------------
//  输入参量: 无
//  输出参量: 无
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//==================================================================================================
void USART_Configure(void)
{
  
    USART_InitTypeDef USART_InitStructure_USARTx1; 
    //----------------------------------------------------------------------------------------------
    // 配置 USARTx1 功能
    //----------------------------------------------------------------------------------------------
    #if (STM32F103xxxx_USARTx1 == 1)
    {
        // 参数检查
        #if ((STM32F103xxxx_USARTx1_BAUD != 600)   \
        &&   (STM32F103xxxx_USARTx1_BAUD != 1200)  \
        &&   (STM32F103xxxx_USARTx1_BAUD != 2400)  \
        &&   (STM32F103xxxx_USARTx1_BAUD != 4800)  \
        &&   (STM32F103xxxx_USARTx1_BAUD != 9600)  \
        &&   (STM32F103xxxx_USARTx1_BAUD != 14400) \
        &&   (STM32F103xxxx_USARTx1_BAUD != 19200) \
        &&   (STM32F103xxxx_USARTx1_BAUD != 38400) \
        &&   (STM32F103xxxx_USARTx1_BAUD != 115200))
        {
            #error "There are errors in the STM32F103xxxx_USARTx1_BAUD"
        }
        #endif  // #if ((STM32F103xxxx_USARTx1_BAUD != 600)   \
                   &&   (STM32F103xxxx_USARTx1_BAUD != 1200)  \
                   &&   (STM32F103xxxx_USARTx1_BAUD != 2400)  \
                   &&   (STM32F103xxxx_USARTx1_BAUD != 4800)  \
                   &&   (STM32F103xxxx_USARTx1_BAUD != 9600)  \
                   &&   (STM32F103xxxx_USARTx1_BAUD != 14400) \
                   &&   (STM32F103xxxx_USARTx1_BAUD != 19200) \
                   &&   (STM32F103xxxx_USARTx1_BAUD != 38400) \
                   &&   (STM32F103xxxx_USARTx1_BAUD != 115200))
        
        #if ((STM32F103xxxx_USARTx1_PORT_A09A10 + STM32F103xxxx_USARTx1_PORT_B06B07) != 1)
        {
            #error "There are errors in the STM32F103xxxx_USARTx1_PORT"
        }
        #endif  // #if ((STM32F103xxxx_USARTx1_PORT_A09A10 + STM32F103xxxx_USARTx1_PORT_B06B07) != 1)
        
        // 配置GPIO引脚映射功能
        #if (STM32F103xxxx_USARTx1_PORT_A09A10 == 1)                // 不映射
        {
        }
        #endif  // #if (STM32F103xxxx_USARTx1_PORT_A09A10 == 1)
        #if (STM32F103xxxx_USARTx1_PORT_B06B07 == 1)                // 完全映射
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
        }
        #endif  // #if (STM32F103xxxx_USARTx1_PORT_B06B07 == 1)
        
        // 配置GPIO TXD引脚
        #if (STM32F103xxxx_USARTx1_PORT_A09A10 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx1_TxD;                    // 定义结构体变量
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);               // 使能GPIOA模块时钟
            
            GPIO_InitStructure_USARTx1_TxD.GPIO_Pin   = GPIO_Pin_9;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx1_TxD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx1_TxD.GPIO_Mode  = GPIO_Mode_AF_PP;        // 设置GPIO引脚输出类型 推挽输出类型
            GPIO_Init(GPIOA, &GPIO_InitStructure_USARTx1_TxD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx1_PORT_A09A10 == 1)
   
        #if (STM32F103xxxx_USARTx1_PORT_B06B07 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx1_TxD;                    // 定义结构体变量
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);               // 使能GPIOB模块时钟
            
            GPIO_InitStructure_USARTx1_TxD.GPIO_Pin   = GPIO_Pin_6;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx1_TxD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx1_TxD.GPIO_Mode  = GPIO_Mode_AF_PP;        // 设置GPIO引脚输出类型 推挽输出类型
            GPIO_Init(GPIOB, &GPIO_InitStructure_USARTx1_TxD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx1_PORT_B06B07 == 1) 
        
        // 配置GPIO RXD引脚
        #if (STM32F103xxxx_USARTx1_PORT_A09A10 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx1_RxD;                    // 定义结构体变量
                        
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);               // 使能GPIOA模块时钟
            
            GPIO_InitStructure_USARTx1_RxD.GPIO_Pin   = GPIO_Pin_10;            // 设置GPIO引脚号
            GPIO_InitStructure_USARTx1_RxD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx1_RxD.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  // 设置GPIO引脚输出类型 浮空输入类型
            GPIO_Init(GPIOA, &GPIO_InitStructure_USARTx1_RxD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx1_PORT_A09A10 == 1)
   
        #if (STM32F103xxxx_USARTx1_PORT_B06B07 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx1_RxD;                    // 定义结构体变量
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);               // 使能GPIOB模块时钟
            
            GPIO_InitStructure_USARTx1_RxD.GPIO_Pin   = GPIO_Pin_7;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx1_RxD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx1_RxD.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  // 设置GPIO引脚输出类型 浮空输入类型
            GPIO_Init(GPIOB, &GPIO_InitStructure_USARTx1_RxD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx1_PORT_B06B07 == 1) 
        
        // 配置 USARTx1 功能
       // USART_InitTypeDef USART_InitStructure_USARTx1;                                          // 定义结构体变量
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);                                  // 使能USART1模块时钟
        
        USART_DeInit(USART1);
        
        USART_InitStructure_USARTx1.USART_BaudRate   = STM32F103xxxx_USARTx1_BAUD;              // 设置USART波特率
        USART_InitStructure_USARTx1.USART_WordLength = USART_WordLength_8b;                     // 设置USART数据位长度 8位数据位
        USART_InitStructure_USARTx1.USART_StopBits   = USART_StopBits_1;                        // 设置USART停止位长度 1位停止位
        USART_InitStructure_USARTx1.USART_Parity     = USART_Parity_No;                         // 设置USART校验位长度 无校验
        USART_InitStructure_USARTx1.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 设置USART硬件数据流控制
        USART_InitStructure_USARTx1.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;           // 设置USART模式 发送模式+接收模式
        USART_Init(USART1, &USART_InitStructure_USARTx1);                                       // 初始化结构体
        USART_Cmd(USART1, ENABLE);                                                              // 使能USART1模块运行
        
        // 配置 USARTx1 中断操作
        #if (STM32F103xxxx_USARTx1_ITEN_RXNE == 1)
        {
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);      // 启用USARTx1接受中断
        }
        #endif  // #if (STM32F103xxxx_USARTx1_ITEN_RXNE == 1)
        
        #if (STM32F103xxxx_USARTx1_ITEN_IDLE == 1)              
        {
            USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);      // 启用USARTx1空闲中断
        }
        #endif  // #if (STM32F103xxxx_USARTx1_ITEN_IDLE == 1)
        
        #if (STM32F103xxxx_USARTx1_ITEN_TC == 1)
        {
            USART_ITConfig(USART1, USART_IT_TC, ENABLE);        // 启用USARTx1发送完成中断
        }
        #endif  // #if (STM32F103xxxx_USARTx1_ITEN_TC == 1)
        
        // 配置 USARTx1 DMA操作
        #if (STM32F103xxxx_USARTx1_DMA_TX == 1)
        {
            USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);      // 启用USARTx1 DMA发送
        }
        #endif  // #if (STM32F103xxxx_USARTx1_DMA_TX == 1)
        
        #if (STM32F103xxxx_USARTx1_DMA_RX == 1)
        {
            USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);      // 启用USARTx1 DMA接收
        }
        #endif  // #if (STM32F103xxxx_USARTx1_DMA_RX == 1)
    }
    #endif  // #if (STM32F103xxxx_USARTx1 == 1)
    
    //----------------------------------------------------------------------------------------------
    // 配置 USARTx2 功能
    //----------------------------------------------------------------------------------------------
    #if (STM32F103xxxx_USARTx2 == 1)
    {
        // 参数检查
        #if ((STM32F103xxxx_USARTx2_BAUD != 600)   \
        &&   (STM32F103xxxx_USARTx2_BAUD != 1200)  \
        &&   (STM32F103xxxx_USARTx2_BAUD != 2400)  \
        &&   (STM32F103xxxx_USARTx2_BAUD != 4800)  \
        &&   (STM32F103xxxx_USARTx2_BAUD != 9600)  \
        &&   (STM32F103xxxx_USARTx2_BAUD != 14400) \
        &&   (STM32F103xxxx_USARTx2_BAUD != 19200) \
        &&   (STM32F103xxxx_USARTx2_BAUD != 38400) \
        &&   (STM32F103xxxx_USARTx2_BAUD != 115200))
        {
            #error "There are errors in the STM32F103xxxx_USARTx2_BAUD"
        }
        #endif  // #if ((STM32F103xxxx_USARTx2_BAUD != 600)   \
                   &&   (STM32F103xxxx_USARTx2_BAUD != 1200)  \
                   &&   (STM32F103xxxx_USARTx2_BAUD != 2400)  \
                   &&   (STM32F103xxxx_USARTx2_BAUD != 4800)  \
                   &&   (STM32F103xxxx_USARTx2_BAUD != 9600)  \
                   &&   (STM32F103xxxx_USARTx2_BAUD != 14400) \
                   &&   (STM32F103xxxx_USARTx2_BAUD != 19200) \
                   &&   (STM32F103xxxx_USARTx2_BAUD != 38400) \
                   &&   (STM32F103xxxx_USARTx2_BAUD != 115200))
        
        #if ((STM32F103xxxx_USARTx2_PORT_A02A03 + STM32F103C8T6_USARTx2_PORT_D06D07) != 1)
        {
            #error "There are errors in the STM32F103xxxx_USARTx2_PORT"
        }
        #endif  // #if ((STM32F103xxxx_USARTx2_PORT_A02A03 + STM32F103C8T6_USARTx2_PORT_D06D07) != 1)
        
        // 配置GPIO引脚映射功能
        #if (STM32F103xxxx_USARTx2_PORT_A02A03 == 1)                // 不映射
        {
        }
        #endif  // #if (STM32F103xxxx_USARTx2_PORT_A02A03 == 1)
        #if (STM32F103C8T6_USARTx2_PORT_D06D07 == 1)                // 完全映射
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            GPIO_PinRemapConfig(GPIO_FullRemap_USART2, ENABLE);  
        }
        #endif  // #if (STM32F103C8T6_USARTx2_PORT_D06D07 == 1)
        
        // 配置GPIO TXD引脚
        #if (STM32F103xxxx_USARTx2_PORT_A02A03 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx2_TxD;                    // 定义结构体变量
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);                // 使能GPIOA模块时钟
            
            GPIO_InitStructure_USARTx2_TxD.GPIO_Pin   = GPIO_Pin_2;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx2_TxD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx2_TxD.GPIO_Mode  = GPIO_Mode_AF_PP;        // 设置GPIO引脚输出类型 推挽输出类型
            GPIO_Init(GPIOA, &GPIO_InitStructure_USARTx2_TxD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx2_PORT_A02A03 == 1)
   
        #if (STM32F103C8T6_USARTx2_PORT_D06D07 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx2_TxD;                    // 定义结构体变量
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);                // 使能GPIOD模块时钟
            
            GPIO_InitStructure_USARTx2_TxD.GPIO_Pin   = GPIO_Pin_6;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx2_TxD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx2_TxD.GPIO_Mode  = GPIO_Mode_AF_PP;        // 设置GPIO引脚输出类型 推挽输出类型
            GPIO_Init(GPIOD, &GPIO_InitStructure_USARTx2_TxD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103C8T6_USARTx2_PORT_D06D07 == 1) 
        
        // 配置GPIO RXD引脚
        #if (STM32F103xxxx_USARTx2_PORT_A02A03 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx2_RxD;                    // 定义结构体变量
                        
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);                // 使能GPIOA模块时钟
            
            GPIO_InitStructure_USARTx2_RxD.GPIO_Pin   = GPIO_Pin_3;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx2_RxD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx2_RxD.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  // 设置GPIO引脚输出类型 浮空输入类型
            GPIO_Init(GPIOA, &GPIO_InitStructure_USARTx2_RxD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx2_PORT_A02A03 == 1)
   
        #if (STM32F103C8T6_USARTx2_PORT_D06D07 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx2_RxD;                    // 定义结构体变量
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);                // 使能GPIOD模块时钟
            
            GPIO_InitStructure_USARTx2_RxD.GPIO_Pin   = GPIO_Pin_7;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx2_RxD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx2_RxD.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  // 设置GPIO引脚输出类型 浮空输入类型
            GPIO_Init(GPIOD, &GPIO_InitStructure_USARTx2_RxD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103C8T6_USARTx2_PORT_D06D07 == 1) 
        
        // 配置 USARTx2 功能
        USART_InitTypeDef USART_InitStructure_USARTx2;                                          // 定义结构体变量
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);                                  // 使能USART2模块时钟
        
        USART_DeInit(USART2);
        
        USART_InitStructure_USARTx2.USART_BaudRate   = STM32F103xxxx_USARTx2_BAUD;              // 设置USART波特率
        USART_InitStructure_USARTx2.USART_WordLength = USART_WordLength_8b;                     // 设置USART数据位长度 8位数据位
        USART_InitStructure_USARTx2.USART_StopBits   = USART_StopBits_1;                        // 设置USART停止位长度 1位停止位
        USART_InitStructure_USARTx2.USART_Parity     = USART_Parity_No;                         // 设置USART校验位长度 无校验
        USART_InitStructure_USARTx2.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 设置USART硬件数据流控制
        USART_InitStructure_USARTx2.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;           // 设置USART模式 发送模式+接收模式
        USART_Init(USART2, &USART_InitStructure_USARTx2);                                       // 初始化结构体
        USART_Cmd(USART2, ENABLE);                                                              // 使能USART2模块运行
        
        // 配置 USARTx2 中断操作
        #if (STM32F103xxxx_USARTx2_ITEN_RXNE == 1)
        {
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);      // 启用USARTx2接受中断
        }
        #endif  // #if (STM32F103xxxx_USARTx2_ITEN_RXNE == 1)
        
        #if (STM32F103xxxx_USARTx2_ITEN_IDLE == 1)              
        {
            USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);      // 启用USARTx2空闲中断
        }
        #endif  // #if (STM32F103xxxx_USARTx2_ITEN_IDLE == 1)
        
        #if (STM32F103xxxx_USARTx2_ITEN_TC == 1)
        {
            USART_ITConfig(USART2, USART_IT_TC, ENABLE);        // 启用USARTx2发送完成中断
        }
        #endif  // #if (STM32F103xxxx_USARTx2_ITEN_TC == 1)
        
        // 配置 USARTx2 DMA操作
        #if (STM32F103xxxx_USARTx2_DMA_TX == 1)
        {
            USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);      // 启用USARTx2 DMA发送
        }
        #endif  // #if (STM32F103xxxx_USARTx2_DMA_TX == 1)
        
        #if (STM32F103xxxx_USARTx2_DMA_RX == 1)
        {
            USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);      // 启用USARTx2 DMA接收
        }
        #endif  // #if (STM32F103xxxx_USARTx2_DMA_RX == 1)
    }
    #endif  // #if (STM32F103xxxx_USARTx1 == 1)
    
    //----------------------------------------------------------------------------------------------
    // 配置 USARTx3 功能
    //----------------------------------------------------------------------------------------------
    #if (STM32F103xxxx_USARTx3 == 1)  
    {
        // 参数检查
        #if ((STM32F103xxxx_USARTx3_BAUD != 600)   \
        &&   (STM32F103xxxx_USARTx3_BAUD != 1200)  \
        &&   (STM32F103xxxx_USARTx3_BAUD != 2400)  \
        &&   (STM32F103xxxx_USARTx3_BAUD != 4800)  \
        &&   (STM32F103xxxx_USARTx3_BAUD != 9600)  \
        &&   (STM32F103xxxx_USARTx3_BAUD != 14400) \
        &&   (STM32F103xxxx_USARTx3_BAUD != 19200) \
        &&   (STM32F103xxxx_USARTx3_BAUD != 38400) \
        &&   (STM32F103xxxx_USARTx3_BAUD != 115200))
        {
            #error "There are errors in the STM32F103xxxx_USARTx3_BAUD"
        }
        #endif  // #if ((STM32F103xxxx_USARTx3_BAUD != 600)   \
                   &&   (STM32F103xxxx_USARTx3_BAUD != 1200)  \
                   &&   (STM32F103xxxx_USARTx3_BAUD != 2400)  \
                   &&   (STM32F103xxxx_USARTx3_BAUD != 4800)  \
                   &&   (STM32F103xxxx_USARTx3_BAUD != 9600)  \
                   &&   (STM32F103xxxx_USARTx3_BAUD != 14400) \
                   &&   (STM32F103xxxx_USARTx3_BAUD != 19200) \
                   &&   (STM32F103xxxx_USARTx3_BAUD != 38400) \
                   &&   (STM32F103xxxx_USARTx3_BAUD != 115200))
        
        #if ((STM32F103xxxx_USARTx3_PORT_B10B11 + STM32F103xxxx_USARTx3_PORT_C10C11 + STM32F103xxxx_USARTx3_PORT_D08D09) != 1)
        {
            #error "There are errors in the STM32F103xxxx_USARTx3_PORT"
        }
        #endif  // #if ((STM32F103xxxx_USARTx3_PORT_B10B11 + STM32F103xxxx_USARTx3_PORT_C10C11 + STM32F103xxxx_USARTx3_PORT_D08D09) != 1)

        // 配置GPIO引脚映射功能
        #if (STM32F103xxxx_USARTx3_PORT_B10B11 == 1)                // 不映射
        {
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_B10B11 == 1)
        #if (STM32F103xxxx_USARTx3_PORT_C10C11 == 1)                // 部分映射
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE); 
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_C10C11 == 1)
        #if (STM32F103xxxx_USARTx3_PORT_D08D09 == 1)                // 完全映射
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);  
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_D08D09 == 1)

        // 配置GPIO TXD引脚
        #if (STM32F103xxxx_USARTx3_PORT_B10B11 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx3_TXD;                    // 定义结构体变量

            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);               // 使能GPIOB模块时钟

            GPIO_InitStructure_USARTx3_TXD.GPIO_Pin   = GPIO_Pin_10;            // 设置GPIO引脚号
            GPIO_InitStructure_USARTx3_TXD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx3_TXD.GPIO_Mode  = GPIO_Mode_AF_PP;        // 设置GPIO引脚输出类型 推挽输出类型
            GPIO_Init(GPIOB, &GPIO_InitStructure_USARTx3_TXD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_B10B11 == 1)
        #if (STM32F103xxxx_USARTx3_PORT_C10C11 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx3_TXD;                    // 定义结构体变量

            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);               // 使能GPIOC模块时钟

            GPIO_InitStructure_USARTx3_TXD.GPIO_Pin   = GPIO_Pin_10;            // 设置GPIO引脚号
            GPIO_InitStructure_USARTx3_TXD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx3_TXD.GPIO_Mode  = GPIO_Mode_AF_PP;        // 设置GPIO引脚输出类型 推挽输出类型
            GPIO_Init(GPIOC, &GPIO_InitStructure_USARTx3_TXD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_C10C11 == 1)
        #if (STM32F103xxxx_USARTx3_PORT_D08D09 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx3_TXD;                    // 定义结构体变量

            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);               // 使能GPIOD模块时钟

            GPIO_InitStructure_USARTx3_TXD.GPIO_Pin   = GPIO_Pin_8;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx3_TXD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx3_TXD.GPIO_Mode  = GPIO_Mode_AF_PP;        // 设置GPIO引脚输出类型 推挽输出类型
            GPIO_Init(GPIOD, &GPIO_InitStructure_USARTx3_TXD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_D08D09 == 1)
        
        // 配置GPIO RXD引脚
        #if (STM32F103xxxx_USARTx3_PORT_B10B11 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx3_RXD;                    // 定义结构体变量

            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);               // 使能GPIOB模块时钟

            GPIO_InitStructure_USARTx3_RXD.GPIO_Pin   = GPIO_Pin_11;            // 设置GPIO引脚号
            GPIO_InitStructure_USARTx3_RXD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx3_RXD.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  // 设置GPIO引脚输出类型 浮空输入类型
            GPIO_Init(GPIOB, &GPIO_InitStructure_USARTx3_RXD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_B10B11 == 1)
        #if (STM32F103xxxx_USARTx3_PORT_C10C11 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx3_RXD;                    // 定义结构体变量

            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);               // 使能GPIOC模块时钟

            GPIO_InitStructure_USARTx3_RXD.GPIO_Pin   = GPIO_Pin_11;            // 设置GPIO引脚号
            GPIO_InitStructure_USARTx3_RXD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx3_RXD.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  // 设置GPIO引脚输出类型 浮空输入类型
            GPIO_Init(GPIOC, &GPIO_InitStructure_USARTx3_RXD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_C10C11 == 1)
        #if (STM32F103xxxx_USARTx3_PORT_D08D09 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_USARTx3_RXD;                    // 定义结构体变量

            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);               // 使能GPIOD模块时钟

            GPIO_InitStructure_USARTx3_RXD.GPIO_Pin   = GPIO_Pin_9;             // 设置GPIO引脚号
            GPIO_InitStructure_USARTx3_RXD.GPIO_Speed = GPIO_Speed_50MHz;       // 设置GPIO引脚速率
            GPIO_InitStructure_USARTx3_RXD.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  // 设置GPIO引脚输出类型 浮空输入类型
            GPIO_Init(GPIOD, &GPIO_InitStructure_USARTx3_RXD);                  // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_USARTx3_PORT_D08D09 == 1)
        
        // 配置 USARTx3 功能
        USART_InitTypeDef USART_InitStructure_USARTx3;                                          // 定义结构体变量
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);                                  // 使能USART3模块时钟
        
        USART_DeInit(USART3);
        
        USART_InitStructure_USARTx3.USART_BaudRate   = STM32F103xxxx_USARTx3_BAUD;              // 设置USART波特率
        USART_InitStructure_USARTx3.USART_WordLength = USART_WordLength_8b;                     // 设置USART数据位长度 8位数据位
        USART_InitStructure_USARTx3.USART_StopBits   = USART_StopBits_1;                        // 设置USART停止位长度 1位停止位
        USART_InitStructure_USARTx3.USART_Parity     = USART_Parity_No;                         // 设置USART校验位长度 无校验
        USART_InitStructure_USARTx3.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 设置USART硬件数据流控制
        USART_InitStructure_USARTx3.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;           // 设置USART模式 发送模式+接收模式
        USART_Init(USART3, &USART_InitStructure_USARTx3);                                       // 初始化结构体
        USART_Cmd(USART3, ENABLE);                                                              // 使能USART3模块运行
        
        // 配置 USARTx3 中断操作
        #if (STM32F103xxxx_USARTx3_ITEN_RXNE == 1)
        {
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);      // 启用USARTx3接受中断
        }
        #endif  // #if (STM32F103xxxx_USARTx3_ITEN_RXNE == 1)
        
        #if (STM32F103xxxx_USARTx3_ITEN_IDLE == 1)              
        {
            USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);      // 启用USARTx3空闲中断
        }
        #endif  // #if (STM32F103xxxx_USARTx3_ITEN_IDLE == 1)
        
        #if (STM32F103xxxx_USARTx3_ITEN_TC == 1)
        {
            USART_ITConfig(USART3, USART_IT_TC, ENABLE);        // 启用USARTx3发送完成中断
        }
        #endif  // #if (STM32F103xxxx_USARTx3_ITEN_TC == 1)
        
        // 配置 USARTx3 DMA操作
        #if (STM32F103xxxx_USARTx3_DMA_TX == 1)
        {
            USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);      // 启用USARTx3 DMA发送
        }
        #endif  // #if (STM32F103xxxx_USARTx3_DMA_TX == 1)
        
        #if (STM32F103xxxx_USARTx3_DMA_RX == 1)
        {
            USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);      // 启用USARTx3 DMA接收
        }
        #endif  // #if (STM32F103xxxx_USARTx3_DMA_RX == 1)
    }
    #endif  // #if (STM32F103xxxx_USARTx3 == 1)
    
    //----------------------------------------------------------------------------------------------
    // 配置 UARTx4 功能
    //----------------------------------------------------------------------------------------------
    #if (STM32F103xxxx_UARTx4 == 1)    
    {
        // 参数检查
        #if ((STM32F103xxxx_UARTx4_BAUD != 600)   \
        &&   (STM32F103xxxx_UARTx4_BAUD != 1200)  \
        &&   (STM32F103xxxx_UARTx4_BAUD != 2400)  \
        &&   (STM32F103xxxx_UARTx4_BAUD != 4800)  \
        &&   (STM32F103xxxx_UARTx4_BAUD != 9600)  \
        &&   (STM32F103xxxx_UARTx4_BAUD != 14400) \
        &&   (STM32F103xxxx_UARTx4_BAUD != 19200) \
        &&   (STM32F103xxxx_UARTx4_BAUD != 38400) \
        &&   (STM32F103xxxx_UARTx4_BAUD != 115200))
        {
            #error "There are errors in the STM32F103xxxx_UARTx4_BAUD"
        }
        #endif  // #if ((STM32F103xxxx_UARTx4_BAUD != 600)   \
                   &&   (STM32F103xxxx_UARTx4_BAUD != 1200)  \
                   &&   (STM32F103xxxx_UARTx4_BAUD != 2400)  \
                   &&   (STM32F103xxxx_UARTx4_BAUD != 4800)  \
                   &&   (STM32F103xxxx_UARTx4_BAUD != 9600)  \
                   &&   (STM32F103xxxx_UARTx4_BAUD != 14400) \
                   &&   (STM32F103xxxx_UARTx4_BAUD != 19200) \
                   &&   (STM32F103xxxx_UARTx4_BAUD != 38400) \
                   &&   (STM32F103xxxx_UARTx4_BAUD != 115200))
        
        #if (STM32F103xxxx_UARTx4_PORT_C10C11 != 1)
        {
            #error "There are errors in the STM32F103xxxx_UARTx4_PORT"
        }
        #endif  // #if (STM32F103xxxx_UARTx4_PORT_C10C11 != 1)
        
        // 配置GPIO引脚映射功能
        #if (STM32F103xxxx_UARTx4_PORT_C10C11 == 1)                 // 不映射
        {
        
        }
        #endif  // #if (STM32F103xxxx_UARTx4_PORT_C10C11 == 1)
        
        // 配置GPIO TXD引脚
        #if (STM32F103xxxx_UARTx4_PORT_C10C11 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_UARTx4_TXD;                     // 定义结构体变量

            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);               // 使能GPIOC模块时钟

            GPIO_InitStructure_UARTx4_TXD.GPIO_Pin   = GPIO_Pin_10;             // 设置GPIO引脚号
            GPIO_InitStructure_UARTx4_TXD.GPIO_Speed = GPIO_Speed_50MHz;        // 设置GPIO引脚速率
            GPIO_InitStructure_UARTx4_TXD.GPIO_Mode  = GPIO_Mode_AF_PP;         // 设置GPIO引脚输出类型 推挽输出类型
            GPIO_Init(GPIOC, &GPIO_InitStructure_UARTx4_TXD);                   // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_UARTx4_PORT_C10C11 == 1)
        
        // 配置GPIO RXD引脚
        #if (STM32F103xxxx_UARTx4_PORT_C10C11 == 1)
        {
            GPIO_InitTypeDef GPIO_InitStructure_UARTx4_TXD;                     // 定义结构体变量

            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);               // 使能GPIOC模块时钟

            GPIO_InitStructure_UARTx4_TXD.GPIO_Pin   = GPIO_Pin_11;             // 设置GPIO引脚号
            GPIO_InitStructure_UARTx4_TXD.GPIO_Speed = GPIO_Speed_50MHz;        // 设置GPIO引脚速率
            GPIO_InitStructure_UARTx4_TXD.GPIO_Mode  = GPIO_Mode_IN_FLOATING;   // 设置GPIO引脚输出类型 浮空输入类型
            GPIO_Init(GPIOC, &GPIO_InitStructure_UARTx4_TXD);                   // 初始化结构体
        }
        #endif  // #if (STM32F103xxxx_UARTx4_PORT_C10C11 == 1)

        // 配置 UARTx4 功能
        USART_InitTypeDef USART_InitStructure_UARTx4;                                           // 定义结构体变量
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);                                   // 使能UART4模块时钟
        
        USART_DeInit(UART4);
        
        USART_InitStructure_UARTx4.USART_BaudRate   = STM32F103xxxx_UARTx4_BAUD;                // 设置USART波特率
        USART_InitStructure_UARTx4.USART_WordLength = USART_WordLength_8b;                      // 设置USART数据位长度 8位数据位
        USART_InitStructure_UARTx4.USART_StopBits   = USART_StopBits_1;                         // 设置USART停止位长度 1位停止位
        USART_InitStructure_UARTx4.USART_Parity     = USART_Parity_No;                          // 设置USART校验位长度 无校验
        USART_InitStructure_UARTx4.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 设置USART硬件数据流控制
        USART_InitStructure_UARTx4.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;            // 设置USART模式 发送模式+接收模式
        USART_Init(UART4, &USART_InitStructure_UARTx4);                                         // 初始化结构体
        USART_Cmd(UART4, ENABLE);                                                               // 使能UART4模块运行
        
        // 配置 UARTx4 中断操作
        #if (STM32F103xxxx_UARTx4_ITEN_RXNE == 1)
        {
            USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);       // 启用UARTx4接受中断
        }
        #endif  // #if (STM32F103xxxx_UARTx4_ITEN_RXNE == 1)
        
        #if (STM32F103xxxx_UARTx4_ITEN_IDLE == 1)
        {
            USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);       // 启用UARTx4空闲中断
        }
        #endif  // #if (STM32F103xxxx_UARTx4_ITEN_IDLE == 1)
        
        #if (STM32F103xxxx_UARTx4_ITEN_TC == 1)
        {
            USART_ITConfig(UART4, USART_IT_TC, ENABLE);         // 启用UARTx4发送完成中断
        }
        #endif  // #if (STM32F103xxxx_UARTx4_ITEN_TC == 1)
        
        // 配置 UARTx4 DMA操作
        #if (STM32F103xxxx_UARTx4_DMA_TX == 1)
        {
            USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);       // 启用UARTx4 DMA发送
        }
        #endif  // #if (STM32F103xxxx_UARTx4_DMA_TX == 1)
        
        #if (STM32F103xxxx_UARTx4_DMA_RX == 1)
        {
            USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);       // 启用UARTx4 DMA接收
        }
        #endif  // #if (STM32F103xxxx_UARTx4_DMA_RX == 1)
    }
    #endif  // #if (STM32F103xxxx_UARTx4 == 1)
}

```
### STM32F103xxxx_USART.H

```c
//==================================================================================================
//  文件名称: STM32F103xxxx_USART.H
//  文件功能: USART-USART通信总线
//  文件说明: 无
//==================================================================================================

#ifndef __STM32F103xxxx_USART_H__
#define __STM32F103xxxx_USART_H__

//--------------------------------------------------------------------------------------------------
//  包含的头文件    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//--------------------------------------------------------------------------------------------------
#include "stm32f10x_usart.h"

//--------------------------------------------------------------------------------------------------
// 宏自定义声明     |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//--------------------------------------------------------------------------------------------------
//  兼容STM32系列单片机 GPIO端口宏定义
#define USARTx1                             USART1
#define USARTx2                             USART2
#define USARTx3                             USART3
#define UARTx4                              UART4
#define UARTx5                              UART5

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define STM32F103xxxx_USARTx1                       1       // USARTx1 开关开启  取值范围 - 0为禁用 1为启用  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx1_BAUD                  115200  // USARTx1 波特率  取值范围 - 600/1200/2400/4800/9600/19200/38400/115200
#define STM32F103xxxx_USARTx1_PORT_A09A10           1       // USARTx1 引脚设置 PA9 - TxD PA10 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx1_PORT_B06B07           0       // USARTx1 引脚设置 PB6 - TxD PB7 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx1_ITEN_RXNE             0       // USARTx1 接受中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx1_ITEN_IDLE             0       // USARTx1 空闲中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx1_ITEN_TC               0       // USARTx1 发送完成中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx1_DMA_TX                0       // USARTx1 DMA发送 开关  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx1_DMA_RX                0       // USARTx1 DMA接受 开关  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx1_DMA_TX_CUNT           1024    // USARTx1 DMA发送 字节数量
#define STM32F103xxxx_USARTx1_DMA_RX_CUNT           1024    // USARTx1 DMA接受 字节数量
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define STM32F103xxxx_USARTx2                       0       // USARTx2 开关开启  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx2_BAUD                  9600    // USARTx2 波特率  取值范围 - 600/1200/2400/4800/9600/19200/38400/115200
#define STM32F103xxxx_USARTx2_PORT_A02A03           1       // USARTx2 引脚设置 PA2 - TxD PA3 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx2_PORT_D05D06           0       // USARTx2 引脚设置 PD5 - TxD PD6 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx2_ITEN_RXNE             0       // USARTx2 接受中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx2_ITEN_IDLE             0       // USARTx2 空闲中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx2_ITEN_TC               0       // USARTx2 发送完成中断操作使能  取值范围 - 0为禁用 1为启用
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define STM32F103xxxx_USARTx3                       0       // USARTx3 开关开启  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx3_BAUD                  115200  // USARTx3 波特率  取值范围 - 600/1200/2400/4800/9600/19200/38400/115200
#define STM32F103xxxx_USARTx3_PORT_B10B11           1       // USARTx3 引脚设置 PB10 - TxD PB11 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx3_PORT_C10C11           0       // USARTx3 引脚设置 PC10 - TxD PC11 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx3_PORT_D08D09           0       // USARTx3 引脚设置 PD8 - TxD PD9 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx3_ITEN_RXNE             0       // USARTx3 接受中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx3_ITEN_IDLE             0       // USARTx3 空闲中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx3_ITEN_TC               0       // USARTx3 发送完成中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_USARTx3_DMA_TX                0       // USARTx3 DMA发送 开关开启  取值范围 - 0为禁用 1为使能
#define STM32F103xxxx_USARTx3_DMA_RX                0       // USARTx3 DMA接受 开关开启  取值范围 - 0为禁用 1为使能
#define STM32F103xxxx_USARTx3_DMA_TX_COUNT          1024    // USARTx3 DMA发送 字节数量
#define STM32F103xxxx_USARTx3_DMA_RX_COUNT          1024    // USARTx3 DMA接受 字节数量
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define STM32F103xxxx_UARTx4                        0       // UARTx4 开关开启  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_UARTx4_BAUD                   115200  // UARTx4 波特率  取值范围 - 600/1200/2400/4800/9600/19200/38400/115200
#define STM32F103xxxx_UARTx4_PORT_C10C11            1       // UARTx4 引脚设置 PC10 - TxD PC11 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_UARTx4_ITEN_RXNE              1       // UARTx4 接受中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_UARTx4_ITEN_IDLE              0       // UARTx4 空闲中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_UARTx4_ITEN_TC                0       // UARTx4 发送完成中断操作使能  取值范围 - 0为禁用 1为启用
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define STM32F103xxxx_UARTx5                        0       // UARTx5 开关开启  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_UARTx5_BAUD                   115200  // UARTx5 波特率  取值范围 - 600/1200/2400/4800/9600/19200/38400/115200
#define STM32F103xxxx_UARTx5_PORT_C12D02            1       // UARTx5 引脚设置 PC10 - TxD PC11 - RxD  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_UARTx5_ITEN_RXNE              1       // UARTx5 接受中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_UARTx5_ITEN_IDLE              0       // UARTx5 空闲中断操作使能  取值范围 - 0为禁用 1为启用
#define STM32F103xxxx_UARTx5_ITEN_TC                0       // UARTx5 发送完成中断操作使能  取值范围 - 0为禁用 1为启用

//--------------------------------------------------------------------------------------------------
//  数据变量定义    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  函数调用声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//--------------------------------------------------------------------------------------------------
//  USART 预配置
void USART_Configure(void);

//  USART 发送字符、字符串
void USART_SendChar(USART_TypeDef* USARTx, unsigned char Char);
void USART_SendString(USART_TypeDef* USARTx, unsigned char *String);

//  USART 接受字符
unsigned char USART_ReceiveChar(USART_TypeDef* USARTx);

//  USART 发送数组 
void USART_SendArray(USART_TypeDef* USARTx, unsigned char *Array, unsigned char Count);

//  USART 发送回车换行
void USART_SendLine(USART_TypeDef* USARTx);

//  USART 发送数字 
void USART_SendNumber_SignedInteger(USART_TypeDef* USARTx, signed short int Number, unsigned char Count);
void USART_SendNumber_UnsignedInteger(USART_TypeDef* USARTx, unsigned short int Number, unsigned char Count);
void USART_SendNumber_Float(USART_TypeDef* USARTx, float Number, unsigned char Count1, unsigned char Count2);
void USART_SendNumber_Binary(USART_TypeDef* USARTx, int Number, unsigned char Count);
                                                    
#endif // #ifndef __STM32F103xxxx_USART_H__

```
### BSP_TEST.C

```c
#include "bsp_test.h"

/**
  * @brief  TFT_LCD_TEST LCD测试函数
  * @param  none
  * @retval 无
 */
  
void  TFT_LCD_TEST(void)
{
  

	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,WHITE);

  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	/********显示字符串示例*******/
  ILI9341_DispStringLine_EN(LINE(0),"dele 3.2 inch LCD:");
  ILI9341_DispStringLine_EN(LINE(1),"Image resolution:240x320");
  ILI9341_DispStringLine_EN(LINE(2),"ILI9341 TFT_LCD_TEST driver ");
  ILI9341_DispStringLine_EN(LINE(3),"www.delehub.top By wangzixuan");
  ILI9341_DispStringLine_EN(LINE(4),"2020-10-02 17:15 TFT_LCD_TEST");
  
  LCD_SetFont(&Font8x16);
  /* 画圆 */
  
  LCD_ClearLine(LINE(5));	/* 清除单行文字 */
	LCD_SetTextColor(GREEN);
	
  ILI9341_DispStringLine_EN(LINE(5),"TFT_LCD_DrawCir_TEST :");

	LCD_SetTextColor(RED);
  ILI9341_DrawCircle(100,200,20,1);
	
	LCD_SetTextColor(GREEN);
  ILI9341_DrawCircle(100,200,40,0);
	
	LCD_SetTextColor(BLUE);
	ILI9341_DrawCircle(100,180,60,0);

  Delay(0xFFFFFF);
  
  ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */

}

void Usart_test(void)
{
  USART_SendString(USART1 , "2020-10-02 18:00 USART1 Test");
  USART_SendLine(USART1);
  USART_SendNumber_SignedInteger(USART1,1234,4);
  USART_SendLine(USART1);
  USART_SendNumber_UnsignedInteger(USART1,12445,5);
  USART_SendLine(USART1);
  USART_SendNumber_Float(USART1,1234.89,7,8);
  USART_SendLine(USART1);
  USART_SendString(USART1 , "www.delehub.top  By wang zi xuan ");
  USART_SendLine(USART1);

}
/**
  * @brief  简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}
```

## MAIN.C
```c
/**
  ******************************************************************************
  * @file    main.c
  * 实验平台:野火 F103-霸道 STM32 开发板 
  ******************************************************************************
  * 第三方库文件导入  START THE FILE
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "bsp_led.h"  
#include "bsp_key.h" 
#include "bsp_beep.h"
#include "bsp_exit.h"
#include "timer.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "bsp_test.h"
#include "STM32F103xxxx_USART.H"  
/******************************************************************************
END THE FILE
  * 第三方库文件导入*********************************************************/ 

void HardWare_Iint(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  LED_Iint();
  Key_Iint();
  Beep_Init();
  //TIM4_Int_Init(9999,7199);
  ILI9341_Init();
  USART_Configure();
  
}
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	
	/* 硬件端口初始化 */
	HardWare_Iint();
  	ILI9341_GramScan ( 6 );
  	while(1)
  	{
	    TFT_LCD_TEST();
	    Usart_test();
 	} 
}
```
### 串口助手打印效果
![串口助手打印效果](https://img-blog.csdnimg.cn/20201002210122772.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
### TFT_LCD 测试效果
![TFT_LCD 测试效果](https://img-blog.csdnimg.cn/20201002210353427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)

# 野火F1开发板STM32案例-外部中断(按键)使用
## 硬件平台
1. 野火STM32F103ZET6 霸道V2开发板
2. 正点原子F1系列开发板
![STM32产品](https://img-blog.csdnimg.cn/20201002210908329.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
## 软件平台
1. Keil MDK 5.31
2. 串口调试助手

工程下载<https://download.csdn.net/download/weixin_43599390/12910397>

## 中断相关概念
```c
中断配置寄存器
# 配置中断时，使用ISER、 ICER 和 IP 寄存器，
## ISER 是中断使能寄存器
## ICER 是中断清除寄存器
## IP 是中断优先级寄存器

中断优先级
1. 数值越小，优先级越高
2. STM32F103 中只使用4位，高4位有效。
3. 用于表达优先级的高 4 位又被分组成抢占式优先级和响应优先级，称为“亚优先级”或“副优先级”
4. 每个中断源都需要被指定这两种优先级。

中断配置步骤
1.使能外设中断
6. 设置中断优先级分组
初始化 NVIC_InitTypeDef 结构体，设置抢占优先级和响应优先级，使能中断请求。
typedef struct
{
     uint8_t NVIC_IRQChannel; //中断源
     uint8_t NVIC_IRQChannelPreemptionPriority; //抢占优先级
     uint8_t NVIC_IRQChannelSubPriority; //响应优先级
     FunctionalState NVIC_IRQChannelCmd; //中断使能或失能
 } NVIC_InitTypeDef;


NVIC_IRQChannel 中断源:中断源的设置，不同的外设中断，中断源不一样
NVIC_IRQChannelPreemptionPriority  //抢占优先级

NVIC_IRQChannelSubPriority  //响应优先级
NVIC_IRQChannelCmd //中断使能或失能:使能配置为 ENABLE，失能配置为 DISABLE。
```
```c
NVIC_IRQChannel   //中断源
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */

/******  STM32 specific Interrupt Numbers *********************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
  TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
  RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
  DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
  DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
  DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
  DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
  DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
  DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
  DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */

```
### STM32F103  中断向量表
![ 中断向量表1](https://img-blog.csdnimg.cn/2020100315141155.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
![ 中断向量表2](https://img-blog.csdnimg.cn/20201003151410982.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
###  EXTI外部中断
```c
1. STM32F10x 外部中断/事件控制器（EXTI）包含多达 20 个用于产生事件/中断请求的边沿检测器。
2. EXTI 的每根输入线都可单独进行配置，以选择类型（中断或事件）和相应的触发事件（上升沿触发、下降沿触发或边沿触发）
还可独立地被屏蔽。
3. EXIT功能:产生中断;产生事件.
```
### STM32 EXIT框架图
![STM32 EXIT框架图](https://img-blog.csdnimg.cn/20201003151626597.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
## EXTI外部中断步骤
1. 使能I0口时钟，配置I0口模式为输入
2. 开启AFIO时钟，设置I0口与中断线的映射关系
3. 配置中断分组（NVIC），使能中断
4. 初始化EXTI，选择触发方式
5. 编写EXTI中断服务函数
#### GPIO引脚配置（本文中和外部中断bsp_exit.h一起进行初始化，可以分开进行配置相关函数）
(一般配置过程类似，无论使用什么外设，UART,IIC,SPI,ADC 一般情况都要GPIO初始化)
```c
//定义GPIO初始化结构体
//使能 GPIO时钟 (时钟分频)
//设置相关外部设备的对应引脚
//设置GPIO端口模式(GPIO输入 GPIO输出)
//设置生效(GPIO端口启动生效)
# GPIO输入模式
void initKey(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                  //定义GPIO初始化结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能 GPIO时钟 (时钟分频)
    GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN|KEY_DOWN_PIN;//设置相关外部设备的对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //设置上拉输入 //设置GPIO端口模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);			      //设置生效(GPIO端口启动生效)
    
}

# GPIO输出模式
void initLED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                  //定义GPIO初始化结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		      //设置对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //设置推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //工作速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);                //设置生效
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);				      //默认低电平，LED不
}
```

#### 配置嵌套向量中断控制器NVIC步骤
1. 定义NVIC结构体        NVIC_InitTypeDef 结构体
```c
typedef struct
{
	uint8_t NVIC_IRQChannel; //中断源
	uint8_t NVIC_IRQChannelPreemptionPriority; //抢占优先级
	uint8_t NVIC_IRQChannelSubPriority; //响应优先级
	FunctionalState NVIC_IRQChannelCmd; //中断使能或失能
} NVIC_InitTypeDef;
```
2. 配置NVIC为优先级组
![优先级组](https://img-blog.csdnimg.cn/20201003160403277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
```c
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
```
3. 配置中断源
```c
NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
// #define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
// #define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
// #define KEY1_INT_EXTI_LINE         EXTI_Line0
// #define KEY1_INT_EXTI_IRQ          EXTI0_IRQn
```
4. 配置抢占优先级
```c
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
```
5. 配置子优先级 
```c
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
```
6. 使能中断通道
```c
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
```
7. 初始化NVIC
```c
NVIC_Init(&NVIC_InitStructure);
```
#### 配置 EXTI中断口，并设置中断优先级步骤流程

```c

1. 定义GPIO_InitTypeDef EXTI_InitTypeDef 结构体
GPIO_InitTypeDef GPIO_InitStructure; 
EXTI_InitTypeDef EXTI_InitStructure;

typedef struct
{
  uint16_t GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_pins_define */

  GPIOSpeed_TypeDef GPIO_Speed;  /*!< Specifies the speed for the selected pins.
                                      This parameter can be a value of @ref GPIOSpeed_TypeDef */

  GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;

typedef struct
{
  uint32_t EXTI_Line;               /*!< Specifies the EXTI lines to be enabled or disabled.
                                         This parameter can be any combination of @ref EXTI_Lines */
   
  EXTIMode_TypeDef EXTI_Mode;       /*!< Specifies the mode for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

  EXTITrigger_TypeDef EXTI_Trigger; /*!< Specifies the trigger signal active edge for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

  FunctionalState EXTI_LineCmd;     /*!< Specifies the new state of the selected EXTI lines.
                                         This parameter can be set either to ENABLE or DISABLE */ 
}EXTI_InitTypeDef;

```
![GPIO结构体](https://img-blog.csdnimg.cn/20201003162222876.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
![中断事件结构体](https://img-blog.csdnimg.cn/20201003162222867.png#pic_center)

```c
/*开启按键GPIO口的时钟*/
RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
/* 配置 NVIC 中断*/
NVIC_Configuration();
```
##### GPIO引脚配置
```c
/*--------------------------KEY1配置-------------------------*/
/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
/*------------------------KEY2配置-----------------------------*/
  /* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);
```
##### 中断配置
1. 中断信号源 
2. EXIT模式
3. 外部中断触发类型
4. 使能中断EXIT
5. 中断初始化
```c
/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
```

### BSP_EXIT.H
```c
//引脚定义
#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY1_IRQHandler            EXTI0_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOC
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_13
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource13
#define KEY2_INT_EXTI_LINE         EXTI_Line13
#define KEY2_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY2_IRQHandler            EXTI15_10_IRQHandler

```
### BSP_EXIT.C
#### 配置嵌套向量中断控制器NVIC
```c
#include “bsp_exit.h”
/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}
```
#### 配置 IO为EXTI中断口，并设置中断优先级

```c
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
												
	/* 配置 NVIC 中断*/
	NVIC_Configuration();
	
/*--------------------------KEY1配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  /*--------------------------KEY2配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}
```
### 中断服务函数

```c
#include "stm32f10x_it.h"
#include "bsp_test.h"
/**
******************************************************************************
  * @file    stm32 用户自定义外部中断
  * @author  dele
  * @version V1.0
  * @date    2020-10-03
  * @brief   按键测试（中断模式/EXTI模式）
  * 实验平台:野火 F103-霸道 STM32 开发板 
  ******************************************************************************
  */ 
void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		// LED1 取反		
		LED1_TOGGLE;
	    BSP_KEY1_EXIT_LCDTEST(); 
	    USART_SendString(USART1,"KEY1 EXIT www.delehub.top");
	    USART_SendLine(USART1);
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);  
     
	}  
}

void KEY2_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		// LED2 取反		
		LED2_TOGGLE
	    BSP_KEY2_EXIT_LCDTEST(); 
	    USART_SendString(USART1,"KEY2 EXIT 2020-10-03 14:50");  
	    USART_SendLine(USART1);
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
      
	}  
}
```

#### 按键外部中断测试函数

```c
void BSP_KEY1_EXIT_LCDTEST(void)
{
  LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,WHITE);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  ILI9341_DispStringLine_EN(LINE(0),"2020-10-03 14:30 BSP_EXIT_TEST");
  ILI9341_DispStringLine_EN(LINE(1),"KEY1 EXIT TEST");
  LCD_SetTextColor(RED);
  ILI9341_DrawCircle(100,200,20,1);
}

void BSP_KEY2_EXIT_LCDTEST(void)
{
  LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,WHITE);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  ILI9341_DispStringLine_EN(LINE(3),"2020-10-03 14:30 DELEHUB");
  ILI9341_DispStringLine_EN(LINE(4),"KEY2 EXIT TEST");
  LCD_SetTextColor(GREEN);
  ILI9341_DrawCircle(100,200,40,0);
}
```

#### MAIN.C
```c
/****************************************************************
  * @file    main.c
  * 实验平台:野火 F103-霸道 STM32 开发板  ***************************************************************** 第三方库文件导入  START THE FILE
 ***************************************************************/ 
#include "stm32f10x.h"
#include "bsp_led.h"  
#include "bsp_key.h" 
#include "bsp_beep.h"
#include "bsp_exit.h"
#include "timer.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "bsp_test.h"
#include "STM32F103xxxx_USART.H"  
/******************************************************************************
END THE FILE
  * 第三方库文件导入*********************************************************/ 
void HardWare_Iint(void)
{
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  EXTI_Key_Config();
  LED_Iint();
  Key_Iint();
  Beep_Init();
  //TIM4_Int_Init(9999,7199);
  ILI9341_Init();
  USART_Configure();
  
}
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	
	/* 硬件端口初始化 */
	HardWare_Iint();
	while(1)
	{
	}
}
```
### 串口打印效果
![串口打印效果](https://img-blog.csdnimg.cn/20201003153440133.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)

### TFT显示效果
![TFT显示效果key2](https://img-blog.csdnimg.cn/20201003153501455.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
![TFT显示效果key2](https://img-blog.csdnimg.cn/20201003153501430.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzU5OTM5MA==,size_16,color_FFFFFF,t_70#pic_center)
### 博客地址<https://delehub.vercel.app/>


