#include "sys_I2C.h"
#include "sys_types.h"
#include "sys_uart.h"
#include "sys_delay.h"
#include "sys_gpio.h"
#include "sys_io.h"


enum {
    I2C_STAT_BUS_ERROR  = 0x00,
    I2C_STAT_TX_START   = 0x08,
    I2C_STAT_TX_RSTART  = 0x10,
    I2C_STAT_TX_AW_ACK  = 0x18,
    I2C_STAT_TX_AW_NAK  = 0x20,
    I2C_STAT_TXD_ACK    = 0x28,
    I2C_STAT_TXD_NAK    = 0x30,
    I2C_STAT_LOST_ARB   = 0x38,
    I2C_STAT_TX_AR_ACK  = 0x40,
    I2C_STAT_TX_AR_NAK  = 0x48,
    I2C_STAT_RXD_ACK    = 0x50,
    I2C_STAT_RXD_NAK    = 0x58,
    I2C_STAT_IDLE       = 0xf8,
};

/*寄存器地址*/
#define F1C100S_TWI0_BASE  (0x01C27000)
#define F1C100S_TWI1_BASE  (0x01C27400)
#define F1C100S_TWI2_BASE  (0x01C27800)

#define TWI_ADDR_REG(TWIx)   (F1C100S_TWI0_BASE+(TWIx*0x400)+0x00)  //TWI Slave Address
#define TWI_XADDR_REG(TWIx)  (F1C100S_TWI0_BASE+(TWIx*0x400)+0x04)  //TWI Extended Slave Address Register
#define TWI_DATA_REG(TWIx)   (F1C100S_TWI0_BASE+(TWIx*0x400)+0x08)  //TWI Data Byte Register
#define TWI_CNTR_REG(TWIx)   (F1C100S_TWI0_BASE+(TWIx*0x400)+0x0C)  //TWI Control Register
#define TWI_STAT_REG(TWIx)   (F1C100S_TWI0_BASE+(TWIx*0x400)+0x10)  //TWI Status Register
#define TWI_CCR_REG(TWIx)    (F1C100S_TWI0_BASE+(TWIx*0x400)+0x14)  //TWI Clock Control Register
#define TWI_SRST_REG(TWIx)   (F1C100S_TWI0_BASE+(TWIx*0x400)+0x18)  //TWI Software Reset Register
#define TWI_EFR_REG(TWIx)    (F1C100S_TWI0_BASE+(TWIx*0x400)+0x1C)  //TWI Enhance Feature Register
#define TWI_LCR_REG(TWIx)    (F1C100S_TWI0_BASE+(TWIx*0x400)+0x20)  //TWI Line Control Register


#define TIMEOUT 500000 //超时计数


/*2021-01-26
    Wait_Idle函数正确改为返回I2C_STAT_IDLE
*/

/*读时-写指令后的延时
*/
static int read_delay_us=0;
void I2C_SetReadDelayUs(int us)
{
  read_delay_us=us;
}
/*I2C io初始化
*/
void I2C_io_init(int TWIx)
{
    if(TWIx==0)
    {
        GPIO_Congif(GPIOE,GPIO_Pin_11,GPIO_Mode_011,GPIO_PuPd_UP);
        GPIO_Congif(GPIOE,GPIO_Pin_12,GPIO_Mode_011,GPIO_PuPd_UP);
//      write32(0x01C20800+0xA4,0x0);
    }else if(TWIx==1){}
     else if(TWIx==2){}
}
/*clear_irq_flag
*/
void twi_clear_irq_flag(int TWIx)
{
    unsigned int temp;
    unsigned int reg_val = read32(TWI_CNTR_REG(TWIx));
    /* start and stop bit should be 0 */
    reg_val |= 1<<3;
    reg_val &= ~(1<<4 | 1<<5);
    write32(TWI_CNTR_REG(TWIx),reg_val);
    /* read two more times to make sure that interrupt flag does really be cleared */
    {

        temp = read32(TWI_CNTR_REG(TWIx));
        temp |= read32(TWI_CNTR_REG(TWIx));
    }
}

/*I2C设置速率
rate_hz<=400000
*/
void I2C_Set_Rate(int TWIx, u64_t rate)
{
    u64_t pclk = 102000000;//APB clock input
    s64_t freq, delta, best = 0x7fffffffffffffffLL;
    int tm = 15, tn = 7;
    int m, n;

    for(n = 0; n <= 7; n++)
    {
        for(m = 0; m <= 15; m++)
        {
            freq = pclk / (10 * (m + 1) * (1 << n));
            delta = rate - freq;
            if(delta >= 0 && delta < best)
            {
                tm = m;
                tn = n;
                best = delta;
            }
            if(best == 0)
                break;
        }
    }
    sysprintf("CLK_M= %d...\r\n",tm);
    sysprintf("CLK_n= %d...\r\n",tn);

    write32(TWI_CCR_REG(TWIx), ((tm & 0xf) << 3) | ((tn & 0x7) << 0));
}

/*I2C等待状态
*/
int f1c100s_i2c_wait_status(int TWIx)
{
    volatile int timeout=0;
    do {
        if((read32(TWI_CNTR_REG(TWIx)) & (1 << 3)))return read32(TWI_STAT_REG(TWIx));
        timeout++;
    } while(timeout<TIMEOUT);
    if(timeout>=TIMEOUT)sysprintf("i2c_wait timeout!\r\n");
    return I2C_STAT_BUS_ERROR;
}
/*等待总线空闲
*/
int Wait_Idle(int TWIx)
{
  volatile int timeout=0;
    do {
        if(read32(TWI_STAT_REG(TWIx))==I2C_STAT_IDLE)return I2C_STAT_IDLE;
        timeout++;
    } while(timeout<TIMEOUT);
    if(timeout>=TIMEOUT)sysprintf("I2C Wait_Idle timeout!\r\n");
    return I2C_STAT_BUS_ERROR;
}
/*I2C开始
*/
int f1c100s_i2c_start(int TWIx)
{
//  u32_t val;
  volatile int timeout=0;
  int reg=read32(TWI_CNTR_REG(TWIx));
  reg|=(1<<5);
    reg&=~(1<<3);
  write32(TWI_CNTR_REG(TWIx),reg);
    do
    {
        if(!(read32(TWI_CNTR_REG(TWIx)) & (1 << 5)))break;
        timeout++;
    } while(timeout<TIMEOUT);
    if(timeout>=TIMEOUT)sysprintf("i2c_start timeout!\r\n");
    return f1c100s_i2c_wait_status(TWIx);
}

/*I2C停止
*/
int f1c100s_i2c_stop(int TWIx)
{
//  u32_t val;
  volatile int timeout=0;
  int reg=read32(TWI_CNTR_REG(TWIx));
  reg|=(1<<4);
    reg&=~(1<<3);
  write32(TWI_CNTR_REG(TWIx),reg);
    do {
        if(!(read32(TWI_CNTR_REG(TWIx)) & (1 << 4)))break;
        timeout++;
    } while(timeout<TIMEOUT);
    if(timeout>=TIMEOUT)sysprintf("i2c_stop timeout!\r\n");
    return read32(TWI_STAT_REG(TWIx));
}
/*I2C发送数据
*/
int f1c100s_i2c_send_data(int TWIx, u8 dat)
{
    write32(TWI_DATA_REG(TWIx), dat);
    C_BIT(TWI_CNTR_REG(TWIx),3);

    return f1c100s_i2c_wait_status(TWIx);
}
/*I2C读
*/
int f1c100s_i2c_read(int TWIx, u8 addr,u8 *buff,int len)
{
    u8 * p = buff;

    if(f1c100s_i2c_send_data(TWIx, (addr << 1 | 1)) != I2C_STAT_TX_AR_ACK)
     {
         sysprintf("err:f1c100s_i2c_read-0\r\n");
         return -1;
     }
    S_BIT(TWI_CNTR_REG(TWIx),2);
    while(len > 0)
    {
        if(len == 1)
        {
            int reg=read32(TWI_CNTR_REG(TWIx));
            reg&=~(1<<2);
            reg&=~(1<<3);
            write32(TWI_CNTR_REG(TWIx),reg);
            if(f1c100s_i2c_wait_status(TWIx) != I2C_STAT_RXD_NAK)
             {
                 sysprintf("err:f1c100s_i2c_read-1\r\n");
                 return -1;
             }
        }
        else
        {
            C_BIT(TWI_CNTR_REG(TWIx),3);
            if(f1c100s_i2c_wait_status(TWIx) != I2C_STAT_RXD_ACK)
             {
                 sysprintf("err:f1c100s_i2c_read-2\r\n");
                 return -1;
             }
        }
        *p++ = read32(TWI_DATA_REG(TWIx));
        len--;
    }
    return 0;
}
/*I2C写数据+地址
*/
int f1c100s_i2c_write(int TWIx, u8 addr,u8 *buff,int len)
{
    u8 * p = buff;
    if(f1c100s_i2c_send_data(TWIx,(addr << 1)) != I2C_STAT_TX_AW_ACK)return -1;
    while(len > 0)
    {
         if(f1c100s_i2c_send_data(TWIx, *p++)!= I2C_STAT_TXD_ACK)
         {
             sysprintf("err:f1c100s_i2c_write()\r\n");
             return -1;
         }
         len--;
    }
    return 0;
}
/*I2C写数据+无地址
*/
int f1c100s_i2c_data_write(int TWIx,u8 *buff,int len)
{
    u8 * p = buff;
    while(len > 0)
    {
         if(f1c100s_i2c_send_data(TWIx, *p++)!= I2C_STAT_TXD_ACK)
         {
             sysprintf("err:f1c100s_i2c_data_write()\r\n");
             return -1;
         }
         len--;
    }
    return 0;
}
/*设置从机地址
*/
u8 slave_addr;
void I2C_Set_Slave_Addr(u8 addr)
{
    slave_addr=addr>>1;
}
/*I2C时钟化
*/
void I2C_clock_init(int TWIx)
{
#define CCU_Base_Address    (u32_t)0x01C20000
#define BUS_CLK_GATING_REG2 (CCU_Base_Address+0x068)
#define BUS_SOFT_RST_REG2   (CCU_Base_Address+0x2D0)
    write32(BUS_CLK_GATING_REG2,read32(BUS_CLK_GATING_REG2)|(1)<<(16+TWIx));
    write32(BUS_SOFT_RST_REG2,read32(BUS_SOFT_RST_REG2)|((1)<<(16+TWIx)));
    delay_ms(1);
}
/*IO测试-SDA-SCL 输出高低电平
*/
void IO_Test(int TWIx)
{
    S_BIT(TWI_LCR_REG(TWIx),0);
    S_BIT(TWI_LCR_REG(TWIx),2);
    while(1)
    {
        S_BIT(TWI_LCR_REG(TWIx),1);
        S_BIT(TWI_LCR_REG(TWIx),3);
        delay_us(1000);
        C_BIT(TWI_LCR_REG(TWIx),1);
        C_BIT(TWI_LCR_REG(TWIx),3);
        delay_us(1000);
    }
}
/*I2C初始化
*/
void I2C_Init(int TWIx)
{
//int i=0;
    sysprintf("TWI%d_Init...\r\n",TWIx);
  I2C_io_init(TWIx);
    I2C_clock_init(TWIx);
    S_BIT(TWI_CNTR_REG(TWIx), 6);//使能总线
    S_BIT(TWI_SRST_REG(TWIx), 0);//复位总线
}

//-------------------------------------------------------------------------------------------------------------
//------------------------------------24C256测试---------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
/*24c256 read_byte-------------------------
addr  读地址
addr_len 地址长度
buuf 缓存地址
len  读长度
*/
int read_byte(int TWIx,u16 addr,u8 addr_len,u8* buff,u16 len)
{
u8 addr_buff[2];
    if(addr_len==2)
    {
        addr_buff[0]=addr>>8;
        addr_buff[1]=addr&0xff;
    }else if(addr_len==1)
    {
        addr_buff[0]=addr&0xff;
        addr_buff[1]=0;
    }
    if(Wait_Idle(TWIx)==I2C_STAT_IDLE)
    {
        //写地址
        f1c100s_i2c_start(TWIx);
        f1c100s_i2c_write(TWIx,slave_addr,addr_buff,addr_len);
    //写指令后的延时-默认0
    if(read_delay_us>0)delay_us(read_delay_us);
        //读数据
        f1c100s_i2c_start(TWIx);
        f1c100s_i2c_read(TWIx,slave_addr,buff,len);
        f1c100s_i2c_stop(TWIx);
        return 0;
    }else sysprintf("BUS ERR!\r\n");
    return -1;
}
/*24c256 write_byte-------------------------
addr  写地址
addr_len 地址长度
buuf 缓存地址
len  写长度
*/
int write_byte(int TWIx,u16 addr,u8 addr_len,u8* buff,u16 len)
{
u8 addr_buff[2];
    if(addr_len==2)
    {
        addr_buff[0]=addr>>8;
        addr_buff[1]=addr&0xff;
    }else if(addr_len==1)
    {
        addr_buff[0]=addr&0xff;
        addr_buff[1]=0;
    }

    if(Wait_Idle(TWIx)==I2C_STAT_IDLE)
    {
        //写地址
        f1c100s_i2c_start(TWIx);
        f1c100s_i2c_write(TWIx,slave_addr,addr_buff,addr_len);
        //写数据
    f1c100s_i2c_data_write(TWIx,buff,len);
        f1c100s_i2c_stop(TWIx);
        return 0;
    }else sysprintf("BUS ERR!\r\n");
    return -1;
}

#if 1
/*I2C测试
*/
void AT24C256_Demo(void)
{
    int i,TWI=TWI0;
  u8 buff_in[10]={73,75,78,3,4,5,6,7,8,9};
  u8 buff_out[10]={0};

    sysprintf("I2C_Demo...\r\n");
    I2C_Init(TWI);
    I2C_Set_Rate(TWI,  Rate100K);//设置速率
    I2C_Set_Slave_Addr(0xA0);
  //EEPROM写入
  sysprintf("write...\r\n");
    write_byte(TWI,0,2,buff_in,3);
    //延时,等EEPROM完成
    delay_ms(10);
  //EEPROM读出
    sysprintf("read...\r\n");
    read_byte(TWI,0,2,buff_out,3);
    for(i=0;i<3;i++)sysprintf("read%d:%d\r\n",i,buff_out[i]);

    while(1);
}
#endif
