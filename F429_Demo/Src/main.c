/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */


#include "global.h"


TASK_TYPE   gTask_Rrg = {0};
NF_TYPE     gNrfReg;



int main(void)
{

    Mcu_Init();

    //Led_Drv_init();

    while (1)
    {
        if(gTask_Rrg.t_10ms == 0)
        {
            gTask_Rrg.t_10ms = Task_10ms;
        }
        if(gTask_Rrg.t_20ms == 0)
        {
            gTask_Rrg.t_20ms = Task_20ms;
            Task_Led_Pwm();
        }
        if(gTask_Rrg.t_50ms == 0)
        {
            gTask_Rrg.t_50ms = Task_50ms;

        }
        if(gTask_Rrg.t_100ms == 0)
        {
            gTask_Rrg.t_100ms = Task_100ms;
        }
        if(gTask_Rrg.t_200ms == 0)
        {
            gTask_Rrg.t_200ms = Task_200ms;
        }
        if(gTask_Rrg.t_500ms == 0)
        {
            gTask_Rrg.t_500ms = Task_500ms;
        }
        if(gTask_Rrg.t_1000ms == 0)
        {
            gTask_Rrg.t_1000ms = Task_1000ms;

        }
    }
}




/**
  * @brief System Clock Configuration
  * @retval None
  */
