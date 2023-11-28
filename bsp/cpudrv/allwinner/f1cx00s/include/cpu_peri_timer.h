#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2

#define F1C100S_TIMER_BASE  (0x01C20C00)

struct TimerCell
{
    vu32 TimerCtrl;        //10,20,30,timer ctrl
    vu32 TimerInterval;    //14,24,34,timer Timer_Interval
    vu32 TimerValue;       //18,28,38,timer current Timer_Value
    u32  pads;
};
struct F1cx00Timer      //0x01c20c00
{
    vu32 IrqEn;         //00 timer irq Timer_enable,only irq,not fiq
    vu32 IrqSta;        //04 timer irq status
    u32 pad[2];
    struct TimerCell Cell[3];   //三个独立定时器
    u32 pad1[14];
    vu32 AvsCtrl;       //80 avs control
    vu32 AvsCnt0;       //84 avs counter 0
    vu32 AvsCnt1;       //88 avs counter 1
    vu32 AvsDiv;        //8c avs counter divisor
    u32 pad2[4];
    vu32 WdogIrqEn;     //a0 Wdog irq enable
    vu32 WdogIrqSta;    //a4 Wdog irq status
    u32 pad3[2];
    vu32 WdogCtrl;      //b0 wdog control
    vu32 WdogCfg;       //b4 wdog config
    vu32 WdogMode;      //b8 wdog mode
};

/*音视频同步时间初始化-100us*/
#define AVS_TIME_0      0
#define AVS_TIME_1      1

void AVS_Time_Init(int AVS_TIME_Inx,int us);
/*开始并计数器清0*/
void AVS_Time_Start(int AVS_TIME_Inx);
void AVS_Time_Stop(int AVS_TIME_Inx);
/*读计数值*/
unsigned int AVS_Time_Read(int AVS_TIME_Inx);
/*计数暂停*/
void AVS_Time_Pause(int AVS_TIME_Inx);
/*计数继续*/
void AVS_Time_Recover(int AVS_TIME_Inx);

//void TIMER0_ISR(void);
//void Timer_Demo(void);
//void Tdelay_ms(int ms);
//void Timer_enable(u8 timer);
//void Timer_disable(u8 timer);
//void Timer_Init(u8 timer,u32_t time_us,u8 IRQ_EN);
//

#endif
