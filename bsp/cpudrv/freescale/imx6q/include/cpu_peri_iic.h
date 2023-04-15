#ifndef __CPU_PERI_I2C_H__
#define __CPU_PERI_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_IIC0  0

//offset of reg
typedef struct
{
  vu32 IICCON;
  vu32 IICSTAT;
  vu32 IICADD;
  vu32 IICDS;
  vu32 IICLC;
}tagI2CReg;

#define CN_IIC_ADDR_OFFSET  0x01        //最低位为读写标志
#define CN_IIC_WRITE_BIT    0x00        //读标志为0
#define CN_IIC_READ_BIT     0x01        //写标志为1

#define IICCON_ACKEN_MASK   ( 1 << 7)
#define IICCON_IICLK_MASK   ( 1 << 6)
#define IICCON_INT_MASK     ( 1 << 5)
#define IICCON_INTPENDINGFLAG_MASK (1 << 4)
#define IICCON_TXCLK_OFFSET (0)
#define IICCON_TXCLK(x)     ((x & 0xF) << IICCON_TXCLK_OFFSET)

#define IICCON_MS_OFFSET    (6)
#define IICSTAT_MS(x)       ((x & 0x3) << IICCON_MS_OFFSET)
#define IICSTAT_MS_SR_MODE  (0 << IICCON_MS_OFFSET)
#define IICSTAT_MS_ST_MODE  (1 << IICCON_MS_OFFSET)
#define IICSTAT_MS_MR_MODE  (2 << IICCON_MS_OFFSET)
#define IICSTAT_MS_MT_MODE  (3 << IICCON_MS_OFFSET)
#define IICSTAT_ACK_MASK    (1 << 0)
#define IICSTAT_START_STOP_MASK (1 << 5)
#define IICSTAT_TXRXEN_MASK (1 << 4)
#define IICSTAT_BSY_MASK            0x20

#define CN_IIC_CLK_100K     (100*1000)
bool_t ModuleInstall_IIC(u8 port);


#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_SPI_H__*/

