#ifndef __ISBUS_PROTOCOL_h__
#define __ISBUS_PROTOCOL_h__

#ifdef __cplusplus
extern "C" {
#endif

//ͨ��Э�飨���ܺţ�������
struct ISBUS_FunctionSocket
{
    struct ISBUS_FunctionSocket *Next;       //���һ������ĩ��ָ��NULL
    u8 Protocol;                            //Э���
    u16 ProtocolRecvLen;                    //��Э�������հ�����
    u16 ProtocolSendLen;                    //��Э������Ͱ�����
    void *CommPort;                         //��Ӧ��ͨ�ſ�(������ӻ�)ָ��
    ISBUS_FntProtocolProcess MyProcess;      //Э�鴦����ָ��
};
//Э���ͷ��7�ֽ�
struct ISBUS_Protocol
{
    u8 start;           //0xeb
    u8 DstAddress;      //Ŀ���ַ
    u8 Protocol;        //������
    u8 SrcAddress;      //Դ��ַ
    u8 Len;             //���ݰ�����
    u8 ChkSum;          //У��ͣ���ǰ5����Ա�ĺ͡�
};

#ifdef __cplusplus
}
#endif

#endif //__ISBUS_PROTOCOL_h__



