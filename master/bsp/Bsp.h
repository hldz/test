
#ifndef __BSP_H
#define __BSP_H



/********************************/
#define ADC1_DR_Address     0x4001244C
///********启动控制引脚定义********/
//#define RCC_APBxPeriph_M35_PWRKEY  RCC_APB2Periph_GPIOB
//#define RCC_APBxPeriph_M35_STATUS  RCC_APB2Periph_GPIOB

//#define M35_PWRKEY_IO     GPIOB
//#define M35_STATUS_IO     GPIOB

//#define M35_PWRKEY_PIN    GPIO_Pin_8
//#define M35_STATUS_PIN    GPIO_Pin_9

/********M35串口3控制引脚定义********/
#define RCC_APBxPeriph_M35_USART_IO  RCC_APB2Periph_GPIOA
#define RCC_APBxPeriph_M35_USART	 RCC_APB1Periph_USART2

#define M35_USART_IO     GPIOA
#define M35_USART_TXD    GPIO_Pin_2
#define M35_USART_RXD	 	 GPIO_Pin_3
#define M35_USART        USART2
#define M35_USART_IRQn	 USART2_IRQn
/**********LED引脚定义***********/
#define RCC_APBxPeriph_RELAY_IO   RCC_APB2Periph_GPIOA
#define RCC_APBxPeriph_LED_IO   RCC_APB2Periph_GPIOB

#define RELAY_IO      GPIOA
#define LED_IO      	GPIOB
#define RELAY_A      	GPIO_Pin_5
#define Relay_B      	GPIO_Pin_4						
#define LED_D7      	GPIO_Pin_6
#define LED_D8      	GPIO_Pin_7

#define LED_WriteBit      GPIO_WriteBit
/*********BSP.C函数定义**********/
void M35_GPIOInit(void);
void M35_USARTx_Config(u32 Baud);
void USART1_Config(u32 Baud); //串口一函数
void LED_init(void);
void LED4_ONOFF(u8 time);
void ClearRAM(u8* ram,u32 n);
void DLED_init(void);
void TIM2_init(void);
void TIM3_init(void);
void NVIC_Configuration(void);
void DMA_init(void);
void ADC_init(void);
u16 ADC_Filter(void);

extern u32 ADCConvertedValue;
void PrintfClear(void);
#endif

