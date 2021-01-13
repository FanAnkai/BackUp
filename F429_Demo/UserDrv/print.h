

#ifndef PRINT_H_
#define PRINT_H_

#include "stm32f4xx_hal.h"


int My_atoi(uint8_t * buff, uint8_t len);
//转换为16进制字符串
uint8_t* My_HexToStr(uint8_t * buff, uint8_t len);
u16 Crc_Crc16(u8 *ptr, u16 len);


#endif

