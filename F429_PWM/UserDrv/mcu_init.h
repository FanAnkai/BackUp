

#ifndef MCU_INIT_H_
#define MCU_INIT_H_

#include "stm32f4xx_hal.h"


//extern SPI_HandleTypeDef hspi1;
//extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;

extern TIM_HandleTypeDef htim4;



#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define SPI1_CE_Pin GPIO_PIN_8
#define SPI1_CE_GPIO_Port GPIOA

#define SPI1_IRQ_Pin GPIO_PIN_12
#define SPI1_IRQ_GPIO_Port GPIOA


void Mcu_Init(void);

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);


void Error_Handler(void);

#endif

