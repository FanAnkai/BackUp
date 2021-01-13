#ifndef __APP_NRF24_H__
#define __APP_NRF24_H__

#include "stm32f4xx_hal.h"
#include "drv_nrf24.h"

#define MAX_DATA_SIZE 400

typedef struct
{
    uint8_t DataBuf[MAX_DATA_SIZE][9];
    uint8_t RxBuf[NRF_FIFO_SIZE];

    uint8_t Rssi;

    uint16_t DelayCnt;
    uint16_t DataNum;

} NF_TYPE;

extern NF_TYPE gNrfReg;

void Nrf_Soft_Init(void);
void Task_Nrf2401(void);

uint8_t MY_StrStr(uint8_t * str1, uint8_t* str2, uint8_t len);
int Find_Equal(unsigned char * buff);
#endif
