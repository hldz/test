#ifndef __MAIN_H
#define __MAIN_H



#include  <stdio.h>
#include  <string.h>
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp.h"
#include "m35.h"
#include "delay.h"




typedef struct
{
  FlagStatus flay;
  __IO u8    cnt;
  __IO u8    M35Rebuf[255];
}M35RbufTypeDef;


typedef struct
{
  __IO u16 result;
  __IO u8  FilterCnt;
}FilterTypeDef;

extern __IO u8 flag5ms;
extern  M35RbufTypeDef M35REBUFStructure;
//extern __IO float httpdata;
//extern __IO u8 HTTPPostData[30];
//extern  u8 Datajing[30];
//extern  u8 Datawei[30];
//extern DHT11_Data_TypeDef DHT11_Data;

extern __IO u8 flagOnceTxd;  //���η�����ɱ�־����������һ���ֽ�
extern __IO u8 cmdArrived;   //������־�������յ���λ���·�������
extern __IO u8 cntRxd;
extern __IO u8 bufRxd[40]; //���ڽ��ջ�����

void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);




#endif







