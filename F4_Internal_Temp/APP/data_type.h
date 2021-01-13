#ifndef DATA_TYPE_H_
#define DATA_TYPE_H_



#define TASK_10ms 10
#define TASK_50ms 50
#define TASK_100ms 100
#define TASK_200ms 200
#define TASK_500ms 500
#define TASK_1000ms 1000

typedef struct
{
    unsigned int TaskCnt0;
    unsigned int TaskCnt1;
    unsigned int TaskCnt2;
    unsigned int TaskCnt3;
    unsigned int TaskCnt4;
    unsigned int TaskCnt5;

} TASK_TPYE;

typedef struct
{
    unsigned int cnt0;
    unsigned int cnt1;

} TIM_TPYE;


#define ADC_MAX_CNT 1000
#define ADC_REMOVE_CNT 300

typedef struct
{
    unsigned int val[ADC_MAX_CNT];
    unsigned int cnt;
	unsigned char stop_flag;

} ADC_TPYE;


typedef struct
{
    float val;

} TEMPERATURE_TPYE;


extern TASK_TPYE gTaskReg;
extern TIM_TPYE gTim4Reg;
extern ADC_TPYE gAdcReg;
extern TEMPERATURE_TPYE gTempReg;




#endif
