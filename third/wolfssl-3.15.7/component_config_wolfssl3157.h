
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"wolfssl3157"//�������
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
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_WOLFSSL3157 == false )
//#warning  " wolfssl3157  �������δ���ã�ʹ��Ĭ������"
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_WOLFSSL3157    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string,1,16
//%$#@SYMBOL
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
//./wolfcrypt/benchmark;./wolfcrypt/test;./wolfcrypt/user-crypto;./tirtos;./testsuite;./tests;
//./swig;./sslSniffer;./mqx;./mplabx;./mcapi;./IDE;./examples;./ctaocrypt;
//./src/bio.c;./src/crl.c;./src/ocsp.c;./src/sniffer.c;./src/tls13.c;
//./wolfcrypt/src/port;./wolfcrypt/src/aes_asm.S;./wolfcrypt/src/arc4.c;./wolfcrypt/src/asm.c;
//./wolfcrypt/src/async.c;./wolfcrypt/src/blake2b.c;./wolfcrypt/src/camellia.c;./wolfcrypt/src/cmac.c;
//./wolfcrypt/src/compress.c;./wolfcrypt/src/cpuid.c;./wolfcrypt/src/cryptodev.c;
//./wolfcrypt/src/curve25519.c;./wolfcrypt/src/des3.c;./wolfcrypt/src/dsa.c;
//./wolfcrypt/src/ecc_fp.c;./wolfcrypt/src/ed25519.c;./wolfcrypt/src/evp.c;
//./wolfcrypt/src/fe_low_mem.c;./wolfcrypt/src/fe_operations.c;./wolfcrypt/src/fips.c;
//./wolfcrypt/src/fips_test.c;./wolfcrypt/src/ge_low_mem.c;./wolfcrypt/src/ge_operations.c;
//./wolfcrypt/src/hc128.c;./wolfcrypt/src/idea.c;./wolfcrypt/src/integer.c;./wolfcrypt/src/md2.c;
//./wolfcrypt/src/md4.c;./wolfcrypt/src/misc.c;./wolfcrypt/src/pkcs7.c;./wolfcrypt/src/pkcs12.c;
//./wolfcrypt/src/pwdbased.c;./wolfcrypt/src/rabbit.c;./wolfcrypt/src/ripemd.c;./wolfcrypt/src/selftest.c;
//./wolfcrypt/src/sp_arm32.c;./wolfcrypt/src/sp_arm64.c;./wolfcrypt/src/sp_armthumb.c;
//./wolfcrypt/src/sp_c32.c;./wolfcrypt/src/sp_c64.c;./wolfcrypt/src/sp_int.c;./wolfcrypt/src/sp_x86_64.c;
//./wolfcrypt/src/srp.c;./wolfcrypt/src/wc_pkcs11.c;./wolfcrypt/src/wolfcrypt_first.c;
//./wolfcrypt/src/wolfcrypt_last.c;./wolfcrypt/src/wolfevent.c

//%$#@endexclude
//%$#@self include path
//./;
//%$#@end self include path
//@#$%component end configure

