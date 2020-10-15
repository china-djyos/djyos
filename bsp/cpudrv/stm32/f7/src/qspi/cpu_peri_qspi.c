/*
 * cpu_peri_qspi.c
 *
 *  Created on: 2020��5��6��
 *      Author: CK
 */


// =============================================================================
#include "string.h"
#include "djyos.h"
#include "cpu_peri.h"
#include <board.h>
#include <dbug.h>
// =============================================================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�


//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_QSPI(void);
//    ModuleInstall_QSPI();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip qspi"    //Ƭ��sdcard����
//parent:"none"                         //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                      //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:none"                      //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SDCARD == false )
//#warning  " cpu_onchip_uart  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SDCARD    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,255,
#define CFG_QSPI_CLOCK_PRESCALER    2   //����QSPI��ʱ��ΪAHBʱ�ӵ�1/(CFG_QSPI_CLOCK_PRESCALER + 1)
//%$#@num,0,31,
#define CFG_QSPI_FIFO_THRESHOLD_LEVEL    4   //����QSPI��FIFO����ֵΪCFG_QSPI_FIFO_THRESHOLD_LEVEL
#define CFG_QSPI_FLASH_SIZE              24  //Flash �е��ֽ��� = 2^[FSIZE+1]
//%$#@num,1,8,
#define CFG_QSPI_CHIP_SELECT_HIGH_TIME   4  //Ƭѡ�ߵ�ƽʱ��
//%$#@enum,1,2
#define CFG_QSPI_FLASH_ID    1   //��QSPI֧�ֿ�������flash������ѡ�񣬽ӵ����ĸ�flash��
#define CFG_QSPI_DDR_HOLD    1    //1��ʹ��ģ���ӳ����ӳ��������,2����������ӳ� 1/4 �� QUADSPI ���ʱ�����ڡ�
//%$#@enum,0,3
#define CFG_QSPI_CK_MODE     0   //ʱ��ģʽ0����3
#define CFG_QSPI_ALTERNATE_BYTES_MODE     0   //�����ֽ�ģʽ 0:�޽����ֽ�,1:���ߴ��佻���ֽ� ,2:˫�ߴ��佻���ֽ� ,3:���ߴ��佻���ֽ�
#define CFG_QSPI_ALTERNATE_BYTES_SIZE     0   //�����ֽڳ��� 0:8 λ�����ֽ�,1:16 λ�����ֽ� ,2:24 λ�����ֽ� ,3:32 λ�����ֽ�
//%$#@enum,true,false
#define CFG_QSPI_DUAL_FLASH_MODE    false   //�Ƿ���˫����ģʽ��
#define CFG_QSPI_SAMPLE_SHIFT       true    //�Ƿ������λ������ڡ�
#define CFG_QSPI_SIOO               false    //�Ƿ������ָ��һ��ģʽ��
#define CFG_QSPI_DDR_ENABLE         false    //�Ƿ�ʹ��DDR��
//%$#@num,0,4294967295,
#define CFG_QSPI_TIMEOUT           5000  //QSPIִ�еĳ�ʱʱ��
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================



#define     REG     0

QSPI_HandleTypeDef QSPI_Handler;


#define     QSPI_SR_TCF     1 << 1      //������ɱ�־
#define     QSPI_SR_FTF     1 << 2      //FIFO��ֵ��־
#define     QSPI_SR_BUSY    1 << 5      //æ
//-----------------------------------------------------------------------------
//����: �ȴ�״̬��־
//����: flag�� �ȴ��ı�־λ��state���ȴ���״̬��time���ȴ�����
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t QSPI_WaitFlag(u32 flag, u8 state, u32 time)
{
    u8 now_state=0;
    while(time)
    {
        now_state = (QUADSPI->SR & flag) ? 1 : 0;
        if(now_state == state)
            break;
        time--;
    }
    if(time)
        return true;
    else
        return false;
}

void QSPI_HandingErrorState(void)
{
    u8 fifo_data = 0;
    u8 read_fifo;
    __IO u32 *data_reg = &QUADSPI->DR;

    if(QSPI_Handler.State == HAL_QSPI_STATE_ERROR)
    {
        notice_printf("QSPI", " handing qspid error state.\r\n");
        while(QSPI_WaitFlag(QSPI_SR_BUSY, 0, 0xffff) == false)
        {
            while(QSPI_WaitFlag(QSPI_SR_FTF, 0, 0xffff) == false)
            {
                fifo_data = (QUADSPI->SR >> 8);
                if(fifo_data > 0x20)
                    printf(" >20 ");
                while(fifo_data > 0)
                {
                    read_fifo = *(__IO u8 *)data_reg;
                    fifo_data --;
                }

            }
            printf(" .");
        }
        QSPI_Handler.State = HAL_QSPI_STATE_READY;
    }
}


//-----------------------------------------------------------------------------
//����: �����
//����: cmd�����͵����addr��Ŀ���ַ��mode��ģʽ��dmcycle����ָ��������
//  mode[1:0]:ָ��ģʽ;00,��ָ��;01,���ߴ���ָ��;10,˫�ߴ���ָ��;11,���ߴ���ָ��
//  mode[3:2]:��ַģʽ;00,�޵�ַ;01,���ߴ����ַ;10,˫�ߴ����ַ;11,���ߴ����ַ
//  mode[5:4]:��ַ����;00,8λ��ַ;01,16λ��ַ;10,24λ��ַ;11,32λ��ַ.
//  mode[7:6]:����ģʽ;00,������;01,���ߴ�������;10,˫�ߴ�������;11,���ߴ�������.
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
#if REG
bool_t QSPI_SendCMD(u8 cmd,u32 addr,u8 mode,u8 dmcycle)
#else
bool_t QSPI_Send_CMD(u32 instruction,u32 addr,u32 dummy_cycles,u32 instruction_mode,u32 addr_mode,u32 addr_size,u32 data_mode)
#endif
{
#if REG
    u32 reg;

    if(QSPI_WaitFlag(QSPI_SR_BUSY, 0 , 0XFFFF))
    {
        reg = 0 << 31;      //��ֹDDRģʽ
        reg |= CFG_QSPI_SIOO << 28;     //ÿ�ζ�����ָ��
        reg |= 0 << 26;     //���дģʽ
        reg |= ((u32)mode>>6) << 24;     //��������ģʽ
        reg |= (u32)dmcycle << 18;       //���ÿ�ָ��������
        reg |= ((u32)mode>>4 & 0x03) << 12;     //���õ�ַ����
        reg |= ((u32)mode>>2 & 0x03) << 10;     //���õ�ַģʽ
        reg |= ((u32)mode>>0 & 0x03) << 8;      //����ָ��ģʽ
        reg |= ((u32)cmd & 0xff);        //����ָ��
        QUADSPI->CCR = reg;

        if(mode & 0x0c)
        {
            QUADSPI->AR = addr;     //���õ�ַ�Ĵ���
        }
        if((mode & 0xc0) == 0)
        {
            if(QSPI_WaitFlag(QSPI_SR_TCF, 1, 0xFFFF))   //�ȴ��������
            {
                QUADSPI->FCR |= QSPI_SR_TCF;    //���TCF��־λ
            }
        }
        return  true;
    }

    return  false;
#else
    QSPI_CommandTypeDef Cmdhandler;
    HAL_StatusTypeDef ret = HAL_ERROR;
    u8 i = 0;

    Cmdhandler.Instruction=instruction;     //ָ��
    Cmdhandler.Address=addr;             //��ַ
    Cmdhandler.DummyCycles=dummy_cycles;     //��������
    Cmdhandler.InstructionMode=instruction_mode; //ָ��ģʽ
    Cmdhandler.AddressMode=addr_mode;         //��ַģʽ
    Cmdhandler.AddressSize=addr_size;         //��ַ����
    Cmdhandler.DataMode=data_mode;               //����ģʽ
    if(CFG_QSPI_SIOO)
        Cmdhandler.SIOOMode=QSPI_SIOO_INST_ONLY_FIRST_CMD;   //��Ϊ��һ�������ָ��
    else
        Cmdhandler.SIOOMode=QSPI_SIOO_INST_EVERY_CMD;   //ÿ�ζ�����ָ��

    switch(CFG_QSPI_ALTERNATE_BYTES_MODE)
    {
        case 0:
            Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_NONE; //�޽����ֽ�
            break;
        case 1:
            Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_1_LINE; //���ߴ��佻���ֽ�
            break;
        case 2:
            Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_2_LINES; //˫�ߴ��佻���ֽ�
            break;
        case 3:
            Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_4_LINES; //���ߴ��佻���ֽ�
            break;
        default:
            error_printf("QSPI","Alternate byte mode error.\r\n");
           break;
    }

    switch(CFG_QSPI_ALTERNATE_BYTES_SIZE)
    {
        case 0:
            Cmdhandler.AlternateBytesSize=QSPI_ALTERNATE_BYTES_8_BITS; //8λ���ֽ�
            break;
        case 1:
            Cmdhandler.AlternateBytesSize=QSPI_ALTERNATE_BYTES_16_BITS; //16λ���ֽ�
            break;
        case 2:
            Cmdhandler.AlternateBytesSize=QSPI_ALTERNATE_BYTES_24_BITS; //24λ���ֽ�
            break;
        case 3:
            Cmdhandler.AlternateBytesSize=QSPI_ALTERNATE_BYTES_32_BITS; //32λ���ֽ�
            break;
        default:
            error_printf("QSPI","Alternate byte size error.\r\n");
           break;
    }

    if(CFG_QSPI_DDR_ENABLE)
        Cmdhandler.DdrMode=QSPI_DDR_MODE_ENABLE;               //��DDR
    else
        Cmdhandler.DdrMode=QSPI_DDR_MODE_DISABLE;               //�ر�DDR
    if(CFG_QSPI_DDR_HOLD == 1)
        Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_ANALOG_DELAY;
    else
        Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_HALF_CLK_DELAY;

    ret = HAL_QSPI_Command(&QSPI_Handler,&Cmdhandler,CFG_QSPI_TIMEOUT);
    if(ret != HAL_OK)
    {
        while(i++ < 3)
        {
            if(ret != HAL_OK)
            {
                QSPI_HandingErrorState();
            }
            else
                break;
            ret = HAL_QSPI_Command(&QSPI_Handler,&Cmdhandler,CFG_QSPI_TIMEOUT);
        }
    }

    if(ret == HAL_OK)
        return  true;
    else
    {
        error_printf("qspi", "cmd send fail\r\n");
        return  false;
    }
#endif
}

//-----------------------------------------------------------------------------
//����: QSPI����ָ�����ȵ�����
//����: buf�� �������ݻ�������len�����ݳ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t QSPI_Receive(u8 *buf,u32 len)
{
#if REG
    u32 reg = QUADSPI->CCR;
//    u32 addr_reg = QUADSPI->AR;
    bool_t sta = false;

    QUADSPI->DLR = len - 1;     //�������ݴ��䳤��
    reg &= ~(3 << 26);          //���ԭ���Ĺ���ģʽ
    reg |= 1 << 26;             //����Ϊ��Ӷ�����ģʽ
    QUADSPI->FCR |= QSPI_SR_TCF;    //���TCF��־λ
    QUADSPI->CCR = reg;
//    QUADSPI->AR = addr_reg;
    while(len)
    {
        sta = QSPI_WaitFlag(QSPI_SR_TCF | QSPI_SR_FTF, 1, 0xFFFF);
        if(sta)
        {
            *buf++ =  *(u8 *)QUADSPI->DR;
            len --;
        }
        else
            break;
    }
    if(sta)
    {
        QUADSPI->CR |= 1 << 1;
        sta = QSPI_WaitFlag(QSPI_SR_TCF, 1, 0xFFFF);
        if(sta)
        {
            QUADSPI->FCR |= QSPI_SR_TCF;    //���TCF��־λ
            sta = QSPI_WaitFlag(QSPI_SR_BUSY, 0, 0xFFFF);
        }
    }
    return sta;
#else

    u8 i = 0;
    HAL_StatusTypeDef ret = HAL_ERROR;
    __IO u32 *data_reg = &QUADSPI->DR;

    QSPI_Handler.Instance->DLR=len-1;       //�������ݳ���

    ret = HAL_QSPI_Receive(&QSPI_Handler, buf, CFG_QSPI_TIMEOUT);
    if(ret != HAL_OK)
    {
        while(i++ < 3)
        {
            if(ret != HAL_OK)
            {
                QSPI_HandingErrorState();
            }
            else
                break;
            ret = HAL_QSPI_Receive(&QSPI_Handler, buf, CFG_QSPI_TIMEOUT);
        }
    }

    if(ret == HAL_OK)
    {
        return true;
    }
    else
    {
        error_printf("qspi", "receive error\r\n");
        return false;
    }
#endif
}

//-----------------------------------------------------------------------------
//����: QSPI����ָ�����ȵ�����
//����: buf�� �������ݻ�������len�����ݳ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
u8 QSPI_Transmit(u8 *buf,u32 len)
{
#if REG
    u32 reg = QUADSPI->CCR;
//    u32 addr_reg = QUADSPI->AR;
    bool_t sta = false;

    QUADSPI->DLR = len - 1;     //�������ݴ��䳤��
    reg &= ~(3 << 26);          //���ԭ���Ĺ���ģʽ
    reg |= 0 << 26;             //����Ϊ���д����ģʽ
    QUADSPI->FCR |= QSPI_SR_TCF;    //���TCF��־λ
    QUADSPI->CCR = reg;
//    QUADSPI->AR = addr_reg;
    while(len)
    {
        sta = QSPI_WaitFlag(QSPI_SR_FTF, 1, 0xFFFF);
        if(sta)
        {
            *(u8 *)QUADSPI->DR = *buf++;
            len --;
        }
        else
            break;
    }
    if(sta)
    {
        QUADSPI->CR |= 1 << 1;
        sta = QSPI_WaitFlag(QSPI_SR_TCF, 1, 0xFFFF);
        if(sta)
        {
            QUADSPI->FCR |= QSPI_SR_TCF;    //���TCF��־λ
            sta = QSPI_WaitFlag(QSPI_SR_BUSY, 0, 0xFFFF);
        }
    }
    return sta;
#else
    u8 i = 0;
    HAL_StatusTypeDef ret = HAL_ERROR;

    QSPI_Handler.Instance->DLR=len-1;

    ret = HAL_QSPI_Transmit(&QSPI_Handler,buf,CFG_QSPI_TIMEOUT);
    if(ret != HAL_OK)
    {
        while(i++ < 3)
        {
            if(ret != HAL_OK)
            {
                QSPI_HandingErrorState();
            }
            else
                break;
            ret = HAL_QSPI_Transmit(&QSPI_Handler,buf,CFG_QSPI_TIMEOUT);
        }
    }

    if(ret == HAL_OK)
    {
        return true;
    }
    else
    {
        error_printf("qspi", "transmit error\r\n");
        return false;
    }
#endif
}
//-----------------------------------------------------------------------------
//����: QSPI��ʼ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t QSPI_Init(void)
{
#if REG
    u32 reg = 0;

    QSPI_GpioInit();
    if(QSPI_WaitFlag(QSPI_SR_BUSY, 0 , 0XFFFF))
    {
        reg= CFG_QSPI_CLOCK_PRESCALER << 24;   //����QSPI��ʱ��ΪAHBʱ�ӵ�1/3,216/3 = 72Mhz = 13.8ns
        reg|= CFG_QSPI_FIFO_THRESHOLD_LEVEL << 8;   //FIFO����ֵΪ4
        if(CFG_QSPI_FLASH_ID == 1)
            reg|= 0 << 7;   //ѡ��FLASH1
        else
            reg|= 1 << 7;   //ѡ��FLASH2

        if(CFG_QSPI_DUAL_FLASH_MODE)
            reg|= 1 << 6;   //ʹ��˫����ģʽ
        else
            reg|= 0 << 6;   //��ֹ˫����ģʽ

        if(CFG_QSPI_SAMPLE_SHIFT)
            reg|= 1 << 4;   //������λ�������
        else
            reg|= 0 << 4;   //������������λ
        QUADSPI->CR = reg;

        reg = CFG_QSPI_FLASH_SIZE << 16; //����flash��СΪ 2^25 = 32M
        reg|= (CFG_QSPI_CHIP_SELECT_HIGH_TIME - 1) << 8;   //����Ƭѡ�ߵ�ƽʱ��Ϊ4��ʱ��13.8ns*4=55.2ns�����ֲ�
        if(CFG_QSPI_CK_MODE)
            reg|= 1 << 0;   //nCS Ϊ�ߵ�ƽ��Ƭѡ�ͷţ�ʱ�� CLK ���뱣�ָߵ�ƽ�����Ϊģʽ 3
        else
            reg|= 0 << 0;
        QUADSPI->DCR = reg;
        QUADSPI->CR |= 1 << 0;  //ʹ��QSPI
    }
    else
        return false;

    return true;
#else
    QSPI_Handler.Instance=QUADSPI;
    QSPI_Handler.Init.ClockPrescaler=CFG_QSPI_CLOCK_PRESCALER;     //����QSPI��ʱ��,216/(CFG_QSPI_CLOCK_PRESCALER+1),���CFG_QSPI_CLOCK_PRESCALER=2�Ļ���clk = 72Mhz = 13.8ns
    QSPI_Handler.Init.FifoThreshold=CFG_QSPI_FIFO_THRESHOLD_LEVEL;      //FIFO����ֵΪ

    if(CFG_QSPI_SAMPLE_SHIFT)
        QSPI_Handler.Init.SampleShifting=QSPI_SAMPLE_SHIFTING_HALFCYCLE;    //������λ�������
    else
        QSPI_Handler.Init.SampleShifting=QSPI_SAMPLE_SHIFTING_NONE;    //������������λ

    QSPI_Handler.Init.FlashSize=CFG_QSPI_FLASH_SIZE; //����flash��СΪ 2^(CFG_QSPI_FLASH_SIZE + 1)

    switch(CFG_QSPI_CHIP_SELECT_HIGH_TIME)
    {
        case 1:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;   //����Ƭѡ�ߵ�ƽʱ��Ϊ1��ʱ��13.8ns*1
            break;
        case 2:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;   //����Ƭѡ�ߵ�ƽʱ��Ϊ2��ʱ��13.8ns*2
            break;
        case 3:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_3_CYCLE;   //����Ƭѡ�ߵ�ƽʱ��Ϊ3��ʱ��13.8ns*3
            break;
        case 4:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE; //����Ƭѡ�ߵ�ƽʱ��Ϊ4��ʱ��13.8ns*4
            break;
        case 5:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE; //����Ƭѡ�ߵ�ƽʱ��Ϊ4��ʱ��13.8ns*5
            break;
        case 6:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE; //����Ƭѡ�ߵ�ƽʱ��Ϊ4��ʱ��13.8ns*6
            break;
        case 7:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_7_CYCLE; //����Ƭѡ�ߵ�ƽʱ��Ϊ4��ʱ��13.8ns*7
            break;
        case 8:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_8_CYCLE; //����Ƭѡ�ߵ�ƽʱ��Ϊ4��ʱ��13.8ns*8
            break;
        default:
            error_printf("QSPI","Chip select high time error.\r\n");
           break;
    }
    if(CFG_QSPI_CK_MODE)
        QSPI_Handler.Init.ClockMode=QSPI_CLOCK_MODE_3;  //ģʽ 3
    else
        QSPI_Handler.Init.ClockMode=QSPI_CLOCK_MODE_0;  //ģʽ 0

    if(CFG_QSPI_FLASH_ID == 1)
        QSPI_Handler.Init.FlashID=QSPI_FLASH_ID_1;  //ѡ��FLASH1
    else
        QSPI_Handler.Init.FlashID=QSPI_FLASH_ID_2;

    if(CFG_QSPI_DUAL_FLASH_MODE)
        QSPI_Handler.Init.DualFlash=QSPI_DUALFLASH_ENABLE;
    else
        QSPI_Handler.Init.DualFlash=QSPI_DUALFLASH_DISABLE; //��ֹ˫����ģʽ

    if(HAL_QSPI_Init(&QSPI_Handler)==HAL_OK)
        return true;
    else
        return false;
#endif

}

//-----------------------------------------------------------------------------
//����: QSPIע��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t QSPI_DeInit(void)
{
    if(HAL_QSPI_DeInit(&QSPI_Handler) == HAL_OK)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------------
//����: ��װQSPI
//����: ��
//����: -1 -- ʧ��; 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_QSPI(void)
{
    RCC->AHB3ENR|=1<<1;  //ʹ��QSPIʱ��
    if(false == QSPI_Init())
        return (-1);
    return (0);
}
