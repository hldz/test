

#include "main.h"


/**
  * @brief  printf函数配置。
  * 
  * 
  */
#if 1
#pragma import(__use_no_semihosting)             
                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;         
int _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{   
  while((USART1->SR&0x40) == 0);
  USART1->DR = (u8)ch;      
  return ch;
}
#endif

/**
  * @brief  M35涉及串口配置.
  * @param  Baud:串口波特率
  * @retval None   csic.taobao.com
  */
void M35_USARTx_Config(u32 Baud)
{
  u8 data;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APBxPeriph_M35_USART_IO | RCC_APBxPeriph_M35_USART,ENABLE);
  				 
  GPIO_InitStructure.GPIO_Pin = M35_USART_TXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(M35_USART_IO,&GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = M35_USART_RXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(M35_USART_IO,&GPIO_InitStructure);


  USART_InitStructure.USART_BaudRate = Baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_Init(M35_USART,&USART_InitStructure);
  data = data;
  data = M35_USART->DR;
  data = M35_USART->SR;
  USART_ITConfig(M35_USART,USART_IT_RXNE,ENABLE);
  USART_Cmd(M35_USART,ENABLE);
}


/**
  * @brief  串口一涉及串口配置.
  * @param  Baud:串口波特率
  * @retval None  csic.taobao.com
  */
void USART1_Config(u32 Baud)
{
  u8 data;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
  				 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);


  USART_InitStructure.USART_BaudRate = Baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	data = data;
  data = M35_USART->DR;
  data = M35_USART->SR;
  USART_Init(USART1,&USART_InitStructure);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  USART_Cmd(USART1,ENABLE);
}

/**
  * @brief  LED引脚初始化.
  * @param  None
  * @retval None  csic.taobao.com
  */
void LED_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APBxPeriph_RELAY_IO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_LED_IO,ENABLE);
  GPIO_InitStructure.GPIO_Pin = RELAY_A;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RELAY_IO,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = Relay_B;
  GPIO_Init(RELAY_IO,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LED_D7;
  GPIO_Init(LED_IO,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LED_D8;
  GPIO_Init(LED_IO,&GPIO_InitStructure);

  LED_WriteBit(RELAY_IO, RELAY_A | Relay_B , Bit_SET);
	LED_WriteBit(LED_IO, LED_D7 | LED_D8, Bit_SET);
}
/**
  * @brief  4个LED闪烁.
  * @param  time: 闪烁次数
  * @retval None   csic.taobao.com
  */
void LED4_ONOFF(u8 time)
{
  u8 i;
  for (i = 0;i < time; i++)
  { 
    LED_WriteBit(RELAY_IO, RELAY_A | Relay_B | LED_D7 | LED_D8, Bit_RESET);
    delay_ms(500);
    LED_WriteBit(RELAY_IO, RELAY_A | Relay_B | LED_D7 | LED_D8, Bit_SET);
    delay_ms(500);
  }
}
/**
  * @brief  清除指定的内存空间和大小
  * @param  ram: 内存空间的首地址 n:要清除空间的大小
  * @retval None	 csic.taobao.com
  */
void ClearRAM(u8* ram,u32 n)
{
  u32 i;
  for (i = 0;i < n;i++)
  {
    ram[i] = 0x00;
  }
}

/**
  * @brief  定时器2初始化
  * @param  None
  * @retval None  csic.taobao.com
  */
  //((1+TIM_Prescaler )/72M)*(1+TIM_Period )=((1+2)/72M)*(1+65535)=0.00273秒=366.2Hz */
void TIM2_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  //GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
  TIM_TimeBaseStructure.TIM_Period = 1799;
  TIM_TimeBaseStructure.TIM_Prescaler = 199;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ARRPreloadConfig(TIM2, DISABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
}
/**
  * @brief  定时器3初始化
  * @param  None
  * @retval None  csic.taobao.com
  */
  //((1+TIM_Prescaler )/36M)*(1+TIM_Period )
void TIM3_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  TIM_TimeBaseStructure.TIM_Period = 179;
  TIM_TimeBaseStructure.TIM_Prescaler = 199;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ARRPreloadConfig(TIM3, DISABLE);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
}
/**
  * @brief  配置定时器2中断
  * @param  None
  * @retval None   csic.taobao.com
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   
  NVIC_InitStructure.NVIC_IRQChannel = M35_USART_IRQn;;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  DMA初始化
  * @param  None
  * @retval None  csic.taobao.com
  */
u32 ADCConvertedValue;
void DMA_init(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);  
}
/**
  * @brief  ADC初始化
  * @param  None
  * @retval None  csic.taobao.com
  */
void ADC_init(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);
  ADC_TempSensorVrefintCmd(ENABLE); 
  ADC_DMACmd(ADC1, ENABLE); 
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
}
/**
  * @brief  采样值滤波
  * @param  None
  * @retval None  csic.taobao.com
  */
u16 ADC_Filter(void)
{
  u16 result=0;
  u8 i;
  for(i=16;i>0;i--)
  {
    result += ADCConvertedValue;
  }
  return result/16;
}

/**
  * @brief  输出调试信息及清空缓存
  * @param  printf 串口一输出数据
  * @retval None  csic.taobao.com
  */

void PrintfClear(void)
{
	printf((char*)(&M35REBUFStructure.M35Rebuf));
	ClearRAM((u8*)(&M35REBUFStructure.M35Rebuf),255);		
}



