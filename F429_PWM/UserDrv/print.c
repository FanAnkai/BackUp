

#include "global.h"

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
/* FILE is typedef’ d in stdio.h. */
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
    USART1->DR = (uint8_t) ch;
    return ch;
}


//将字符转为整形
int My_atoi(uint8_t * buff, uint8_t len)
{
    int temp = 0;
    uint8_t i = 0;

    for(i=0; i<len; i++)
    {
        temp = temp*256+buff[i];
    }

    return temp;
}

//转换为16进制字符串
uint8_t temp[20];
uint8_t * My_HexToStr(uint8_t * buff, uint8_t len)
{

    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t byte = 0;

    for(i=0; i<len*2; i++)
    {
        j = i>>1;
        byte = buff[j] >> 4 ;
        if(byte<=9)
            temp[i] = byte+'0' ;
        else
            temp[i] = byte+'A'-10 ;

        i++;

        byte = buff[j] & 0x0f;
        if(byte<=9)
            temp[i] = byte+'0' ;
        else
            temp[i] = byte+'A'-10 ;
    }

    return (uint8_t*)temp;

}


u16 Crc_Crc16(u8 *ptr, u16 len)
{
    u8 i = 0;
    u16 u16Crc = 0;

    while (len--)
    {
        for (i = 0x80; i != 0; i /= 2)
        {
            /* 余式CRC乘以2 再求CRC */
            if ((u16Crc & 0x8000) != 0)
            {
                u16Crc *= 2;
                u16Crc ^= 0x1021;
            }
            else
            {
                u16Crc *= 2;
            }
            /* 再加上本位的CRC */
            if ((*ptr & i) != 0)
            {
                u16Crc ^= 0x1021;
            }
        }
        ptr++;
    }
    return u16Crc;
}





