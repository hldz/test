/**
  ******************************************************************************
  * @file    USART/Interrupt/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t TxBuffer1[];
extern uint8_t TxBuffer2[];
extern uint8_t RxBuffer1[];
extern uint8_t RxBuffer2[];
extern __IO uint8_t TxCounter1;
extern __IO uint8_t TxCounter2;
extern __IO uint8_t RxCounter1;
extern __IO uint8_t RxCounter2;
extern uint8_t NbrOfDataToTransfer1;
extern uint8_t NbrOfDataToTransfer2;
extern uint8_t NbrOfDataToRead1;
extern uint8_t NbrOfDataToRead2;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/



/*TIM2 INTERRUPT PERIOD 5MS*/

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        flag5ms++;
    }
}


void USART1_IRQHandler(void)
{
    /*
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
    			USART_SendData(USART1, USART_ReceiveData(USART1));
    }


    	u8 data;
    	data = data;
    	if(USART_GetFlagStatus(M35_USART, USART_FLAG_ORE)!=RESET)
    	{
    		data = M35_USART->DR;
    	}
    	if(USART_GetFlagStatus(M35_USART,USART_FLAG_RXNE)!=RESET)
    	{
    		M35REBUFStructure.M35Rebuf[M35REBUFStructure.cnt++] = M35_USART->DR;
    		USART_ITConfig(M35_USART,USART_IT_IDLE,ENABLE);
    	}
    	if(USART_GetFlagStatus(M35_USART,USART_FLAG_IDLE)!=RESET)
    	{
    		USART_ITConfig(M35_USART,USART_IT_IDLE,DISABLE);
    		M35REBUFStructure.flay = SET;
    		M35REBUFStructure.cnt = 0;
    	}
    	*/
}


void USART2_IRQHandler(void)
{
    u8 data;
    data = data;
    if(USART_GetFlagStatus(M35_USART, USART_FLAG_ORE)!=RESET)
    {
        data = M35_USART->DR;
    }
    if(USART_GetFlagStatus(M35_USART,USART_FLAG_RXNE)!=RESET)
    {
        M35REBUFStructure.M35Rebuf[M35REBUFStructure.cnt++] = M35_USART->DR;
        USART_ITConfig(M35_USART,USART_IT_IDLE,ENABLE);
    }
    if(USART_GetFlagStatus(M35_USART,USART_FLAG_IDLE)!=RESET)
    {
        USART_ITConfig(M35_USART,USART_IT_IDLE,DISABLE);
        M35REBUFStructure.flay = SET;
        M35REBUFStructure.cnt = 0;
    }
}




void USART3_IRQHandler(void)
{

    if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=RESET)
    {
        if (cntRxd < sizeof(bufRxd)) //接收缓冲区尚未用完时，
        {
            bufRxd[cntRxd++] = USART_ReceiveData(USART3); //保存接收字节，并递增计数器
        }
        USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
    }

    if(USART_GetFlagStatus(USART3,USART_FLAG_IDLE)!=RESET)
    {
        USART_ITConfig(USART3,USART_IT_IDLE,DISABLE);
        cmdArrived = 1; //设置命令到达标志
    }
		
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
