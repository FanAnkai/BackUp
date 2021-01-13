
#include "global.h"


EC20_4G_TYPE gEc20Reg = {0};



#define IP_IDEEX	21
#define IP_SIZE		15

#define PORT_IDEEX	38
#define PORT_SIZE	5

#define MODE_IDEEX	46
#define MODE_SIZE	1

#define WAIT_CNT	100	//100ms执行，共等待40s


const u8 p_ec20_reboot[]	=	"AT+CFUN=1,1\r\n";
const u8 p_ec20_cpin[]		=	"AT+CPIN?\r\n";
const u8 p_ec20_creg[]		=	"AT+CREG=1\r\n";
const u8 p_ec20_cgreg[]		=	"AT+CGREG=1\r\n";
const u8 p_ec20_apn[]		=	"AT+QICSGP=1\r\n";
const u8 p_ec20_active[]	=	"AT+QIACT=1\r\n";
const u8 p_ec20_csq[]		=	"AT+CSQ\r\n";

const u8 p_ec20_close[]		=	"AT+QICLOSE=0\r\n";
const u8 p_ec20_stop[]		=	"+++";

const u8 p_ec20_open[]		=	"AT+QIOPEN=1,0,\"TCP\",\"061.149.215.202\",03000,0,2\r\n";


u8 p_ec20_ip[16] =	"061.149.215.202";
u8 p_ec20_port[6] =	"03000";
u8 p_ec20_mode[2] =	"2";

u8 step_flag = 0;
u8 next_flag = 0;
u8 task_cnt = 0;



void Ec20_SendBuff(u8* buff, u8 len)
{
    HAL_UART_Transmit(&huart1, (u8*)buff, len, 400);
}

u8 find_test = 0;
u8 Ec20_FindStr(const char* str)
{
    char* pStr = null;
    pStr = strstr((const char *)gEc20Reg.DataBuff, str);

    find_test = !(pStr == null);
    return find_test;
}

void Ec20_SoftInit(void)
{
    gEc20Reg.StepFlag = 0;
    gEc20Reg.WaitFlag = 0;
    gEc20Reg.WaitCnt = WAIT_CNT;
    gEc20Reg.TimOutCnt = 0;

    printf("Hello World\r\n");
    HAL_Delay(20);

}

void EC20_RunFunc_WaitAck(void(*func)(void), u8* str)
{
    if(gEc20Reg.WaitFlag == 0)
    {
        func();
        gEc20Reg.WaitFlag = 1;
    }

    if(gEc20Reg.AT_RxOk == 1)
    {
        gEc20Reg.AT_RxOk = 0;
        memcpy(gEc20Reg.DataBuff, gEc20Reg.RxBuff, RxNum_4G);
        memset(gEc20Reg.RxBuff, 0, RxNum_4G);

        if(Ec20_FindStr((const char*)str))
        {
            gEc20Reg.StepFlag++;
            gEc20Reg.WaitFlag = 0;
            gEc20Reg.WaitCnt = WAIT_CNT;
            if(func == &Ec20_Reboot)
            {
                gEc20Reg.WaitCnt = 500;
            }
            else if(func == &Ec20_Active)
            {
                gEc20Reg.WaitCnt = 500;
            }
            else if(func == &Ec20_Csq)
            {
                gEc20Reg.Rssi = (gEc20Reg.DataBuff[15]-'0')*10;
                gEc20Reg.Rssi += gEc20Reg.DataBuff[16] - '0';
                //15,16
            }
        }
        if(Ec20_FindStr("ERROR"))
        {
            gEc20Reg.WaitCnt = 0;
        }
    }
}

void EC20_Open_WaitAck(u8* str)
{
    if(gEc20Reg.WaitFlag == 0)
    {
        Ec20_Open(p_ec20_ip, p_ec20_port, p_ec20_mode);
        gEc20Reg.WaitFlag = 1;
    }

    if(gEc20Reg.AT_RxOk == 1)
    {
        gEc20Reg.AT_RxOk = 0;
        memcpy(gEc20Reg.DataBuff, gEc20Reg.RxBuff, RxNum_4G);
        memset(gEc20Reg.RxBuff, 0, RxNum_4G);

        if(Ec20_FindStr((const char*)str))
        {
            gEc20Reg.StepFlag++;
            gEc20Reg.WaitFlag = 0;
            gEc20Reg.WaitCnt = WAIT_CNT;
        }
        if(Ec20_FindStr("ERROR"))
        {
            gEc20Reg.WaitCnt = 0;
        }
    }
}


void EC20_WaitAck(u8* str)
{
    if(gEc20Reg.AT_RxOk == 1)
    {
        gEc20Reg.AT_RxOk = 0;
        memcpy(gEc20Reg.DataBuff, gEc20Reg.RxBuff, RxNum_4G);
        memset(gEc20Reg.RxBuff, 0, RxNum_4G);

        if(Ec20_FindStr((const char*)str))
        {
            gEc20Reg.StepFlag++;
            gEc20Reg.WaitFlag = 0;
            gEc20Reg.WaitCnt = WAIT_CNT;
        }
        if(Ec20_FindStr("ERROR"))
        {
            gEc20Reg.WaitCnt = 0;
        }
    }
}




void Ec20_AT_Rx(void)
{
    gEc20Reg.RxBuff[gEc20Reg.RxIndex] = huart1.Instance->DR;
    gEc20Reg.RxTimStart = 1;

    switch (gEc20Reg.RxIndex)
    {
#if 0
    case 0:
        if(gEc20Reg.RxBuff[gEc20Reg.RxIndex] == 'A')
            gEc20Reg.RxIndex++;
        else
            gEc20Reg.RxIndex = 0;
        break;
    case 1:
        if(gEc20Reg.RxBuff[gEc20Reg.RxIndex] == 'T')
            gEc20Reg.RxIndex++;
        else
            gEc20Reg.RxIndex = 0;
        break;

#endif

    default:

#if 1
    {
        gEc20Reg.RxIndex++;

        gEc20Reg.RxTimCnt = 0;
    }
    break;

#else
    if(gEc20Reg.RxBuff[gEc20Reg.RxIndex] == 0x0A)
    {
        uart_end_cnt++;
        if(uart_end_cnt == 2)
        {
            uart_end_cnt = 0;

            memset(gEc20Reg.DataBuff, 0, RxNum_4G);
            memcpy(gEc20Reg.DataBuff, gEc20Reg.RxBuff, gEc20Reg.RxIndex);
            memset(gEc20Reg.RxBuff, 0, RxNum_4G);

            gEc20Reg.AT_RxOk = 1;
            gEc20Reg.RxIndex = 0;
        }
    }
    else
    {
        gEc20Reg.RxIndex++;
    }
    break;

#endif
    }




}

void Ec20_TT_Rx(void)
{
    gEc20Reg.RxBuff[gEc20Reg.RxIndex] = huart2.Instance->DR;

    switch (gEc20Reg.RxIndex)
    {
    case 0:
        if ( gEc20Reg.RxBuff[gEc20Reg.RxIndex] == 0x7a )
        {
            gEc20Reg.RxIndex++;
        }
        else
        {
            gEc20Reg.RxIndex = 0;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        gEc20Reg.RxIndex++;
        break;
    case 9:
        if(gEc20Reg.RxBuff[gEc20Reg.RxIndex] == 0x7b)
        {
            gEc20Reg.RxIndex = 0;
            gEc20Reg.RxOk = 1;
        }
        else
        {
            gEc20Reg.RxIndex = 0;
        }
        break;
    default:
        gEc20Reg.RxIndex = 0;

        break;
    }
}



void Ec20_Rx(void)
{
    uint32_t it_flag = 0;
    uint32_t it_source = 0;

    it_flag = __HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE);
    it_source = __HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE);

    if((it_flag != RESET) && (it_source != RESET))
    {
        if(gEc20Reg.StepFlag == STEP_CONNECT)
        {
            Ec20_TT_Rx();
        }
        else
        {
            Ec20_AT_Rx();
        }

    }
}


void Ec20_DataParse(void)
{


}



void Ec20_Reboot(void)
{
    Ec20_SendBuff((u8*)p_ec20_reboot, sizeof(p_ec20_reboot)-1);
}

void Ec20_Cpin(void)
{
    Ec20_SendBuff((u8*)p_ec20_cpin, sizeof(p_ec20_cpin)-1);
}

void Ec20_Creg(void)
{
    Ec20_SendBuff((u8*)p_ec20_creg, sizeof(p_ec20_creg)-1);
}

void Ec20_Cgreg(void)
{
    Ec20_SendBuff((u8*)p_ec20_cgreg, sizeof(p_ec20_cgreg)-1);
}


void Ec20_Apn(void)
{
    Ec20_SendBuff((u8*)p_ec20_apn, sizeof(p_ec20_apn)-1);
}


void Ec20_Active(void)
{
    Ec20_SendBuff((u8*)p_ec20_active, sizeof(p_ec20_active)-1);
}

void Ec20_Close(void)//断开连接
{
    Ec20_SendBuff((u8*)p_ec20_close, sizeof(p_ec20_close)-1);
}


void Ec20_Stop_T_T(void)//停止透传	//Transparent_transmission
{
    Ec20_SendBuff((u8*)p_ec20_stop, sizeof(p_ec20_stop)-1);
}

void Ec20_Csq(void)		//获取4G信号强度
{
    Ec20_SendBuff((u8*)p_ec20_csq, sizeof(p_ec20_csq)-1);
}



void Ec20_Open(u8* ip_buff, u8* port_buff, u8* mode_buff)//开启连接并启动透传
{
    u8 len = sizeof(p_ec20_open)-1;

    memcpy(gEc20Reg.TxBuff, p_ec20_open, len);

    memcpy(&gEc20Reg.TxBuff[IP_IDEEX], ip_buff, IP_SIZE);
    memcpy(&gEc20Reg.TxBuff[PORT_IDEEX], port_buff, PORT_SIZE);
    memcpy(&gEc20Reg.TxBuff[MODE_IDEEX], mode_buff, MODE_SIZE);



    Ec20_SendBuff(gEc20Reg.TxBuff, len);

}


void Ec20_ConnectMode(void)//处于连接状态
{
    gEc20Reg.WaitCnt = WAIT_CNT;

}

void Ec20_Idle(void)//处于空闲状态
{
    gEc20Reg.WaitCnt = WAIT_CNT;

}

u8 tcp_send_buff[20] = {0x7a, 0x7a, 0, 0, };
void Ec20_Tcp_Up(u8* buff, u16 len)
{
    u16 crc_temp = 0;

    tcp_send_buff[2] = TCP_VERSION>>8;
    tcp_send_buff[3] = TCP_VERSION&0x00ff;
    tcp_send_buff[4] = TCP_CMD_DATA;
    tcp_send_buff[5] = len>>8;
    tcp_send_buff[6] = len&0x00ff;

    crc_temp = Crc_Crc16(buff, len);

    tcp_send_buff[10] = crc_temp>>8;
    tcp_send_buff[11] = crc_temp&0x00ff;
    tcp_send_buff[12] = 0x7b;
    tcp_send_buff[13] = 0x7b;

    HAL_UART_Transmit(&huart1, tcp_send_buff, 7, 100);
    HAL_UART_Transmit(&huart1, buff, len, 5000);
    HAL_UART_Transmit(&huart1, &tcp_send_buff[10], 4, 100);


}

void Task_4G(void)
{
    if(gEc20Reg.WaitCnt > 0)
    {
        gEc20Reg.WaitCnt--;
    }

    if(gEc20Reg.WaitCnt == 0)
    {
        gEc20Reg.TimOutCnt++;
        if(gEc20Reg.TimOutCnt <= 2)
        {
            gEc20Reg.WaitFlag = 0;
            gEc20Reg.WaitCnt = WAIT_CNT;
        }
        else if(gEc20Reg.TimOutCnt >= 3)
        {
            Ec20_SoftInit();
        }
    }

    Ec20_DataParse();

    switch(gEc20Reg.StepFlag)
    {
    case STEP_REBOOT:

        EC20_RunFunc_WaitAck(Ec20_Reboot, "OK");
        break;

    case STEP_REBOOT_WAIT:
        EC20_WaitAck("RDY");
        break;

    case STEP_CPIN:
        EC20_RunFunc_WaitAck(Ec20_Cpin, "READY");
        break;
    case STEP_CREG:
        EC20_RunFunc_WaitAck(Ec20_Creg, "OK");
        break;
    case STEP_CGREG:
        EC20_RunFunc_WaitAck(Ec20_Cgreg, "OK");
        break;
    case STEP_APN:
        EC20_RunFunc_WaitAck(Ec20_Apn, "OK");
        break;
    case STEP_ACT:
        EC20_RunFunc_WaitAck(Ec20_Active, "OK");
        break;
    case STEP_RSSI:
        EC20_RunFunc_WaitAck(Ec20_Csq, "OK");
        break;
    case STEP_OPENT:
        EC20_Open_WaitAck("CONNECT");
        break;
    case STEP_CONNECT:
        Ec20_ConnectMode();
        break;
    case STEP_STOP_TT:
        EC20_RunFunc_WaitAck(Ec20_Stop_T_T, "OK");
        break;
    case STEP_CLOSE:
        EC20_RunFunc_WaitAck(Ec20_Close, "OK");
        break;
    case STEP_IDLE:
        Ec20_Idle();
        break;
    default:
        break;
    }

}




