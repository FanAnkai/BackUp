/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

extern volatile uint8_t Flag_1s;
extern volatile float Scatter_Signal;
extern volatile float Scatter_Signal_temp;
extern volatile float tmcu;
volatile uint16_t systick_cnt=0;
volatile float sum=0;
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	GPIOA->ODR ^= (1<<11);//SYNC
	GPIOB->ODR ^= (1<<4);//PWM
	
	systick_cnt++;
	ADS7822_Fetch();
	sum=sum+Scatter_Signal_temp;
	if(systick_cnt>399)
	{
		Flag_1s=1;
		systick_cnt = 0;
		Scatter_Signal=sum/400;
		sum=0;
		tmcu = get_temperature();
	}
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
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

extern uint8_t aRxBuffer[100];
extern uint8_t RxCounter;
extern uint8_t ReceiveState;
/**
  * @brief  This function handles USARTy global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	if((USART1->ISR&(1<<5))==(1<<5))// 如果接收到数据
	{
		aRxBuffer[RxCounter++] = USART_ReceiveData(USART1);
	}
	else if((USART1->ISR&(1<<11))==(1<<11))// 如果发生超时中断
	{
		USART1->ICR|=(1<<11);//清除超时中断
//		USART_SendData(USART1, RxCounter);
		ReceiveState = 1;//标志接收完成
//		RxCounter = 0;
	}
//	else if((USART1->ISR&(1<<4))==(1<<4))//如果发生IDEL中断
//	{
//		USART1->ICR|=(1<<4);//清除IDEL中断
//		USART_SendData(USART1, RxCounter);
//		ReceiveState = 1;//标志接收完成
//		RxCounter = 0;
//	}
}
//void USART1_IRQHandler(void)
//{
//	uint16_t cnt=0;
//	uint32_t isr_temp;
//	
//	isr_temp=USART1->ISR;
//	if((isr_temp&(1<<5))==(1<<5))//如果接收到数据
////  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)// 如果接收到数据
//  {
////		USART1->CR1 &= ~(1<<5);// 关闭接收中断
//    /* Read one byte from the receive data register */
//    aRxBuffer[RxCounter++] = USART_ReceiveData(USART1);
////    if(RxCounter == 8)
////    {
////      ReceiveState = 1;
////      RxCounter = 0;
////			if((aRxBuffer[0]==0x0A)&&(aRxBuffer[6]==0x38)&&(aRxBuffer[7]==0xFF))//数据接收正确
////			{
////				
////			}
////    }
//  }
//}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
