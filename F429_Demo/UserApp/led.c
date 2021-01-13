

#include "led.h"

void Led_Drv_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    LED_PORT_EN();

    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void Led_App_Init(void)
{
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);//点亮
}


//50ms调用
void Task_Led_Blink(void)
{
    static u8 led_cnt = 0;
    if(gTask_Rrg.PowerUpCnt > 0)
        return;

    led_cnt++;

    if(led_cnt <= 2)
    {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);//点亮
    }
    else if(led_cnt < 20)
    {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);//灭
    }
    else
    {
        led_cnt = 0;
    }
}

#define LED_UP 1
#define LED_DOWN 0

void Task_Led_Pwm(void)
{
    static u8 led_cnt = 0;
    static u8 led_flag = LED_UP;

    if(gTask_Rrg.PowerUpCnt > 0)
        return;

    if(led_flag == LED_UP)
        led_cnt++;
    else if(led_flag == LED_DOWN)
        led_cnt--;

    Led_Set_Pwm(led_cnt);

    if(led_cnt >= 100)
    {
        led_flag = LED_DOWN;
    }

    if(led_cnt <= 1)
    {
        led_flag = LED_UP;
    }

}


void Led_Set_Pwm(u32 n)
{

    TIM_OC_InitTypeDef sConfigOC = {0};

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = n*10;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}

/*******************************  END OF FILE  ********************************/
