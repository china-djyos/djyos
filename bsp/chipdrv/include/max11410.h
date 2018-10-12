// =============================================================================

// 文件名     ：Max11410.h
// 模块描述: 温度检测芯片Max11410的驱动
// 模块版本: V1.00
// 创建人员:
// 创建时间: 2017年8月31日
// =============================================================================

#ifndef MAX11410_H_
#define MAX11410_H_

#ifdef __cplusplus
extern "C" {
#endif

/*Register Map Define*/

//8-Bit Control Registers
#define Max11410_PD                   (0x00)//工作模式
#define Max11410_CONV_START           (0x01)//启动转换选择转换结果保存位置
#define Max11410_SEQ_START            (0x02)//选择序列起始地址
#define Max11410_CAL_START            (0x03)//选择校准类型并启动校准
#define Max11410_GP0_CTRL             (0x04)//控制GPIO0的行为
#define Max11410_GP1_CTRL             (0x05)//控制GPIO1的行为
#define Max11410_GP_CONV              (0x06)//GPIIO启动转换执行这条
#define Max11410_GP_SEQ_ADDR          (0x07)//GPIO事件序列起始地址
#define Max11410_FILTER               (0x08)//滤波器范围
#define Max11410_CTRL                 (0x09)//选择时钟源
#define Max11410_SOURCE               (0x0a)//选择励磁电流源阻抗选择
#define Max11410_MUX_CTRL0            (0x0b)//寄存器选择那些模拟输入链接
#define Max11410_MUX_CTRL1            (0x0c)//寄存器使匹配的励磁电流源
#define Max11410_MUX_CTRL2            (0x0d)//AIN0 到 AIN7，供到 VBIAS 源的连接
#define Max11410_PGA                  (0x0e)//PGA 寄存器控制信号路径
#define Max11410_WAIT_EXT             (0x0f)//这个寄存器延时等待命令的计数范围
#define Max11410_WAIT_START           (0x10)//这个寄存器写入将执行与时钟计数等于 '等待' 操作
//24-Bit Control, Data, and Status Regist
#define Max11410_PART_ID              (0x11)//芯片ID

#define Max11410_SYSC_SEL             (0x12)//选择系统校准寄存器A B
#define Max11410_SYS_OFF_A            (0x13)//系统A 偏移量校准值减去每个转换的结果
#define Max11410_SYS_OFF_B            (0x14)//系统 B 校准偏移量的值减去每个转换的结果
#define Max11410_SYS_GAIN_A           (0x15)//系统增益校准 A 值用于缩放偏移校正转换结果，范围-50%~~25%
#define Max11410_SYS_GAIN_B           (0x16)//系统增益校准 B 值用于缩放偏移校正转换结果，范围-50%~~25%
#define Max11410_SELF_OFF             (0x17)//自校准偏移量的值减去转换结果 二进制补码形式
#define Max11410_SELF_GAIN_1          (0x18)//可选自动增益校准寄存器
#define Max11410_SELF_GAIN_2          (0x19)//
#define Max11410_SELF_GAIN_4          (0x1a)//
#define Max11410_SELF_GAIN_8          (0x1b)//
#define Max11410_SELF_GAIN_16         (0x1c)//
#define Max11410_SELF_GAIN_32         (0x1d)//
#define Max11410_SELF_GAIN_64         (0x1e)//
#define Max11410_SELF_GAIN_128        (0x1f)//

#define Max11410_LTHRESH0             (0x20)//在 DATA0 寄存器中保存比较下限值。比较结果 TUR_0 状态位
#define Max11410_LTHRESH1             (0x21)//在 DATA1 寄存器中保存比较下限值。比较结果 TUR_1 状态位
#define Max11410_LTHRESH2             (0x22)//
#define Max11410_LTHRESH3             (0x23)//
#define Max11410_LTHRESH4             (0x24)//
#define Max11410_LTHRESH5             (0x25)//
#define Max11410_LTHRESH6             (0x26)//
#define Max11410_LTHRESH7             (0x27)//
#define Max11410_UTHRESH0             (0x28)//在 DATA0 寄存器中保存比较上限值。比较结果 TOR_0 状态位
#define Max11410_UTHRESH1             (0x29)//
#define Max11410_UTHRESH2             (0x2a)//
#define Max11410_UTHRESH3             (0x2b)//
#define Max11410_UTHRESH4             (0x2c)//
#define Max11410_UTHRESH5             (0x2d)//
#define Max11410_UTHRESH6             (0x2e)//
#define Max11410_UTHRESH7             (0x2f)//
#define Max11410_DATA0                (0x30)//ADC 转换结果存储在 DATA0
#define Max11410_DATA1                (0x31)//
#define Max11410_DATA2                (0x32)//
#define Max11410_DATA3                (0x33)//
#define Max11410_DATA4                (0x34)//
#define Max11410_DATA5                (0x35)//
#define Max11410_DATA6                (0x36)//
#define Max11410_DATA7                (0x37)//
#define Max11410_Status               (0x38)//可以读状态寄存器来确定设备的状态
#define Max11410_Status_IE            (0x39)//
//16-bit Sequencer Registers
#define Max11410_uC0                  (0x3A)//
#define Max11410_uC1                  (0x3B)//
#define Max11410_uC2                  (0x3C)//
#define Max11410_uC3                  (0x3D)//
#define Max11410_uC4                  (0x3E)//
#define Max11410_uC5                  (0x3F)//
#define Max11410_uC6                  (0x40)//
#define Max11410_uC7                  (0x41)//
#define Max11410_uC8                  (0x42)//
#define Max11410_uC9                  (0x43)//
#define Max11410_uC10                 (0x44)//
#define Max11410_uC11                 (0x45)//
#define Max11410_uC12                 (0x46)//
#define Max11410_uC13                 (0x47)//
#define Max11410_uC14                 (0x48)//
#define Max11410_uC15                 (0x49)//
#define Max11410_uC16                 (0x4A)//
#define Max11410_uC17                 (0x4B)//
#define Max11410_uC18                 (0x4C)//
#define Max11410_uC19                 (0x4D)//
#define Max11410_uC20                 (0x4E)//
#define Max11410_uC21                 (0x4F)//
#define Max11410_uC22                 (0x50)//
#define Max11410_uC23                 (0x51)//
#define Max11410_uC24                 (0x52)//
#define Max11410_uC25                 (0x53)//
#define Max11410_uC26                 (0x54)//
#define Max11410_uC27                 (0x55)//
#define Max11410_uC28                 (0x56)//
#define Max11410_uC29                 (0x57)//
#define Max11410_uC30                 (0x58)//
#define Max11410_uC31                 (0x59)//
#define Max11410_uC32                 (0x5A)//
#define Max11410_uC33                 (0x5B)//
#define Max11410_uC34                 (0x5C)//
#define Max11410_uC35                 (0x5D)//
#define Max11410_uC36                 (0x5E)//
#define Max11410_uC37                 (0x5F)//
#define Max11410_uC38                 (0x60)//
#define Max11410_uC39                 (0x61)//
#define Max11410_uC40                 (0x62)//
#define Max11410_uC41                 (0x63)//
#define Max11410_uC42                 (0x64)//
#define Max11410_uC43                 (0x65)//
#define Max11410_uC44                 (0x66)//
#define Max11410_uC45                 (0x67)//
#define Max11410_uC46                 (0x68)//
#define Max11410_uC47                 (0x69)//
#define Max11410_uC48                 (0x6A)//
#define Max11410_uC49                 (0x6B)//
#define Max11410_uC50                 (0x6C)//
#define Max11410_uC51                 (0x6D)//
#define Max11410_uC52                 (0x6E)//
#define Max11410_uCADDR               (0x6f)//当前正在执行命令的地址
//===============输入引脚定义==================================//
#define Max11410_Ain0      (0)
#define Max11410_Ain1      (1)
#define Max11410_Ain2      (2)
#define Max11410_Ain3      (3)
#define Max11410_Ain4      (4)
#define Max11410_Ain5      (5)
#define Max11410_Ain6      (6)
#define Max11410_Ain7      (7)
#define Max11410_Ain8      (8)
#define Max11410_Ain9      (9)
#define Max11410_Ain_NUll  (10)

struct Init_Reg8Bit
{
    u8 ADDR;
    u8 Reg;
};

struct Init_Reg16Bit
{
    u8 ADDR;
    u8 RegAddr;
    u8 Data;
};
struct Max11410_CfgTab
{
    struct Init_Reg8Bit  const* pt_Reg8Bit;
    u8 Reg8BitNum;
    struct Init_Reg16Bit const* pt_Reg16Bit;
    u8 Reg16BitNum;
};

// =============================================================================
//功能：初始化Max11410芯片，若采用连续读模式，则配置其模式为连续采样
//参数：参数：
//     BusName,总结名称，如“SPI1”
//返回：true = 成功初始化，false = 初始化失败
// =============================================================================
bool_t ModuleInstall_Max11410(char *BusName );
// =============================================================================
// 函数功能：读温度(注意：函数类型为float，必须声名，否则返回值出错)
// 输入参数： temp_x 采集通道号TEMP_X
// 返回值  ：温度值，为float类型；-100，TEMP_ERROR 表示出错
// =============================================================================
#define TEMP_1            1
#define TEMP_2            2
#define TEMP_3            3
#define TEMP_4            4
#define TEMP_ERROR        (-100)
float Max11410_TemperatureGet(u8 temp_x);

#ifdef __cplusplus
}
#endif


#endif /* BOARD_SR5335_SRC_MAX11410_H_ */
