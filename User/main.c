/**
  ******************************************************************************
  * @file    main.c
  * 实验平台:野火 F103-霸道 STM32 开发板 
  ******************************************************************************
  * 第三方库文件导入  START THE FILE
  ******************************************************************************
  */ 
//基本库
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "delay.h"
//扩展库
#include "multi_button.h"//按键驱动

/********************************************************************************
变量定义
*********************************************************************************/
struct Button button1;
//  引脚定义
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_0

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY2_GPIO_PORT    GPIOC		   
#define    KEY2_GPIO_PIN		 GPIO_Pin_13

/********************************************************************************
函数部分
*********************************************************************************/
void KEY_Init(void) //IO初始化
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	//选择按键KEY1的引脚 
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// 设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}


//按键状态读取接口  按键输入模式 ReadInputDataBit
uint8_t read_button1_GPIO() 
{
    return GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
}

//按键1按下事件回调函数
void btn1_press_down_Handler(void* btn)
{
    printf("---> key1 press down! <---\r\n");
}

//按键1松开事件回调函数
void btn1_press_up_Handler(void* btn)
{
    printf("***> key1 press up! <***\r\n");
}

void button_callback(void *button)
{
    uint32_t btn_event_val; 
    
    btn_event_val = get_button_event((struct Button *)button); 
    
    switch(btn_event_val)
    {
      case PRESS_DOWN:
          printf("---> key1 press down! <---\r\n"); 
      break; 

      case PRESS_UP: 
          printf("***> key1 press up! <***\r\n");
      break; 

      case PRESS_REPEAT: 
          printf("---> key1 press repeat! <---\r\n");
      break; 

      case SINGLE_CLICK: 
          printf("---> key1 single click! <---\r\n");
      break; 

      case DOUBLE_CLICK: 
          printf("***> key1 double click! <***\r\n");
      break; 

      case LONG_PRESS_START: 
          printf("---> key1 long press start! <---\r\n");
      break; 

      case LONG_PRESS_HOLD: 
          printf("***> key1 long press hold! <***\r\n");
      break; 
    }
}


/**
  * @brief  HardWare_Iint
  * @param  无
  * @retval 无
  */ 

void HardWare_Iint(void)
{
  SysTick_Init();
  delay_init();
  KEY_Init();
    
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
  
  printf("MultiButton Test...\r\n");
	
  //初始化按键对象
	button_init(&button1, read_button1_GPIO, 0);
  
  //注册按钮事件回调函数

  button_attach(&button1, PRESS_DOWN,       button_callback);
  button_attach(&button1, PRESS_UP,         button_callback);

  
  //启动按键
  button_start(&button1);
  while(1)
  
  {
        
        button_ticks();
        delay_ms(5);
  } 

}








