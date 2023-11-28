#ifndef __SYS_TEV_H__
#define __SYS_TEV_H__

#define Tve0  0

#define __u64  unsigned long
#define __u32  unsigned int
#define __s32  int
#define __u8  unsigned char
#define __s8  char
typedef enum
{
  DISP_TV_MOD_PAL_SVIDEO = 0,
    DISP_TV_MOD_PAL,
    DISP_TV_MOD_PAL_M,
    DISP_TV_MOD_PAL_M_SVIDEO,
    DISP_TV_MOD_PAL_NC,
    DISP_TV_MOD_PAL_NC_SVIDEO,
    DISP_TV_MOD_NTSC,
    DISP_TV_MOD_NTSC_SVIDEO,
    DISP_TV_MOD_480I,
    DISP_TV_MOD_576I,
    DISP_TV_MOD_480P,
    DISP_TV_MOD_576P,
    DISP_TV_MOD_720P_50HZ,
    DISP_TV_MOD_720P_60HZ,
    DISP_TV_MOD_1080I_50HZ,
    DISP_TV_MOD_1080I_60HZ,
    DISP_TV_MOD_1080P_50HZ,
    DISP_TV_MOD_1080P_60HZ,
}__tve_mode_t;


/* tv encoder registers offset */
#define TVE_000    (0x000)
#define TVE_004    (0x004)
#define TVE_008    (0x008)
#define TVE_00C    (0x00c)
#define TVE_010    (0x010)
#define TVE_014    (0x014)
#define TVE_018    (0x018)
#define TVE_01C    (0x01c)
#define TVE_020    (0x020)
#define TVE_024    (0x024)
#define TVE_030    (0X030)
#define TVE_034    (0x034)
#define TVE_038    (0x038)
#define TVE_03C    (0x03c)
#define TVE_040    (0x040)
#define TVE_044    (0x044)
#define TVE_048    (0x048)
#define TVE_04C    (0x04c)
#define TVE_0F8    (0x0f8)
#define TVE_0FC    (0x0fc)
#define TVE_100    (0x100)
#define TVE_104    (0x104)
#define TVE_108    (0x108)
#define TVE_10C    (0x10c)
#define TVE_110    (0x110)
#define TVE_114    (0x114)
#define TVE_118    (0x118)
#define TVE_11C    (0x11c)
#define TVE_120    (0x120)
#define TVE_124    (0x124)
#define TVE_128    (0x128)
#define TVE_12C    (0x12c)
#define TVE_130    (0x130)
#define TVE_134    (0x134)
#define TVE_138    (0x138)
#define TVE_13C    (0x13C)
#define TVE_300    (0x300)
#define TVE_304    (0x304)
#define TVE_308    (0x308)
#define TVE_380    (0x380)
#define TVE_3A0    (0x3a0)



extern __u32 tve_reg_base;
#define TVE_GET_REG_BASE()    (tve_reg_base)

#define TVE_WUINT8(offset,value)           (*((volatile __u8  *)(TVE_GET_REG_BASE()+offset))=(value))
#define TVE_RUINT8(offset)                 (*((volatile __u8  *)(TVE_GET_REG_BASE()+offset)))
#define TVE_WUINT16(offset,value)          (*((volatile __u16 *)(TVE_GET_REG_BASE()+offset))=(value))
#define TVE_RUINT16(offset)                (*((volatile __u16 *)(TVE_GET_REG_BASE()+offset)))
#define TVE_WUINT32(offset,value)          (*((volatile __u32 *)(TVE_GET_REG_BASE()+offset))=(value))
#define TVE_RUINT32(offset)                (*((volatile __u32 *)(TVE_GET_REG_BASE()+offset)))

#define TVE_WUINT8IDX(offset,index,value)  (*((volatile __u8  *)(TVE_GET_REG_BASE()+offset+index))=(value))
#define TVE_RUINT8IDX(offset,index)        (*((volatile __u8  *)(TVE_GET_REG_BASE()+offset+index)))
#define TVE_WUINT16IDX(offset,index,value) (*((volatile __u16 *)(TVE_GET_REG_BASE()+offset+2*index))=(value))
#define TVE_RUINT16IDX(offset,index)       (*((volatile __u16 *)(TVE_GET_REG_BASE()+offset+2*index)))
#define TVE_WUINT32IDX(offset,index,value) (*((volatile __u32 *)(TVE_GET_REG_BASE()+offset+4*index))=(value))
#define TVE_RUINT32IDX(offset,index)       (*((volatile __u32 *)(TVE_GET_REG_BASE()+offset+4*index)))

#define TVE_SET_BIT(offset,bit)            (*((volatile __u32 *)(TVE_GET_REG_BASE()+offset)) |= (bit))
#define TVE_CLR_BIT(offset,bit)            (*((volatile __u32 *)(TVE_GET_REG_BASE()+offset)) &= (~(bit)))

void TVE_Init(void);
void TVE_Demo(void);


#endif
