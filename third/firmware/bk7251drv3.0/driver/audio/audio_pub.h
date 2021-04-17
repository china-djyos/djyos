#ifndef __AUDIO_PUB_H__
#define __AUDIO_PUB_H__

#include "common/typedef.h"

#define AUD_FAILURE                  (1)
#define AUD_SUCCESS                  (0)

#define AUD_DAC_DEV_NAME             "aud_dac"
#define AUD_DAC_CMD_MAGIC            (0x1EBC0000)

typedef struct aud_dac_cfg_st
{
    UINT8 *buf;
    UINT16 buf_len;
    UINT16 freq;
    UINT16 channels;     
    UINT16 dma_mode;
    UINT16 mute_pin;
    UINT16 def_volume;
    #if CFG_GENERAL_DMA
    void (*buf_finish_cb)(UINT8 *buf, void *usr_data);
    void *usr_data;
    #endif
    
} AUD_DAC_CFG_ST, *AUD_DAC_CFG_PTR;

enum
{
    AUD_DAC_CMD_GET_FREE_BUF_SIZE = AUD_DAC_CMD_MAGIC + 1,
    AUD_DAC_CMD_PLAY,    
    AUD_DAC_CMD_PAUSE,
    AUD_DAC_CMD_SET_SAMPLE_RATE,
    AUD_DAC_CMD_SET_VOLUME,    
};

#include "gpio_pub.h"
#include "sys_ctrl_pub.h"
#define AUD_USE_EXT_PA                      0
#if AUD_USE_EXT_PA
#define AUD_DAC_MUTE_PIN                    GPIO9
#define AUD_DAC_MUTE_ENA_LEVEL              0
#endif

#define AUD_DAC_SINGLE_PORT         1
#define AUD_DAC_DIFF_PORT           2
#define AUD_DAC_USE_PORT_SET        AUD_DAC_SINGLE_PORT


///////////////////////////////////////////////////////////////////////////////


#define AUD_ADC_DEV_NAME             "aud_adc"
#define AUD_ADC_CMD_MAGIC            (0x2EBC0000)
typedef struct aud_adc_cfg_st
{
    UINT8 *buf;
    UINT16 buf_len;
    UINT16 freq;
    UINT16 channels;
    UINT16 mode;
    UINT32 linein_detect_pin;
} AUD_ADC_CFG_ST, *AUD_ADC_CFG_PTR;

#if CFG_SUPPORT_DJYOS	//CK 
enum
{
    AUD_ADC_CMD_GET_FILL_BUF_SIZE = AUD_ADC_CMD_MAGIC + 1,
    AUD_ADC_CMD_PLAY,    
    AUD_ADC_CMD_PAUSE,
    AUD_ADC_CMD_DO_LINEIN_DETECT,
    AUD_ADC_CMD_SET_SAMPLE_RATE,
    AUD_ADC_CMD_SET_VOLUME,
    AUD_ADC_CMD_SET_CHANNEL
};

typedef enum
{
    audio_sample_rate_11025 = 11025,
    audio_sample_rate_22050 = 22050,
    audio_sample_rate_44100 = 44100,
    audio_sample_rate_12000 = 12000,
    audio_sample_rate_24000 = 24000,
    audio_sample_rate_48000 = 48000,
    audio_sample_rate_8000  = 8000,
    audio_sample_rate_16000 = 16000,
    audio_sample_rate_32000 = 32000,
}audio_sample_rate_e;
#else
enum
{
    AUD_ADC_CMD_GET_FILL_BUF_SIZE = AUD_ADC_CMD_MAGIC + 1,
    AUD_ADC_CMD_PLAY,    
    AUD_ADC_CMD_PAUSE,
    AUD_ADC_CMD_DO_LINEIN_DETECT,
    AUD_ADC_CMD_SET_SAMPLE_RATE,
    AUD_ADC_CMD_SET_VOLUME
};
#endif

#define AUD_ADC_LINEIN_DETECT_PIN           GPIO8
#define AUD_ADC_LINEIN_ENABLE_LEVEL         0
#define AUD_ADC_DAC_HARDWARD_LOOPBACK       0

#define AUD_ADC_MODE_DMA_BIT                (1 << 0)  // 1: DMA MODE, 0: ISR MODE
#define AUD_ADC_MODE_LINEIN                 (1 << 1)  // 1: LINEIN, 0: MIC 
#define AUD_ADC_DEF_WR_THRED                (8)
#define AUD_ADC_DEF_GAIN                    (0x2D)   // 0dm
#define AUD_ADC_MAX_THRED                   (0x10)
#define AUD_ADC_MAX_VOLUME                  (124)

void audio_init(void);
void audio_exit(void);

#if CFG_SUPPORT_DJYOS	//CK 
UINT32 audio_adc_ctrl(UINT32 cmd, void *param);
UINT32 audio_adc_read(char *user_buf, UINT32 count, UINT32 op_flag);
UINT32 audio_adc_close(void);
UINT32 audio_adc_open(UINT32 op_flag);

UINT32 audio_dac_open(UINT32 op_flag);
UINT32 audio_dac_close(void);
UINT32 audio_dac_write(char *user_buf, UINT32 count, UINT32 op_flag);
UINT32 audio_dac_ctrl(UINT32 cmd, void *param);
#endif

#endif // __AUDIO_PUB_H__
