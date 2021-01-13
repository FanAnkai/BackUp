
#ifndef _EC20_H
#define _EC20_H

#include "data_type.h"


#define RxNum_4G	100
#define TxNum_4G	100

#define TCP_VERSION 0x0100	//V1.00

#define TCP_CMD_HEART	0x00	//心跳
#define TCP_CMD_DATA	0x01	//实时数据
#define TCP_CMD_SET_TIM 0x03	//设置上传间隔
#define TCP_CMD_SET_IP	0x04	//设置IP地址、端口号



enum CONNEVT_TYPE
{
    STEP_REBOOT = 0,
    STEP_REBOOT_WAIT,
    STEP_CPIN,
    STEP_CREG,
    STEP_CGREG,
    STEP_APN = 5,
    STEP_ACT,
    STEP_RSSI,
    STEP_OPENT,
    STEP_CONNECT,
    STEP_STOP_TT = 10,
    STEP_CLOSE,
    STEP_IDLE,

};



typedef struct
{


    u8 RxBuff[RxNum_4G];
    u8 TxBuff[TxNum_4G];
    u8 DataBuff[RxNum_4G];

    u8 AT_RxOk;
    u8 TT_RxOk;
    u8 RxOk;
    u8 TxOk;
    u8 UpFlag;		//上传标志位
    u8 StepFlag;
    u8 WaitFlag;
    u8 Rssi;


    u8 TimOutCnt;		//AT指令应答超时
    __IO u16 RxTimCnt;	//串口接受超时认为接收完成
    __IO u8 RxTimStart;

    u16 DataLen;
    u16 WaitCnt;
    __IO u16 RxIndex;


} EC20_4G_TYPE;

extern EC20_4G_TYPE gEc20Reg;

void Task_4G(void);
void Ec20_Reboot(void);
void Ec20_Active(void);
void Ec20_Csq(void);

void Ec20_Tcp_Up(u8* buff, u16 len);
void Ec20_SoftInit(void);
void Ec20_Open(u8* ip_buff, u8* port_buff, u8* mode_buff);



#endif

/*******************************  END OF FILE  ********************************/


