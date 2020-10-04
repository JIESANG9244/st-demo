/**
  ******************************************************************************
  * @file    main.c
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  ******************************************************************************
  * ���������ļ�����  START THE FILE
  ******************************************************************************
  */ 
//������
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "delay.h"
//��չ��
#include "multi_button.h"//��������

/********************************************************************************
��������
*********************************************************************************/
struct Button button1;
//  ���Ŷ���
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_0

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY2_GPIO_PORT    GPIOC		   
#define    KEY2_GPIO_PIN		 GPIO_Pin_13

/********************************************************************************
��������
*********************************************************************************/
void KEY_Init(void) //IO��ʼ��
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	//ѡ�񰴼�KEY1������ 
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}


//����״̬��ȡ�ӿ�  ��������ģʽ ReadInputDataBit
uint8_t read_button1_GPIO() 
{
    return GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
}

//����1�����¼��ص�����
void btn1_press_down_Handler(void* btn)
{
    printf("---> key1 press down! <---\r\n");
}

//����1�ɿ��¼��ص�����
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
  * @param  ��
  * @retval ��
  */ 

void HardWare_Iint(void)
{
  SysTick_Init();
  delay_init();
  KEY_Init();
    
}
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */ 
int main(void)
{	
	/* Ӳ���˿ڳ�ʼ�� */
	HardWare_Iint();
  
  printf("MultiButton Test...\r\n");
	
  //��ʼ����������
	button_init(&button1, read_button1_GPIO, 0);
  
  //ע�ᰴť�¼��ص�����

  button_attach(&button1, PRESS_DOWN,       button_callback);
  button_attach(&button1, PRESS_UP,         button_callback);

  
  //��������
  button_start(&button1);
  while(1)
  
  {
        
        button_ticks();
        delay_ms(5);
  } 

}








