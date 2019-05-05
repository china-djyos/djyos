
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"wolfssh130"   //�������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:third               //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"wolfssl3157"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef BUILDING_WOLFSSL   //****�������Ƿ��Ѿ����ú�
//#warning    ���������á��������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,65535
#define SFTP_SERVER_DEFAULT_LISTEN_PORT        22222      //"ssh server listen port"
//%$#@enum,true,false
//%$#@select
//%$#@string,1,16
#define serverKeyRsaPemFile "/efs/server-key-rsa.pem"   //"rsa.pem file"����ʹ���ļ�ϵͳ����Ҫ����
#define serverKeyEccDerFile "/efs/server-key-ecc.der"   //"ecc.der file"
#define serverKeyRsaDerFile "/efs/server-key-rsa.der"   //"rsa.der file"

#define SFTP_SERVER_USERNAME_PASSWORD_STRINGS  "jill:upthehill\n jack:fetchapail\n" //"server username and password"
//%$#@SYMBOL
#define SIZEOF_LONG_LONG 8
#define NO_WRITEV
#define BUILDING_WOLFSSL
#define WOLFSSL_PUBLIC_MP
#define _POSIX_THREADS
#define HAVE_THREAD_LS
#define TFM_TIMING_RESISTANT
#define ECC_TIMING_RESISTANT
#define WC_RSA_BLINDING
#define HAVE_AESGCM
#define WOLFSSL_SHA512
#define WOLFSSL_SHA384
#define NO_DSA
#define HAVE_ECC
#define TFM_ECC256
#define ECC_SHAMIR
#define WOLFSSL_BASE64_ENCODE
#define NO_RC4
#define NO_HC128
#define NO_RABBIT
#define WOLFSSL_SHA224
#define WOLFSSL_SHA3
#define HAVE_POLY1305
#define HAVE_ONE_TIME_AUTH
#define HAVE_CHACHA
#define HAVE_HASHDRBG
#define HAVE_TLS_EXTENSIONS
#define HAVE_SUPPORTED_CURVES
#define HAVE_EXTENDED_MASTER
#define NO_PSK
#define NO_MD4
#define NO_PWDBASED
#define USE_FAST_MATH
#define WOLFSSL_X86_64_BUILD
#define WC_NO_ASYNC_THREADING
#define NO_DES3
#define HAVE___UINT128_T
#define BUILDING_WOLFSSH
#define HAVE_CYASSL_OPTIONS
#define WOLFSSH_SFTP
#define HAVE_CONFIG_H
#define NDEBUG

//%$#@free
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
./examples;./src/keygen.c;./src/misc.c;./tests;

//%$#@endexclude
//%$#@includepath
//./;
//%$#@endincludepath

