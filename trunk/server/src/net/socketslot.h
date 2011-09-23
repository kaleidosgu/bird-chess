#ifndef __net_socketslot_h_
#define __net_socketslot_h_

#include "msgbasic.h"
#include "../base/mutex.h"
#include "../base/loopqueue.h"
#include "senddataelement.h"
//#include "recvdataelementqueue.h"
#include "recvdataelement.h"
#include <netinet/in.h>

const unsigned int cSEND_QUEUE_SIZE = 4096;

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
        bool SendMsg(MSG_BASE & rMsg);
        //bool SendMsg(MSG_BASE & rMsg, bool & rbRemainData);
        bool SendData();
        bool RecvData();
        unsigned int GetSlotIndex() const;
        int GetFd() const;
        SocketState GetState() const;
        bool Disconnect(int nDisconnectReason);
        void SetStateConnected();
        bool Close();

        bool IsInSendQueue() const
        {
            return m_bIsInSendQueue;
        }
        void SetInSendQueue();
        void SetNotInSendQueue();

        //For ClientSocketMgr

    protected:
        CSocketSlot();
        ~CSocketSlot();
        void _Reset();
        void _SetSlotIndex(unsigned int nSlotIndex);
        void _SetRecvQueue(LoopQueue< CRecvDataElement * > * pRecvQueue);

        //bool _SendMsg(MSG_BASE & rMsg, bool & rbRemainData);
        bool _AddMsgToWSQ(MSG_BASE * pMsg);
        void _DisposeSendQueue();
        bool _SendBufferData();
        bool _SendWSQ();
        //bool _SendMsgCache();
        //bool _SendMsgQueue();
        bool _SendElement(CSendDataElement *pOut, int nCount);
        bool _AddSendMsg(MSG_BASE & rMsg);
        void _ClearSendMsgQueue();
        bool _CopyMsgToRecvBuff(MSG_BASE & rMsg);
        bool _AddRecvMsg(MSG_BASE & rMsg);
        bool _DisposeMsgData();
        bool _SetState(SocketState eSrcState, SocketState eDesState);
        bool _SetState(SocketState eSrcState, SocketState eDesState, int nReason);

        void * m_pEncryptFunc;
        void * m_pDecryptFunc;
        void * m_pCompressFunc;
        void * m_pUncompressFunc;
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
        char m_Buffer[cMAX_PACKET_SIZE * 2];
        char * m_pMidBuffer;
        char * m_pBufferHead;
        unsigned short m_nBytesRemain;
        MSG_BASE * m_pWaitingMsg;
        unsigned short m_nMsgDataSize;

        LoopQueue< CRecvDataElement * > * m_pRecvQueue;

        // for send
        bool m_bIsInSendQueue;
        Mutex m_MutexForIsInSendQueue;

        // for application to send msg
        LoopQueue< MSG_BASE * > m_SendQueue;

        /*
        // for network engine send msg
        iovec m_aWaintingSendQueue[cSEND_QUEUE_SIZE];
        iovec * m_pBeginWSQ;
        iovec * m_pEndWSQ;
        iovec * m_pHeadWSQ;
        iovec * m_pTailWSQ;
        */

        // remain msg
        //bool m_bRemainData;
        MSG_BASE * m_pMsgCache;
        int m_nSeek;

        CSendDataElement m_aSendData[cSEND_QUEUE_SIZE];
        CSendDataElement *m_pSendDataHead;
        CSendDataElement *m_pSendDataTail;
    };

    class CSocketSlotMgr
    {
    public:
        CSocketSlotMgr();
        ~CSocketSlotMgr();

        bool Init(unsigned int nMaxSlotNum, LoopQueue< CRecvDataElement * > * pRecvQueue);
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

