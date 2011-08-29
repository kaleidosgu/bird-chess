#ifndef __net_socketslot_h_
#define __net_socketslot_h_

#include "msgbasic.h"
#include "../base/mutex.h"
#include "senddataelement.h"
#include "recvdataelementqueue.h"
#include <netinet/in.h>

const unsigned int cMAX_SEND_BUFF_PACKET_COUNT = 4096;

using namespace base;
namespace net
{
    enum SocketState
    {
        SocketState_Free,
        SocketState_Accepting,
        SocketState_Normal,
        SocketState_Disconnecting,
        SocketState_Broken,
        SocketState_Closed
    };
    enum DisconnectReason
    {
        DisconnectReason_Unkown,
        DisconnectReason_ErrMsgSize,
        DisconnectReason_RecvBuffOverflow,
        DisconnectReason_Shutdown,
        DisconnectReason_ReadError,
        DisconnectReason_SendError,
        DisconnectReason_RemainDataError,
        DisconnectReason_CreateMSG_BASE,
    };


    class CSocketSlotMgr;
    class CSocketSlot
    {
        friend class CSocketSlotMgr;
    public:
        void OnAccept(int nFd, const sockaddr_in &rSaClient);
        bool SendMsg(MSG_BASE & rMsg, bool & rbRemainData);
        bool SendData();
        bool RecvData();
        unsigned int GetSlotIndex() const;
        int GetFd() const;
        SocketState GetState() const;
        bool Disconnect(int nDisconnectReason);
        void SetStateConnected();
        bool Close();

        //For ClientSocketMgr

    protected:
        CSocketSlot();
        ~CSocketSlot();
        void _Reset();
        void _SetSlotIndex(unsigned int nSlotIndex);
        void _SetRecvQueue(CRecvDataElementQueue * pRecvQueue);

        bool _SendMsg(MSG_BASE & rMsg, bool & rbRemainData);
        bool _SendMsgCache();
        bool _SendMsgQueue();
        bool _SendElement(CSendDataElement *pOut, int nCount);
        bool _AddSendMsg(MSG_BASE & rMsg);
        void _ClearSendMsgQueue();
        bool _CopyMsgToRecvBuff(MSG_BASE & rMsg);
        bool _AddRecvMsg(MSG_BASE & rMsg);
        bool _DisposeMsgData();
        bool _SetState(SocketState eSrcState, SocketState eDesState);
        bool _SetState(SocketState eSrcState, SocketState eDesState, int nReason);

        int m_nFd;
        unsigned short m_nPort;
        in_addr m_addr;
        unsigned int m_nSlotIndex;
        SocketState m_State;
        Mutex m_MutexForState;
        int m_nDisconnectReason;

        CSocketSlot * m_next;
        Mutex m_MutexForSend;

        // for recv
        char m_Buffer[cMAX_PACKET_SIZE];
        unsigned short m_nBytesRemain;
        CRecvDataElementQueue * m_pRecvQueue;

        // for send
        bool m_bRemainData;
        MSG_BASE * m_pMsgCache;
        int m_nSeek;
        CSendDataElement m_aSendData[cMAX_SEND_BUFF_PACKET_COUNT];
        CSendDataElement *m_pSendDataHead;
        CSendDataElement *m_pSendDataTail;
    };

    class CSocketSlotMgr
    {
    public:
        CSocketSlotMgr();
        ~CSocketSlotMgr();

        bool Init(unsigned int nMaxSlotNum, CRecvDataElementQueue *pRecvQueue);
        CSocketSlot * GetFreeSlot();
        bool ReleaseSlot(unsigned int nSlotIndex);
        bool ReleaseSlot(CSocketSlot & rSocketSlot);
        CSocketSlot * GetSocketSlot(unsigned int nSlotIndex) const;
        //            unsigned int GetCurrentSlotNum() const;

    private:
        bool _FreeSlot(CSocketSlot & rSocketSlot);

        unsigned int m_nMaxSlotNum;
        unsigned int m_nFreeSlotNum;
        CSocketSlot * m_aSlot;
        CSocketSlot * m_pFreeSlot;
        Mutex m_MutexForFreeSlotQueue;
    };

    bool SetNonBlocking(int nFd);

}

#endif // __net_socketslot_h_

