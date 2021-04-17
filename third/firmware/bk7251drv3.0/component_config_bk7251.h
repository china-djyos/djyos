
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"bk7251V3.0"       //ԭ���̼���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:third               //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"bk7251"                //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BK7251V3P0 == false )
//#warning  " bk7251  �������δ���ã�ʹ��Ĭ������"
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BK7251V3P0    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string,1,16
//%$#@select
//%$#@SYMBOL
#define CN_BEKEN_SDK_USE    1
#define CN_BEKEN_SDK_V3     1
//%$#@free
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
//./common;./alios;./os;
//./driver/codec;./driver/fft;
//./driver/i2c;./driver/irda;./driver/jpeg;./driver/rc_beken;
//./driver/sdio;./driver/spidma;./driver/usb;
//./driver/driver.c;./driver/SConscript;./driver/pwm/timer.c;
//./func/bk7011_cal/bk7011_cal.c;./func/bk7011_cal/bk7231_cal.c;./func/bk7011_cal/bk7231U_cal.c;
//./func/bk7011_cal/manual_cal_bk7231.c;./func/hostapd-2.5/src/crypto/crypto_ali.c;
//./func/lwip_intf;
//./func/user_driver/BkDriverGpio.c;./func/user_driver/BkDriverPwm.c;
//./func/user_driver/BkDriverUart.c;./func/user_driver/BkDriverWdg.c;./func/mem_arch.c;
//./func/wlan_ui/lsig_monitor.c;./func/voice_transfer;./func/vad;
//./func/saradc_intf;./func/misc;./func/music_player;./app/app_demo;
//./app/airkiss;./driver/entry;
//./ip;
//%$#@end exclude
//%$#@API include path         ****APIͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ�����Ӧ�ñ���ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//.;
//./app;./app/config;
//./app/standalone-ap;./app/standalone-station;
//./common;
//./demo;
//./driver;./djyos/include;
//./driver/ble;./driver/ble/ble_pub/modules/rwip/api;./driver/ble/ble_lib/ip/ble/ll/src/rwble;
//./driver/ble/ble_lib/ip/ble/hl/inc;./driver/ble/ble_lib/ip/ble/hl/api;
//./driver/ble/ble_pub/ip/ble/hl/inc;./driver/ble/ble_pub/plf/refip/src/arch;
//./driver/ble/ble_pub/modules/app/api;./driver/ble/ble_pub/plf/refip/src/arch/compiler;
//./driver/ble/ble_pub/plf/refip/src/build/ble_full/reg/fw;./driver/ble/ble_pub/plf/refip/src/arch/ll;
//./driver/ble/ble_pub/plf/refip/src/driver/reg;./driver/ble/ble_pub/modules/common/api;
//./driver/ble/ble_lib/ip/em/api;./driver/ble/ble_lib/ip/ble/ll/src/em;
//./driver/ble/ble_lib/modules/h4tl/api;./driver/ble/ble_pub/modules/dbg/api;
//./driver/ble/ble_lib/ip/ea/api;./driver/ble/ble_pub/modules/rf/api;
//./driver/ble/ble_pub/plf/refip/src/driver/ble_icu;./driver/ble/ble_lib/modules/ke/api;
//./driver/ble/ble_lib/ip/hci/api;./driver/ble/ble_pub/modules/ecc_p256/api;
//./driver/ble/ble_lib/ip/ble/ll/src/llc;./driver/ble/ble_lib/ip/ble/ll/src/llm;
//./driver/ble/ble_lib/ip/ble/ll/src/lld;./driver/ble/ble_lib/modules/ke/src;
//./driver/ble/ble_lib/ip/ahi/api;./driver/ble/ble_pub/ip/ble/profiles/sdp/api;
//./driver/ble/ble_pub/ip/ble/profiles/comm/api;./driver/ble/ble_lib/ip/ble/hl/src/gatt/atts;
//./driver/audio;./driver/common;./driver/common/reg;./driver/demo;./driver/sdcard;./driver/icu;
//./driver/dma;./driver/flash;./driver/include;./driver/gpio;./driver/intc;
//./driver/phy;./driver/pwm;./driver/sys_ctrl;./driver/uart;./driver/qspi;
//./func/ethernet_intf;./func/hostapd-2.5;./func/hostapd-2.5/bk_patch;
//./func/hostapd-2.5/hostapd;./func/hostapd-2.5/src;./func/hostapd-2.5/src/ap;
//./func/hostapd-2.5/src/common;./func/hostapd-2.5/src/drivers;./func/hostapd-2.5/src/utils;
//./ip/umac/src/apm;./func/include;./func/joint_up;
//./func/rwnx_intf;./func/temp_detect;
//./func/uart_debug;./func/user_driver;
//./ip/common;
//./ip/ke;
//./ip/lmac;./ip/lmac/src/rwnx;./ip/lmac/src/tx;./ip/lmac/src/tx/txl;
//./ip/lmac/src/hal;./ip/lmac/src/scan;./ip/lmac/src/mm;./ip/lmac/src/bfr;
//./ip/lmac/src/chan;./ip/lmac/src/p2p;./ip/lmac/src/rd;./ip/lmac/src/rx;
//./ip/umac/src/bam;./ip/lmac/src/sta;./ip/lmac/src/tpc;./ip/lmac/src/tdls;
//./ip/lmac/src/td;./ip/lmac/src/vif;./ip/lmac/src/ps;./ip/lmac/src/rx/rxl;
//./ip/mac;
//./ip/umac;./ip/umac/src/me;./ip/umac/src/mesh;
//./ip/umac/src/mfp;./ip/umac/src/rc;./ip/umac/src/rxu;./ip/umac/src/scanu;
//./ip/umac/src/llc;./ip/umac/src/sm;./ip/umac/src/txu;
//%$#@end API include path

//%$#@self include path         ****����������Ҫ��ͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ��������뱾��ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//%$#@end self include path

//%$#@library path             ****�г���������õ�.a���ļ��б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ���
//./ip/libip_7221u_sta.a;./driver/ble/libble.a
//%$#@end library path

//@#$%component end configure
