#ifndef __SYS_SPI_H__
#define __SYS_SPI_H__

#define CN_SPI0     0
#define CN_SPI1     1
#define CN_SPI_NUM  2

#define SPI0  0  //0x01C05000
#define SPI1  1  //0x01C06000

#ifdef __SPI_DMA_EN //SPI DMA使能
    #define SPI_DMA_EN 1
#endif

#if SPI_DMA_EN
  #include "sys_dma.h"
    extern DMA * SPI_TXdma;
    extern DMA * SPI_RXdma;
#endif
struct tagSpiReg
{
    u32 pad0;
    vu32 SPI_GCR;      //= 0x04,
    vu32 SPI_TCR;      //= 0x08,
    u32 pad1;
    vu32 SPI_IER;      //= 0x10,
    vu32 SPI_ISR;      //= 0x14,
    vu32 SPI_FCR;      //= 0x18,
    vu32 SPI_FSR;      //= 0x1c,
    vu32 SPI_WCR;      //= 0x20,
    vu32 SPI_CCR;      //= 0x24,
    u32 pad2[2];
    vu32 SPI_MBC;      //= 0x30,
    vu32 SPI_MTC;      //= 0x34,
    vu32 SPI_BCC;      //= 0x38,
    u32 pad3[113];
    vu32 SPI_TXD;      //= 0x200,
    u32 pad4[63];
    vu32 SPI_RXD;      //= 0x300,
};

enum {
    SPI_GCR = 0x04,
    SPI_TCR = 0x08,
    SPI_IER = 0x10,
    SPI_ISR = 0x14,
    SPI_FCR = 0x18,
    SPI_FSR = 0x1c,
    SPI_WCR = 0x20,
    SPI_CCR = 0x24,
    SPI_MBC = 0x30,
    SPI_MTC = 0x34,
    SPI_BCC = 0x38,
    SPI_TXD = 0x200,
    SPI_RXD = 0x300,
};

#define W25X_FastReadDual_4ADDR     0x3C

#define spi_get_addr(x) (0x01C05000+x*0x1000)
/* SPI GLOBAL Control Register Bit Fields & Masks,defualt value:0x0002_001C */
#define SPI_GLB_CTL_EN          (0x1 << 0) /* SPI module enable control 1:enable;0:disable;default:0 */
#define SPI_GLB_CTL_FUNC_MODE   (0x1 << 1) /* SPI function mode select 1:master;0:slave;default:0 */
#define SPI_GLB_CTL_T_PAUSE_EN  (0x1 << 7) /* Transmit Pause Enable;Master mode: 1-stop when RXFIFO full;0-ignore rxFIFO */
#define SPI_GLB_CTL_SOFT_RESET  (0x1 << 31)

/* default work mode3: pol = 1,pha = 1; */
#define SPI_CTL_PHA             (0x1 << 0) /* SPI Clock polarity control,  0: phase0,1: phase1;default:1  */
#define SPI_CTL_POL             (0x1 << 1) /* SPI Clock/Data phase control,0:low level idle,1:high level idle;default:1 */
#define SPI_POL_PHA_BIT_POS     (2)

#define SPI_CTL_SSPOL       (0x1 << 2) /* SPI Chip select signal polarity control,default: 1,low effective like this:~~|_____~~ */
#define SPI_CTL_SSCTL       (0x1 << 3) /* SPI chip select control,default 0:SPI_SSx remains asserted between SPI bursts,1:negate SPI_SSx between SPI bursts */

#define SPI_CTL_SS_MASK     (0x3 << 4) /* SPI chip select:00-SPI_SS0;01-SPI_SS1;10-SPI_SS2;11-SPI_SS3*/
#define SPI_SS_BIT_POS      (4)

/* bit5 reserved */
#define SPI_CTL_LMTF            (0x1 << 12) /* LSB/MSB transfer first select 0:MSB,1:LSB,default 0:MSB first */
#define SPI_SAMPLE_MODE     (0x1 << 13)

#define SPI_CTL_XCH         (0x1 << 31) /* Exchange burst default 0:idle,1:start exchange;when BC is zero,this bit cleared by SPI controller*/
/* 19 只有smart模式,去掉了normal模式 去掉宏 SPI_CTL_SMC  */
#define SPI_CTL_RAPID       (0x1 << 10) /* 1:enable,> 80MHz(spi nor flash);0:disable,<80MHz(default).*/

#define SPI_CTL_DDB         (0x1 << 9) /* Dummy burst Type,default 0: dummy spi burst is zero;1:dummy spi burst is one */
#define SPI_CTL_DHB         (0x1 << 8) /* Discard Hash Burst,default 0:receiving all spi burst in BC period 1:discard unused,fectch WTC bursts */

#define SPI_CTL_SS_CTRL     (0x1 << 6) /* SS output mode select default is 0:automatic output SS;1:manual output SS */
#define SPI_CTL_SS_LEVEL        (0x1 << 7) /* defautl is 1:set SS to high;0:set SS to low */

#define SPI_CTL_MASTER_SDC  (0x1 << 11) /* master sample data control, 1: delay--high speed operation;0:no delay. */
/* aw1620 control register 31-20bit reserved bit */

/* SPI Interrupt Register Bit Fields & Masks,default value:0x0000_0000 */
#define SPI_INTEN_RR        (0x1 << 0)  /* rxFIFO Ready Interrupt Enable,---used for immediately received,0:disable;1:enable */
#define SPI_INTEN_RE        (0x1 << 1)  /* rxFIFO ENPTY Interrupt Enable ---used for IRQ received */
#define SPI_INTEN_RF        (0x1 << 2)  /* rxFIFO Full Interrupt Enable ---seldom used */
#define SPI_INTEN_RO        (0x1 << 8)  /* rxFIFO Overflow Interrupt Enable ---used for error detect */
#define SPI_INTEN_RU        (0x1 << 9)  /* rxFIFO Underrun Interrupt Enable ---used for error detect */

#define SPI_INTEN_TR        (0x1 << 4)  /* txFIFO Ready Interrupt Enable,*/
#define SPI_INTEN_TE        (0x1 << 5)  /* txFIFO Empty Interrupt Enable ---seldom used */
#define SPI_INTEN_TF        (0x1 << 6) /* txFIFO Full Interrupt Enable ---seldom used */
#define SPI_INTEN_TO        (0x1 << 10) /* txFIFO Overflow Interrupt Enable ---used for error detect */
#define SPI_INTEN_TU        (0x1 << 11) /* txFIFO Underrun Interrupt Enable ---not happened */

#define SPI_INTEN_TC        (0x1 << 12) /* Transfer Completed Interrupt Enable  ---used */
#define SPI_INTEN_SSI   (0x1 << 13) /* SSI interrupt Enable,chip select from valid state to invalid state,for slave used only */

#define SPI_INTEN_ERR   (SPI_INTEN_TO|SPI_INTEN_RU|SPI_INTEN_RO) //NO txFIFO underrun
#define SPI_INTEN_MASK  ((0x7<<0)|(0x7<<4)|(0x3F<<8))


/* SPI Status Register Bit Fields & Masks,default value:0x0000_1B00 all bits are written 1 to clear 0 */
#define SPI_STAT_RR     (0x1 << 0) /* rxFIFO ready, 0:no valid data;1:more than 1 word in rxfifo */
#define SPI_STAT_RE     (0x1 << 1) /* rxFIFO ENPTY  in rxfifo */
#define SPI_STAT_RF     (0x1 << 2) /* rxFIFO full,0:not full;1:full */
#define SPI_STAT_RO     (0x1 << 8) /* rxFIFO overflow, 0: rxfifo is available;1:rxfifo has overflowed! */
#define SPI_STAT_RU     (0x1 << 9) /* rxFIFO underrun,fectch data with no data available in FIFO */
/* 7bit reserved */

#define SPI_STAT_TR     (0x1 << 4)
#define SPI_STAT_TE     (0x1 << 5)  /* txFIFO empty,0:txfifo contains one or more words;1:txfifo is empty.default value:1 */
#define SPI_STAT_TF     (0x1 << 6) /* txFIFO Full */
#define SPI_STAT_TO     (0x1 << 10) /* txFIFO overflow 0:not overflow;1:overflow */
#define SPI_STAT_TU     (0x1 << 11) /* txFIFO underrun 0:not underrun;1:undrrun */

#define SPI_STAT_TC     (0x1 << 12) /* Transfer Complete, 0:BUSY;1:transfer completed */
#define SPI_STAT_SSI        (0x1 << 13) /* SS Invalid Interrupt ,for slave used only */

#define SPI_STAT_MASK   ((0x7<<0)|(0x7<<4)|(0x3F<<8))

#define SPI_STAT_ERR        (SPI_STAT_TO|SPI_STAT_RU|SPI_STAT_RO) //Slave mode,no SPI_STAT_TU

#define SPI_DRQEN_TX    (0x1 << 24)
#define SPI_DRQEN_RX    (0x1 << 8)
#define SPI_RXDMA_DMODE (0x1 << 9)

#define SPI_TX_WL       (32)
#define SPI_RX_WL       (32)

//TX FIFO Enpty Request Trigger Level
#define SPI_TX_TRIG_LEVEL_HALF  (0x80 << 16)
#define SPI_RX_TRIG_LEVEL_HALF  (0x80 << 0)

#define SPI_DRQEN_MASK      ((0x1<<8)|(0x1<<24)|(0xff<<16)|0xff)

/* SPI Wait Clock Register Bit Fields & Masks,default value:0x0000_0000 */
#define SPI_WAIT_CLK_MASK   (0xFFFF << 0)   /* used only in master mode: Wait Between Transactions */
/* 31:16bit reserved */

/* SPI Wait Clock Register Bit Fields & Masks,default:0x0000_0002 */
#define SPI_CLKCTL_CDR2     (0xFF << 0)  /* Clock Divide Rate 2,master mode only : SPI_CLK = AHB_CLK/(2*(n+1)) */
#define SPI_CLKCTL_CDR1     (0xF  << 8)  /* Clock Divide Rate 1,master mode only : SPI_CLK = AHB_CLK/2^(n+1) */
#define SPI_CLKCTL_DRS      (0x1  << 12) /* Divide rate select,default,0:rate 1;1:rate 2 */
#define SPI_CLK_SCOPE       (SPI_CLKCTL_CDR2+1)
/* 31:13bits reserved */

/* SPI Burst Counter Register Bit Fields & Masks,default value: 0x0000_0000 */
/* master mode: when SMC = 1,BC specifies total burst number, Max length is 16Mbytes */
#define SPI_BC_BC_MASK      (0x7FFFFF << 0 ) /* Total Burst Counter,tx length + rx length  */
#define SPI_TRANSFER_SIZE   (SPI_BC_BC_MASK)

/* SPI Transmit Counter reigster default:0x0000_0000,Max length is 16Mbytes */
#define SPI_TC_WTC_MASK     (0x7FFFFF << 0) /* Write Transmit Counter,tx length, NOT rx length!!! */

/* SPI FIFO status register default is 0x0000_0000 */
#define SPI_FIFO_RXCNT      (0x3F << 0) /* rxFIFO counter,how many bytes in the rxFIFO */
#define SPI_RXCNT_BIT_POS   (0)
/* 15:7bits reserved */

#define SPI_FIFO_TXCNT     (0x3F << 16) /* txFIFO counter,how many bytes in the txFIFO */
#define SPI_TXCNT_BIT_POS     (16)

#define SPI_FIFO_CTL_RST_TXFIFO   (0x1 << 31) /* SPI reset rxFIFO write 1 automatic clear 0*/
#define SPI_FIFO_CTL_RST_RXFIFO   (0x1 << 15) /* SPI reset txFIFO write 1 automatic clear 0*/



extern int rx_dual_en; //0=RX单线读 1=RX双线读
#define set_rx_dual_enable()  rx_dual_en=1 //1=RX双线读
#define set_rx_dual_disable()  rx_dual_en=0 //失能双线读

//SPI模式
extern unsigned int spi_slave_mode_en;//=0主机模式，=1主机模式

void W25Q128_Demo(void);
void spi_init(int SPI);
void spi_spi_confing(int SPI,int clk);
#define spi_flash_init  spi_init
int sys_spi_flash_read(int SPI,int addr, void * buf, int count);
void SPI_Flash_Write(int SPI,unsigned char * pBuffer,unsigned int WriteAddr,unsigned int NumByteToWrite);
void set_max_clk(int clk);
//===============================================
int SPI_Flash_Erase_Block32K(int SPI,int addr);
int SPI_Flash_Erase_Block64K(int SPI,int addr);
int SPI_Flash_Read_UID(int SPI,unsigned char *rbuff);
unsigned char spi_nor_flash_read_device_id(int SPI);
int spi_nor_flash_read_jedec_id(int SPI);
int SPI_Flash_WriteSR(int SPI,char Reg,char Val);
int SPI_FLASH_Write_Enable(int SPI);
void SPI_Flash_Wait_Busy(int SPI);
//===============================================
void sys_spi_select(int SPI);
void sys_spi_deselect(int SPI);
int sys_spi_transfer(int SPI,void * txbuf, void * rxbuf, int len);
//===============================================
void spi_software_cs_inx(int inx);
void spi_software_cs_gpio_init(GPIO_TypeDef *port,unsigned int Pin) ;
void spi_enable_software_cs(void);
void spi_exit_software_cs(void);
//===============================================
void spi_set_slave_mode(void);
void spi_exit_slave_mode(void);
void spi_Master_mode_test(void);
void spi_slave_mode_test(void);
int read_rx_count(int SPI);
//===============================================
void spi_dma_delay(void);
void *spi_malloc(int size);
void spi_free(void *p);
void sys_spi_enable_drq(int SPI,int tx_drq_en,int rx_drq_en);
void sys_spi_set_count(int SPI,int txc,int len);

#endif
