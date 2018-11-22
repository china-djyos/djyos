// =======================================================================

// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���  ��vnc_socket.h
// ������Ա: Administrator
// ����ʱ��: 2013-3-5
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef VNC_SOCKET_H_
#define VNC_SOCKET_H_

#include <rfb/rfb.h>

//��ʼ���������Ķ˿ڣ���������listen״̬
void rfbInitServerSocket(rfbScreenInfoPtr rfbScreen);

//�Է������Ķ˿ڽ��м�⣬���Ƿ����µ�����
extern int rfbCheckFds(rfbScreenInfoPtr rfbScreen,long usec);

//����len���ֳ����ͻ��ˣ�һ�η�����෢����
int WriteExact(rfbClientRec* cl, char *buf, int len);

//�ӿͻ��˶�ȡn���ֳ���һ�ζ�����������
int ReadExact(rfbClientRec* cl, char *buf, int len);

//�رտͻ��ˣ���ɹرտͻ��˺��һЩ�ƺ���
void rfbCloseClient(rfbClientRec* cl);

#endif /* SOCKET_H_ */
