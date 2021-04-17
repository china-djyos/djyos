
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���


//#define ADD_TO_ROUTINE_SHELL(cmdname,fun,help) \
//                __attribute__((section(".ro_shell_cmd")))\
//                const struct shell_cmd djysh_##cmdname = \
//                        {.shell_fun_addr  = fun,.shell_help_addr = help,}
//#if BUILDING_SFTPC
//bool_t sftp_client(char *Param);
//ADD_TO_ROUTINE_SHELL(sftpc,sftp_client,"start the sftp client,usage: sftpc -h host -p port -u user -P password\n example:  sftpc -h 192.168.0.1 -p 2323 -u user -P user");
//#endif
//#if BUILDING_SFTPD
//bool_t sftp_server(char *Param);
//ADD_TO_ROUTINE_SHELL(sftpd,sftp_server,"start the sftp server,usage: sftpd -p port\n example:  sftpd -p 22222");
//#endif

//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"wolfssh130"  //�������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:third               //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"wolfssl3157"      //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_WOLFSSH130 == false )
//#warning  " wolfssh130  �������δ���ã�ʹ��Ĭ������"
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_WOLFSSH130    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,65535
#define SFTP_SERVER_DEFAULT_LISTEN_PORT        22222      //"sftpdĬ�ϼ����˿�"
//%$#@enum,true,false
#define BUILDING_SFTPC          false
#define BUILDING_SFTPD          false
//%$#@select
//%$#@string,1,32
#define serverKeyRsaPemFile "/efs/server-key-rsa.pem"   //"rsa.pem file"����ʹ���ļ�ϵͳ����Ҫ����
#define serverKeyEccDerFile "/efs/server-key-ecc.der"   //"ecc.der file"
#define serverKeyRsaDerFile "/efs/server-key-rsa.der"   //"rsa.der file"

#define SFTP_SERVER_USERNAME_PASSWORD_STRINGS  "jill:upthehill\n jack:fetchapail\n" //"�û������룬��ʽΪ: username:password\n"
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
//./examples;./src/keygen.c;./src/misc.c;./tests;

//%$#@endexclude
//%$#@self include path
//./;
//%$#@end self include path
//@#$%component end configure

