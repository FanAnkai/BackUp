
#include "global.h"



//uint8_t  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};  //本地地址
//uint8_t  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};  //接收地址

static uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x19,0x3c,0xb7};

uint8_t SPI_Write_Reg(uint8_t reg, uint8_t value);
static uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars);
static uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars);
void delay_us(uint16_t t)
{
    while(t--)
    {}
}
/*******************************************************************************
* Function Name  : Flash_ReadWriteByte
* Description    : 通过硬件SPI发送一个字节到SST25VF016B
* Input          : - data: 发送的数据
* Output         : None
* Return         : SST25VF016B 返回的数据
* Attention      : None
*******************************************************************************/

#define MY_SPI_MODE 1
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{

#if MY_SPI_MODE == 0
    while((SPI1->SR&1<<1)==0);  //等待发送区空
    SPI1->DR=TxData;              //发送一个byte
    while((SPI1->SR&1<<0)==0);  //等待接收完一个byte
    return SPI1->DR;            //返回收到的数据

#elif MY_SPI_MODE == 1

    uint8_t RxData;
    HAL_SPI_TransmitReceive(&hspi1, &TxData, &RxData, 1, 1000);
    return RxData;

#elif MY_SPI_MODE == 2

#define DELAY_CNT 50
    u8 bit_ctr;
    int delay;
    delay = DELAY_CNT;
    while(delay) delay--;

    for(bit_ctr = 0; bit_ctr < 8; bit_ctr++)
    {
        if(TxData & 0x80)
            MOSI_OUT(1);
        else
            MOSI_OUT(0);
        TxData = (TxData << 1);
        delay = DELAY_CNT;
        while(delay) delay--;
        SCK_OUT(1);
        TxData |= MISO_IN();
        delay = DELAY_CNT;
        while(delay) delay--;
        SCK_OUT(0);
        delay = DELAY_CNT;
        while(delay) delay--;

    }
    return(TxData);


#endif
}
/*
*****************************************************************
* 初始化
*****************************************************************
*/
void Init_NRF24L01(void)
{
    HAL_Delay(10);
    NRF_CE_LOW();
    SPI_NSS_HIGH();

    SPI_Write_Reg(RF_WRITE_REG + CONFIG, 0x0f);							// 20 CRC使能，16位CRC校验，上电，接收模式
    SPI_Write_Reg(RF_WRITE_REG + SETUP_AW, 0x01);						//23
    SPI_Write_Buf(RF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	// 写入发送地址
    SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// 2A  为了应答接收设备，接收通道0地址和发送地址相同
    SPI_Write_Reg(RF_WRITE_REG + EN_AA, 0x0);							// 21 使能接收通道0自动应答
    SPI_Write_Reg(RF_WRITE_REG + EN_RXADDR, 0x01);						// 22 使能接收通道0

    SPI_Write_Reg(RF_WRITE_REG + RF_CH, 0x1e);							// 25 选择射频通道0x40
    SPI_Write_Reg(RF_WRITE_REG + RX_PW_P0, NRF_FIFO_SIZE);				// 31 接收通道0选择和发送通道相同有效数据宽度

    SPI_Write_Reg(RF_WRITE_REG + RF_SETUP, 0x03);						// 26 数据传输率2Mbps，发射功率7dBm


    SPI_Write_Reg(RF_WRITE_REG + STATUS, 0x70);							// 27 清除所有的中断标志位
    SPI_Write_Reg(FLUSH_RX,0xff);										//清除RX FIFO寄存器


    NRF_CE_HIGH();
    HAL_Delay(10);
}

/*
*****************************************************************
* 写寄存器
*****************************************************************
*/
uint8_t wRegStat;
uint8_t SPI_Write_Reg(uint8_t reg, uint8_t value)
{

    SPI_NSS_LOW();                    /* 选通器件 */
    wRegStat = SPI_ReadWriteByte(reg);  /* 写寄存器地址 */
    wRegStat = SPI_ReadWriteByte(value);         /* 写数据 */
    SPI_NSS_HIGH();                   /* 禁止该器件 */
    return  wRegStat;
}
/*
*****************************************************************
* 读寄存器
*****************************************************************
*/
uint8_t SPI_Read_Reg(uint8_t reg)
{
    uint8_t reg_val;
    SPI_NSS_LOW();                    /* 选通器件 */
    SPI_ReadWriteByte(reg);           /* 写寄存器地址 */
    reg_val = SPI_ReadWriteByte(0);   /* 读取该寄存器返回数据 */
    SPI_NSS_HIGH();                   /* 禁止该器件 */
    return  reg_val;
}
/*
*****************************************************************
*
* 写缓冲区
*
*****************************************************************
*/

uint8_t wBufStat;
static uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
    uint8_t i;

    SPI_NSS_LOW();                      /* 选通器件 */
    wBufStat = SPI_ReadWriteByte(reg);    /* 写寄存器地址 */
    for(i=0; i<uchars; i++)
    {
        wBufStat = SPI_ReadWriteByte(pBuf[i]);     /* 写数据 */
    }
    SPI_NSS_HIGH();                     /* 禁止该器件 */
    return  wBufStat;
}
/*
*****************************************************************
* 读缓冲区
*****************************************************************
*/
static uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
    uint8_t i;
    uint8_t status;
    SPI_NSS_LOW();                      /* 选通器件 */
    status = SPI_ReadWriteByte(reg);    /* 写寄存器地址 */
    for(i=0; i<uchars; i++)
    {
        pBuf[i] = SPI_ReadWriteByte(0); /* 读取返回数据 */
    }
    SPI_NSS_HIGH();                     /* 禁止该器件 */
    return  status;
}


/*
*****************************************************************
* 接收模式
*****************************************************************
*/
void SetRX_Mode(void)
{
    NRF_CE_LOW();
    SPI_Write_Reg(RF_WRITE_REG + CONFIG, 0x0f);         // IRQ收发完成中断响应，16位CRC ，主接收
    NRF_CE_HIGH();
}
/*
*****************************************************************
* 接收模式
*****************************************************************
*/
void SetTX_Mode(void)
{
    NRF_CE_LOW();
    SPI_Write_Reg(RF_WRITE_REG + CONFIG, 0x02);          // IRQ收发完成中断响应，16位CRC，主发送
    NRF_CE_HIGH();
}
/*
*****************************************************************
* 读数据包
*****************************************************************
*/
uint8_t rRegStat=0;
uint8_t nRF24L01_RxPacket(uint8_t * rx_buf, uint8_t len)
{
    uint8_t revale=0;

    rRegStat = SPI_Read_Reg(STATUS); // 读取状态寄存其来判断数据接收状况

    gNrfReg.Rssi = SPI_Read_Reg(CD);

    if(rRegStat&0x40)                // 判断是否接收到数据
    {
        SPI_Read_Buf(RD_RX_PLOAD,rx_buf,len);// read receive payload from RX_FIFO buffer
        SPI_Write_Reg(FLUSH_RX,0xff);			//清除RX FIFO
        revale =1;								//读取数据完成标志
    }
    SPI_Write_Reg(RF_WRITE_REG+STATUS,rRegStat);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志


    if(!gNrfReg.Rssi)
    {
        //memset(rx_buf, 0x00, len);
        //return 0;
    }


    return revale;
}
/*
*****************************************************************
* 写数据包
*****************************************************************
*/
void nRF24L01_TxPacket(uint8_t * tx_buf, uint8_t len)
{
    NRF_CE_LOW();        //StandBy I模式

    SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
    SPI_Write_Buf(WR_TX_PLOAD, tx_buf, len);             // 装载数据
//  SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0x0e);         // IRQ收发完成中断响应，16位CRC，主发送
    NRF_CE_HIGH();       //置高CE，激发数据发送
}


/*======================================================================*/

