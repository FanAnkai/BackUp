/****************************************************/
// 程序用途：用来测试通过内部基准电压计算外部VDDA的值
// 程序作者：孟瑞生
// 微信公众号：科技老顽童
/****************************************************/
#include "stm32f0xx.h"
#include "stdio.h"

__IO uint16_t VREFINT_CAL;
__IO uint16_t VREFINT_DATA;
__IO float VDDA_VAL;

// 下面三个延时函数,是用示波器试出来的,非精确延时(48MHz)
void delay_1us(void)
{
    volatile uint16_t i = 1;

    while(i--);
}

void delay_us(uint16_t us)
{
    while(us--)
    {
        delay_1us();
    }
}

void delay_ms(uint16_t ms)
{
    while(ms--)
    {
        delay_us(995);
    }
}

/**
  * @brief  ADC Configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
    ADC_InitTypeDef     ADC_InitStructure;
    GPIO_InitTypeDef    GPIO_InitStructure;

    /* GPIOC Periph clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* ADC1 Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Configure ADC Channel 0 as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADCs DeInit */
    ADC_DeInit(ADC1);

    /* Initialize ADC structure */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* Convert the ADC1 Channel 0 with 239.5 Cycles as sampling time */
    ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint , ADC_SampleTime_239_5Cycles);
    ADC_VrefintCmd(ENABLE);

    /* 得到基准电压校准值 */
    VREFINT_CAL = *(__IO uint16_t *)(0X1FFFF7BA);

    /* ADC Calibration */
    ADC_GetCalibrationFactor(ADC1);

    /* Enable the ADC peripheral */
    ADC_Cmd(ADC1, ENABLE);

    /* Wait the ADRDY flag */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

    /* ADC1 regular Software Start Conv */
    ADC_StartOfConversion(ADC1);
}

/**
  * @brief  UART1 Configuration
  * @param  None
  * @retval None
  */
void UART1_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* Enable USART1 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* USART1 Pins configuration ************************************************/
    /* Connect pin to Periph */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    /* Configure pins as AF pushpull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1 IRQ Channel configuration */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* USART1  configured as follow:
    - BaudRate = 9600 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    /* Enable the USART1 */
    USART_Cmd(USART1, ENABLE);
}

int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    /* Loop until transmit data register is empty */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}

// 主函数
int main(void)
{
    delay_ms(200);
    ADC_Config();
    UART1_Init();

    while(1)
    {
        VREFINT_DATA = ADC_GetConversionValue(ADC1);
        VDDA_VAL = (3.3 * VREFINT_CAL) / VREFINT_DATA;
        printf("\n\rVDDA:%.3fV\n\r", VDDA_VAL);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        delay_ms(1000);
    }
}


