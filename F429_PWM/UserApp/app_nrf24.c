

#include "global.h"






uint16_t i = 0;



void Nrf_Soft_Init(void)
{
    Init_NRF24L01();
    SetRX_Mode();
}

#if 1
void Task_Nrf2401(void)
{

    uint8_t *print_buf;
    int print_num = 0;

    if(nRF24L01_RxPacket(gNrfReg.RxBuf, NRF_FIFO_SIZE))
    {
        if((gNrfReg.RxBuf[0] == 0x00))
        {
            for(i = 0; i <= gNrfReg.DataNum; i++)
            {
                if(MY_StrStr((uint8_t*)gNrfReg.DataBuf[i], gNrfReg.RxBuf, 4))//有相同
                {
                    break;
                }
                if(i == gNrfReg.DataNum)
                {
                    memcpy((char*)gNrfReg.DataBuf[gNrfReg.DataNum], (char*)gNrfReg.RxBuf, 4);
                    gNrfReg.DataBuf[gNrfReg.DataNum][4] = gNrfReg.Rssi;
                    memset(&gNrfReg.DataBuf[gNrfReg.DataNum][5], 0, 4);
                    gNrfReg.DataNum++;

                    if(gEc20Reg.StepFlag == STEP_CONNECT)
                    {
                        //huart1, gNrfReg.RxBuf[4] = gNrfReg.Rssi;
                        //HAL_UART_Transmit(&huart1, gNrfReg.RxBuf, 5, 100);
                        //print_num = My_atoi(gNrfReg.RxBuf, 4);
                        //print_buf = My_HexToStr(gNrfReg.RxBuf, 4);

                        //printf("%s %d  RSSI=%d  %d\r\n", print_buf, print_num, gNrfReg.Rssi, gNrfReg.DataNum);
                    }
                    else
                    {
                        //print_num = My_atoi(gNrfReg.RxBuf, 4);
                        //print_buf = My_HexToStr(gNrfReg.RxBuf, 4);

                        //printf("%s %d  RSSI=%d  %d\r\n", print_buf, print_num, gNrfReg.Rssi, gNrfReg.DataNum);
                    }

                    break;
                }
            }
        }
    }

    if(gNrfReg.DelayCnt >= 40)
    {
        __disable_irq();
        if(gEc20Reg.StepFlag == STEP_CONNECT)
        {
            //HAL_UART_Transmit(&huart1, (u8*)gNrfReg.DataBuf, 5*gNrfReg.DataNum, 5000);
            Ec20_Tcp_Up((u8*)gNrfReg.DataBuf, gNrfReg.DataNum);
        }


        gNrfReg.DelayCnt = 0;
        memset(gNrfReg.RxBuf, 0, sizeof(gNrfReg.RxBuf));
        memset(gNrfReg.DataBuf, 0, sizeof(gNrfReg.DataBuf));
        gNrfReg.DataNum = 0;
        __enable_irq();

        //printf("\r\nNRF_Scan:\r\n");
    }
}

#else

void Task_Nrf2401(void)
{

    uint8_t *print_buf;

    if(nRF24L01_RxPacket(rev_buff, NRF_FIFO_SIZE))
    {
        if(gNrfReg.RxBuf[0] == 0x00)
        {
            //memcpy((char*)gNrfReg.DataBuf[gNrfReg.DataNum], (char*)gNrfReg.RxBuf, NRF_FIFO_SIZE);
            gNrfReg.DataNum++;


            print_num = My_atoi(gNrfReg.RxBuf, 4);
            print_buf = My_HexToStr(gNrfReg.RxBuf, 4);

            printf("%s %d  RSSI=%d  %d\r\n", print_buf, print_num, RegVal, gNrfReg.DataNum);

        }
    }

}



#endif

//相同返回1
uint8_t MY_StrStr(uint8_t * str1, uint8_t* str2, uint8_t len)
{
    while(len > 0)
    {
        len--;
        if(*str1++ != *str2++)
            return 0;
    }
    return 1;
}

unsigned char equal_buff[] = {0x00, 0x0B, 0x51, 0xAA};

int Find_Equal(unsigned char * buff)
{
    uint8_t get_num = 0;
    uint32_t i = 0;
    uint32_t n = 0;

    unsigned char * test = equal_buff;

    uint32_t num = sizeof(equal_buff);

    for(n=0; n<num/4; n++)
    {
        for(i=0; i<num; i+=4)
        {
            if((test[0] == equal_buff[i])
                    && (test[1] == equal_buff[i+1])
                    && (test[2] == equal_buff[i+2])
                    && (test[3] == equal_buff[i+3]))
            {
                get_num++;
            }
        }

        HAL_UART_Transmit(&huart1, &get_num, 1, 100);
        get_num = 0;
        test = test+4;

    }

    while(1);

}




