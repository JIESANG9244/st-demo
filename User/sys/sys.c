/********************************Copyright (c)**********************************\
**----------------------------------�ļ���Ϣ------------------------------------
** �ļ�����: sys.c
** ������Ա: temp_duan
** ��������: 2019-07-21
** �ĵ�����: 
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "sys.h"
void WFI_SET(void)
{
    __ASM volatile("wfi");
}
//�ر������ж�
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}

#if 0
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr)
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
#endif
