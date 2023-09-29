#include <stdint.h>
#include <stddef.h>

#include <gd32e103r_start.h>
#include <gd32e10x.h>

#include <os.h>
#include <cpu_peri.h>

#include "uartctrl.h"
#include "pcf8574.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//board featrue and drivers
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                       //ѡ�third��system��bsp��user����������������IDE�з���
//select:required                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                     //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                      //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","gd32e10x","cpu onchip gpio"//������������������������none����ʾ�������������
                                      //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//weakdependence:"none"               //��������������������������none����ʾ�������������
                                      //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                      //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
// ���ܣ����ݾ���İ�����ô��ڵ�GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      ��������ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ںʹ���ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_UartGpioInit(u8 SerialNo)
{
    rcu_periph_enum periph;
    uint32_t gpio_periph;
    uint32_t mode;
    uint32_t speed;
    uint32_t pin;

    uint32_t com = 0;

    com = SerialNo;
    if (com > CN_UART4) return false;

    /* debug: use usart3 for debug
        PC10 - White wire
        PC11 - Green wire */

    com = UART3;
    gpio_periph = GPIOC;

    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOC);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_UART3);

    /* connect port to USARTx_Tx */
    gpio_init(gpio_periph, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* connect port to USARTx_Rx */
    gpio_init(gpio_periph, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);

        while (1) {

                /* transmit */
                uint32_t usart_periph = UART3;
                static uint32_t data = 'U';
                /* receive */
                uint16_t ch;

                usart_data_transmit(usart_periph, data);
                while (usart_flag_get(usart_periph, USART_FLAG_TBE) == RESET);

                if (usart_flag_get(usart_periph, USART_FLAG_RBNE) == SET) {
                        ch = usart_data_receive(usart_periph);
                        data = ch;
                }
                /* ���Դ����շ� */
                break;
        };


    return true;
}

// =============================================================================
// ���ܣ����ݾ���İ������IIC��GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      IIC����ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ں�����ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_IicGpioInit(u8 I2Cx)
{

    return true;
}
// =============================================================================
// ���ܣ�������ص�gpio����
// ��������
// ���أ�true
// =============================================================================

bool_t Board_EthGpioInit(void)
{
//    GPIO_PowerOn(GPIO_A);
//    GPIO_PowerOn(GPIO_C);
//    GPIO_PowerOn(GPIO_D);
//    GPIO_PowerOn(GPIO_G);
//    RCC->APB2ENR|=1<<14;    //ʹ��SYSCFGʱ��
//    SYSCFG->PMC|=1<<23;     //ʹ��RMII�ӿ�


#if 0

    GPIO_CfgPinFunc(GPIO_A,PIN1|PIN2|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_D,PIN3,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_C,PIN1|PIN4|PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_G,PIN11|PIN13|PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_PU);

    GPIO_AFSet(GPIO_A,1,11);    //PA1,AF11
    GPIO_AFSet(GPIO_A,2,11);    //PA2,AF11
    GPIO_AFSet(GPIO_A,7,11);    //PA7,AF11

    GPIO_AFSet(GPIO_C,1,11);    //PC1,AF11
    GPIO_AFSet(GPIO_C,4,11);    //PC4,AF11
    GPIO_AFSet(GPIO_C,5,11);    //PC5,AF11

    GPIO_AFSet(GPIO_G,11,11);   //PG11,AF11
    GPIO_AFSet(GPIO_G,13,11);   //PG13,AF11
    GPIO_AFSet(GPIO_G,14,11);   //PG14,AF11

    GPIO_SettoLow(GPIO_D,PIN3);
    DJY_EventDelay(10*1000);
    GPIO_SettoHigh(GPIO_D,PIN3);
#endif
    return true;
}
// =============================================================================
// ���ܣ����ݾ���İ������SPI��GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      SPI����ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ں�����ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_SpiGpioInit(u8 SPIx)
{
    if(SPIx == CN_SPI1)
    {
//        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // ʹ��PORTBʱ��
//        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // ʹ��SPI1ʱ��
//
//        RCC->AHB1ENR |= 1<<6; // ʹ��PORTGʱ��

//##WIP##        RCC->APB2RSTR |= RCC_APB2RSTR_SPI1; // ��λSPI1
//##WIP##        RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1; // ֹͣ��λSPI1

#if 0
        GPIO_CfgPinFunc(GPIO_B,PIN4|PIN5|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU); // GPB3��4��5ΪSCK��MISO��MOSI
        GPIO_AFSet(GPIO_B,3,5);
        GPIO_AFSet(GPIO_B,4,5);
        GPIO_AFSet(GPIO_B,5,5);
// CS���豸ȷ��
        GPIO_CfgPinFunc(GPIO_B,PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU); // GPB14ΪCS����NOR

        GPIO_CfgPinFunc(GPIOG,PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_100M,GPIO_PUPD_PU); // GPG7ΪCS��������ģʽ
        GPIOG->ODR|=1<<7;

        GPIO_SettoHigh(GPIO_B,PIN14);// CSΪ��
#endif
    }
    else
    {
        return false;
    }
    return true;
}

// =============================================================================
// ���ܣ����ݾ���İ��485����Ŀ��ƹܽţ�����485Ϊ����״̬�ð����Ӳ����·ʵ���������
// ���������ں��磺CN_UART1
// ���أ���
// =============================================================================

void Board_UartHalfDuplexSend(u8 SerialNo)
{
    switch(SerialNo)
    {
        case CN_UART1:break;
        case CN_UART2:break;
        default:      break;
    }
}

// ============================================================================
// ���ܣ���ȡ�˿ڵ�����RS_485/RS_232 RS485 ����false
// ������SerialNo,���ں�
// ���أ�true/false
// ============================================================================
u8   Board_CheckPortIsRS232(u8 SerialNo)
{

    if(SerialNo==CN_UART1||SerialNo==CN_UART2)
        return false;
    return true;
}

 // =============================================================================
 // ���ܣ����ݾ���İ��485����Ŀ��ƹܽţ�����485Ϊ������״̬
 // ���������ں��磺CN_UART1
 // ���أ���
 // =============================================================================

void Board_UartHalfDuplexRecv(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART1: break;
    case CN_UART2: break;
    default:       break;
    }
}

// =============================================================================
// ���ܣ����ݶ˿ںſ���Ƭѡ
// ������SPIPort�˿ں�  level==0ʹ��ʹ�ܻ�  1ʧ��
// ���أ�true
// =============================================================================
#define BITBAND(addr, bitnum)       ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)      MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOB_ODR_Addr                (GPIOB+20) //0x40020414
#define PAout(n)                       BIT_ADDR(GPIOA_ODR_Addr,n)  //���
#define PBout(n)                       BIT_ADDR(GPIOB_ODR_Addr,n)  //���

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    switch(SPIPort)
    {
        case CN_SPI1:
            if(level)
                PBout(14) = 1;
            else
                PBout(14) = 0;
            break;
        default:  return false;
    }
    return true;
}
#pragma GCC diagnostic pop

static void Board_NORGpioInit (void )
{
#if 0
    Board_SpiGpioInit(CN_SPI1);
    GPIO_CfgPinFunc(GPIO_B, PIN14, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M,
                    GPIO_PUPD_PU); // GPB14ΪCS����NOR

    // RCC->AHB1ENR |= 1<<6; // ʹ��PORTGʱ��
    GPIO_CfgPinFunc(GPIOG,PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M,
                    GPIO_PUPD_PU); // GPG7ΪCS��������ģʽ
    // GPIOG->ODR |= 1<<7;

    GPIO_SettoHigh(GPIO_B,PIN14);// CSΪ��
#endif
}

void board_rest_init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOC);

    /* configure led GPIO port */
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_bit_set(GPIOC,GPIO_PIN_6);
}
void Board_RestInit  (void) __attribute__ ((weak, alias ("board_rest_init")));
void Board_Rest_Init (void) __attribute__ ((weak, alias ("board_rest_init")));


// =============================================================================
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//  �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
// ��������
// ���أ���
// =============================================================================

void Board_Init(void)
{
    Board_EthGpioInit();//����
    Board_UartGpioInit(CN_UART3);
    Board_NORGpioInit();
    Board_RestInit();
}

////���ڵ� PHY оƬ��LAN8720��λ
bool_t LAN8720_RESET(void)
{
//  PCF8574_WriteBit(ETH_RESET_IO,1);
//  DJY_DelayUs(100*mS);
//  PCF8574_WriteBit(ETH_RESET_IO,0);
//  DJY_DelayUs(100*mS);
    //PD3�����һ��100mS�ĸߵ�ƽ����
    return true;
}


