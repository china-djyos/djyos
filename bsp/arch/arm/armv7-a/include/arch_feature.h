#ifndef __ARCH_FEATURE_H__
#define __ARCH_FEATURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm32_feature.h"
#include "board-config.h"   //��startupĿ¼�¶�Ӧ���Ŀ¼��,���ڹ��������м��� -I
#include "arch_stdint.h"

//�жϽ�������£�ִ��CPU����ָ�wfi��ʱ������ж��Ѿ�����ģ������ֿ��������
//һ�ǲ���������ߣ����ж�ʹ�ܺ�������Ӧ�жϣ����ǽ������ߴ���һ���жϲŻ��ѡ�
//��һ������Ƚϳ�������֪����û�еڶ�������ģ��ڶ�������͹�����ƻ�ǳ��鷳��
#define CN_SLEEP_ON_INT     true        //true��ʾ���ж�����ʱ�ܽ�������

//���������������ջ�ռ����ֵ
//16=ͨ�üĴ�����+cpsr-SP
#define CN_CONTEXT_LIMIT        (16)*4

#ifdef __cplusplus
}
#endif
#endif // __ARCH_FEATURE_H__

