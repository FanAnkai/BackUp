
#ifndef _LED_H
#define _LED_H

#include "global.h"

#define LED_PORT_EN()		__HAL_RCC_GPIOD_CLK_ENABLE();
#define LED_PORT GPIOD
#define LED_PIN GPIO_PIN_13

void Led_Drv_init(void);

void Task_Led_Pwm(void);
void Task_Led_Blink(void);

void Led_App_Init(void);
void Led_Set_Pwm(u32 n);


#endif /* __DRV_LED_H */

/*******************************  END OF FILE  ********************************/



