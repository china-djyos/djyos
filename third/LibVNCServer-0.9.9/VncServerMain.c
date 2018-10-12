// =======================================================================

// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��VNCSERVER.c
// ������Ա: Administrator
// ����ʱ��: 2013-3-8
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-8>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include <rfb/rfb.h>
#include "VncOsPort.h"
#include "VncNetPort.h"

rfbScreenInfo* pgRfbServerScreen = NULL;
void rfbDefaultScreenInit(rfbScreenInfo* ptscreen)
{
   //Ŀǰֻ����TCP���������⣬����Ĳ�����ĵģ�UDP,HTTP�ȣ�
   ptscreen->listenport=5900;
   ptscreen->listenSock=-1;
   ptscreen->desktopName = "LibVNCServer";
   //���ͻ��˶��г�ʼ��Ϊ�գ�
   ptscreen->clientHead=NULL;
   INIT_MUTEX(ptscreen->clientsync);
   ptscreen->protocolMajorVersion = rfbProtocolMajorVersion;
   ptscreen->protocolMinorVersion = rfbProtocolMinorVersion;

   /* cursor */

   ptscreen->cursorX=ptscreen->cursorY=ptscreen->underCursorBufferLen=0;
   ptscreen->underCursorBuffer=NULL;
   ptscreen->dontConvertRichCursorToXCursor = FALSE;
   ptscreen->cursor = NULL;
   INIT_MUTEX(ptscreen->cursorMutex);

   IF_PTHREADS(ptscreen->backgroundLoop = FALSE);

   /* proc's and hook's */

   ptscreen->setTranslateFunction = rfbSetTranslateFunction;
   ptscreen->kbdAddEvent = NULL;
   ptscreen->kbdReleaseAllKeys = NULL;
   ptscreen->ptrAddEvent = NULL;
   ptscreen->setXCutText = NULL;
   ptscreen->getCursorPtr = NULL;
   ptscreen->newClientHook = NULL;
   ptscreen->displayHook = NULL;
   ptscreen->displayFinishedHook = NULL;
   ptscreen->getKeyboardLedStateHook = NULL;
   ptscreen->xvpHook = NULL;

}
// =========================================================================
// �������ܣ������¿ͻ��˵����Ӻ�����ͨ��
// �������������������pgRfbServerScreen
// ���������
// ����ֵ  ��
// ˵��    ��
// =========================================================================
ptu32_t ClientListenTask()
{
    rfbInitServerSocket(pgRfbServerScreen);
    while(1)//ѭ�����
    {
        rfbCheckFds(pgRfbServerScreen,4000);
    }
    return 0;
}
// =============================================================================
// �������ܣ�VNC������
// ���������
// ���������
// ����ֵ  ��
// ˵��    �����ö��̵߳���ʽʵ�ַ������Ĺ��ܣ�һ���������߳�
//          1,accept�߳�����ר�Ŵ����¿ͻ��˵�����
//          2,messsadge �߳�����ר�Ŵ���ͻ��˺ͷ���������Ϣ����
//          3,update�߳����ڴ���ͼ����¸��ͻ���
// =============================================================================
int VncServer_Main(const char *lcdname)
{
    bool_t  ret;
    pgRfbServerScreen=M_Malloc(sizeof(rfbScreenInfo),0);
    if(NULL==pgRfbServerScreen)
    {
        printk("%s :No Mem For pgRfbServerScreen\n\r",__FUNCTION__);
        goto EXIT_NOMEMFORSCREEN;
    }
    else
    {
        memset((void *)pgRfbServerScreen, 0, sizeof(rfbScreenInfo));
    }
    rfbDefaultScreenInit(pgRfbServerScreen);

    //��װVNC��ʾ��
    extern bool_t VncDispInstall(rfbScreenInfo* rfbserver,const char *lcdname);
    if(false == VncDispInstall(pgRfbServerScreen,lcdname))
    {
        printk("%s :VncDisplay��Init Failed\n\r",__FUNCTION__);
        goto EXIT_DISPLAYFAILED;
    }
    //�����������˿���?
    ret=TaskCreate(ClientListenTask,pgRfbServerScreen,"VncServerListen");
    if(false == ret)
    {
        printk("%s :CreateListenTask Failed\n\r",__FUNCTION__);
        goto CLIENTLISTENTASK_FAILED;
    }
    printk("%s :INIT SUCCESS\n\r",__FUNCTION__);
    return  0;
//ʧ���Ժ�Ӧ�õȴ����߳̽���
CLIENTLISTENTASK_FAILED:
EXIT_DISPLAYFAILED:
    free(pgRfbServerScreen);
    pgRfbServerScreen = NULL;
EXIT_NOMEMFORSCREEN:
    printk("%s:INIT FAILED!\n\r",__FUNCTION__);
    return -1;
}
