
#include <stddef.h>

typedef enum _RecordState
{
    OFF,
    ON,
}tagRecordState;

typedef struct _IntIn{
    u32 dwTotal;
    u32 dwAHBERR; // ������ģʽ�£�
    u32 dwACK; // �յ�/����ACK��Ӧ�ж�
    u32 dwSTALL; // �յ�STALL��Ӧ�ж�
    u32 dwDTERR; // ����ͬ������
    u32 dwFRMOR;  // ֡���
    u32 dwXFRC;  // �������
    u32 dwCHH;  // ��ֹͨ��
    u32 dwTXERR; // ͨ���������
    u32 dwNAK; // �յ�NAK��Ӧ
}tagIntIn;

typedef struct _IntOut{
    u32 dwTotal;
    u32 dwAHBERR; // ������ģʽ�£�
    u32 dwACK; // �յ�/����ACK��Ӧ�ж�
    u32 dwNYET; // ��HSģʽ�£��յ���δ����
    u32 dwFRMOR;  // ֡���
    u32 dwXFRC;  // �������
    u32 dwSTALL; // �յ�STALL��Ӧ�ж�
    u32 dwNAK; // �յ�NAK��Ӧ
    u32 dwTXERR; // ͨ���������
    u32 dwDTERR; // ����ͬ������
    u32 dwCHH;  // ��ֹͨ��
}tagIntOut;

typedef struct _Int{
    tagRecordState state;
    u32 dwTotal;
    u32 dwINCOMPISOOUT; // δ������ڴ���
    u32 dwIISOIXFR; // δ���INͬ������
    u32 dwPTXFE; // ������TX FIFOΪ��
    u32 dwMMIS; // ģʽ��ƥ��
    u32 dwDISCINT;  // ��⵽�Ͽ�����
    u32 dwHPRTINT;  // �����˿��ն�
    u32 dwSOF;  // ������SOF֡
    u32 dwHCINT; // ����ͨ���ж�
    u32 dwRXFLVL; // RX FIFO�ǿգ������ݿɶ�
    tagIntIn in;
    tagIntOut out;
}tagInt;
