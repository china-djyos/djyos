//#include "sys_pwm.h"
//#include "sys_types.h"
//#include "sys_uart.h"
//#include "sys_interrupt.h"
//#include "sys_delay.h"
#include <stdint.h>
#include "cpu_peri_gpio.h"
#include "cpu_peri_io.h"

#define sysprintf printf
#define delay_ms(x) DJY_DelayUs(x * 1000)
#define u64_t u64
#define u32_t u32
#define u16_t u16
#define u8_t  u8

/*寄存器地址*/
#define F1C100S_PWM_BASE  (0x01C21000)
#define PWM_CTRL_REG                      F1C100S_PWM_BASE+(0x0)//PWM Control Register
#define PWM_CH0_PERIOD                  F1C100S_PWM_BASE+(0x4)//PWM Channel 0 Period Register
#define PWM_CH1_PERIOD                  F1C100S_PWM_BASE+(0x8)//PWM Channel 1 Period Register


#define PWM_BIT(x, bit) ((1 << bit) << ((x) * 15))
#define PWM_PERIOD(x)     (x==0?PWM_CH0_PERIOD:PWM_CH1_PERIOD)

static const u32_t prescaler_table[] = {
    120,
    180,
    240,
    360,
    480,
    0,
    0,
    0,
    12000,
    24000,
    36000,
    48000,
    72000,
    0,
    0,
    0,
};
/*pwm io初始化
*/
void pwm_io_init(int channe)
{
 if(channe==0)
    //GPIO_Congif(GPIOE,GPIO_Pin_12,GPIO_Mode_100,GPIO_PuPd_NOPULL);
  GPIO_Congif(GPIOA,GPIO_Pin_2,GPIO_Mode_011,GPIO_PuPd_NOPULL);
 else
    GPIO_Congif(GPIOE,GPIO_Pin_6,GPIO_Mode_011,GPIO_PuPd_NOPULL);
}
/*pwm配置
channel     通道=0/1
polarity    极性 0: Low Level  1: High Level
period      周期
duty      占空
*/
static void pwm_f1c100s_config(int channel,int duty, int period, int polarity)
{
    u64_t rate = 24000000;
    u64_t div;
    u32_t prd, dty, gate;
    u32_t prescaler = 0;
    u32_t ctrl;


        prescaler = 0xf;
        div = rate * period + 1000000000 / 2;
        div = div / 1000000000;
        if(div - 1 > 0xffff)prescaler = 0;

        if(prescaler == 0)
        {
            for(prescaler = 0; prescaler < 0xf; prescaler++)
            {
                if(!prescaler_table[prescaler])
                    continue;
                div = rate;
                div = div / prescaler_table[prescaler];
                div = div * period;
                div = div / 1000000000;
                if(div - 1 <= 0xffff)
                    break;
            }

            if(div - 1 > 0xffff)
            {
                return;
            }
        }
        prd = div;
        div *= duty;
        div = div / period;
        dty = div;


        sysprintf("prd=%d dty=%d prescaler=%d \r\n",prd,dty,prescaler);

        //清空
        write32(PWM_CTRL_REG, read32(PWM_CTRL_REG) &~(0x3fff << (channel * 15)));
        write32(PWM_PERIOD(channel), 0);


    //返回时钟通过否
        gate = (read32(PWM_CTRL_REG) & PWM_BIT(channel, 6)) ? 1 : 0;
        if(gate)//时钟通过先关掉
        {
            ctrl = read32(PWM_CTRL_REG);
            ctrl &= ~PWM_BIT(channel, 6);
            write32(PWM_CTRL_REG, ctrl);
        }

        //设置 预分频
        ctrl = read32(PWM_CTRL_REG);
        ctrl &= ~(0xf << (channel * 15));
        ctrl |= (prescaler << (channel * 15));
        write32(PWM_CTRL_REG, ctrl);

        //设置 预分频
        write32(PWM_PERIOD(channel), ((prd - 1) << 16) | (dty & 0xffff));

        //原来是开的-现在开
        if(gate)
        {
            ctrl = read32(PWM_CTRL_REG);
            ctrl |= PWM_BIT(channel, 6);
            write32(PWM_CTRL_REG, ctrl);
        }
    //设置极性
    ctrl = read32(PWM_CTRL_REG);
    if(polarity)ctrl |=  PWM_BIT(channel, 5);
    else        ctrl &= ~PWM_BIT(channel, 5);
    write32(PWM_CTRL_REG, ctrl);

}
/*pwm使能*/
void pwm_f1c100s_enable(int channel)
{
    u32_t ctrl;
    ctrl = read32(PWM_CTRL_REG);
    ctrl |= PWM_BIT(channel, 4);
    ctrl |= PWM_BIT(channel, 6);
    write32(PWM_CTRL_REG, ctrl);
}
/*pwm out 24mhz enable*/
void pwm_out_osc24m_enable(int channel)
{
    if(channel==1)S_BIT(PWM_CTRL_REG,24);//pwm1 out osc24MHZ
    if(channel==0)S_BIT(PWM_CTRL_REG,9);//pwm0 out osc24MHZ
}
/*pwm初始化
channel 通道 0,1
frequency 频率HZ
pwm 占空比% 0-100
polarity 极性 1低占空 0高占空
*/
void PWM_Init(int channel,int frequency_hz,int pwm,int polarity)
{
int period=1000000000/frequency_hz;
int duty=period*pwm/100;

    sysprintf("PWM_Init...\r\n");

    pwm_io_init(channel);
    pwm_f1c100s_config(channel,duty,period,polarity);
    if(frequency_hz==24000000)pwm_out_osc24m_enable(channel);
    pwm_f1c100s_enable(channel);
}

/*pwm测试*/
//void PWM_Demo(void)
//{
//    int i;
//    sysprintf("PWM_Demo...\r\n");
//    PWM_Init(1,1000,100,1);
//    delay_ms(2000);
//    //亮暗循环
//    while(1)
//    {
//        for(i=0;i<100;i++)
//        {
//            delay_ms(10);
//            PWM_Init(1,24000000,i,1);
//        }
//        for(i=100;i>0;i--)
//        {
//            delay_ms(10);
//            PWM_Init(1,24000000,i,1);
//        }
//    }
//}


