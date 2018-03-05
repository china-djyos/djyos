
#include <stddef.h>

typedef enum _RecordState
{
    OFF,
    ON,
}tagRecordState;

typedef struct _IntIn{
    u32 dwTotal;
    u32 dwAHBERR; // （高速模式下）
    u32 dwACK; // 收到/发出ACK响应中断
    u32 dwSTALL; // 收到STALL响应中断
    u32 dwDTERR; // 数据同步错误
    u32 dwFRMOR;  // 帧溢出
    u32 dwXFRC;  // 传输完成
    u32 dwCHH;  // 禁止通道
    u32 dwTXERR; // 通信事务错误
    u32 dwNAK; // 收到NAK响应
}tagIntIn;

typedef struct _IntOut{
    u32 dwTotal;
    u32 dwAHBERR; // （高速模式下）
    u32 dwACK; // 收到/发出ACK响应中断
    u32 dwNYET; // （HS模式下）收到尚未就绪
    u32 dwFRMOR;  // 帧溢出
    u32 dwXFRC;  // 传输完成
    u32 dwSTALL; // 收到STALL响应中断
    u32 dwNAK; // 收到NAK响应
    u32 dwTXERR; // 通信事务错误
    u32 dwDTERR; // 数据同步错误
    u32 dwCHH;  // 禁止通道
}tagIntOut;

typedef struct _Int{
    tagRecordState state;
    u32 dwTotal;
    u32 dwINCOMPISOOUT; // 未完成周期传输
    u32 dwIISOIXFR; // 未完成IN同步传输
    u32 dwPTXFE; // 周期性TX FIFO为空
    u32 dwMMIS; // 模式不匹配
    u32 dwDISCINT;  // 检测到断开链接
    u32 dwHPRTINT;  // 主机端口终端
    u32 dwSOF;  // 发送了SOF帧
    u32 dwHCINT; // 主机通道中断
    u32 dwRXFLVL; // RX FIFO非空，有数据可读
    tagIntIn in;
    tagIntOut out;
}tagInt;
