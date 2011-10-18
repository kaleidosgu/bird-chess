#ifndef __net_socketslot_h_
#define __net_socketslot_h_

#include "msgbasic.h"
#include "../base/mutex.h"
#include "../base/loopqueue.h"
#include "senddataelement.h"
//#include "recvdataelementqueue.h"
#include "recvdataelement.h"
#include <netinet/in.h>
#include "systemmsg.h"

const unsigned int cSEND_QUEUE_SIZE = 4096;
const unsigned int cMAX_RECV_BUFFER_SIZE = cMAX_MSG_SIZE * 2;

#ifndef EPOLLRDHUP
#define EPOLLRDHUP              0x2000
#endif //EPOLLRDHUP


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
        DisconnectReason_RecvBuffOverflow_OR_StateNotNormal,
        DisconnectReason_Shutdown,
        DisconnectReason_ReadError,
        DisconnectReason_SendError,
        DisconnectReason_RemainDataError,
        DisconnectReason_CreateMSG_BASE,
        DisconnectReason_AliveTimeOut,
        DisconnectReason_DisposeSendQueueFailed,
    };

    class CSocketSlotMgr;
    class CSocketMgr;
    class CSocketSlot
    {
        friend class CSocketSlotMgr;
    public:
        void OnAccept(int nFd, const sockaddr_in &rSaClient);
        bool SendMsg(MSG_BASE & rMsg);
        //bool SendMsg(MSG_BASE & rMsg, bool & rbRemainData);
        bool SendData();
        bool RecvData();
        unsigned int GetSlotIndex() const
        {
            return m_nSlotIndex;
        }
        int GetFd() const
        {
            return m_nFd;
        }
        SocketState GetState() const
        {
            return m_State;
        }
        bool Disconnect(int nDisconnectReason);
        void SetStateConnected();
        bool Close();

        bool IsInSendQueue() const
        {
            return m_bIsInSendQueue;
        }
        void SetInSendQueue();
        void SetNotInSendQueue();

        void SendAliveMsg();
        //void SetStateNotAlive();
        time_t GetLatestAliveTime()
        {
            return m_tLatestAliveTime;
        }

    protected:
        CSocketSlot();
        virtual ~CSocketSlot();
        void _Reset();
        void _SetEncrypt(bool bEncrypt);
        void _SetCompress(bool bCompress);
        void _SetSendDataBuffer(unsigned char * pSendDataBuffer);
        void _SetUncompressBuffer(unsigned char * pUncompressBuffer);
        void _SetSlotIndex(unsigned int nSlotIndex);
        void _SetRecvQueue(LoopQueue< CRecvDataElement * > * pRecvQueue);
        void _SetSocketMgr(CSocketMgr * pSocketMgr)
        {
            m_pSocketMgr = pSocketMgr;
        }
        bool _SendMsgDirectly(MSG_BASE * pMsg);
        bool _AddMsgToWSQ(MSG_BASE * pMsg);
        bool _AddDataToWSQ(const unsigned char * pData, int nLen);
        bool _CompressDataToWSQ(const unsigned char * pData, int nLen);
        bool _CompressAndEncryptDataToWSQ(const unsigned char * pData, int nLen);
        bool _EncryptDataToWSQ(const unsigned char * pData, int nLen);
        //void _Compress(unsigned char * pDes, unsigned int & rnDesLen, const unsigned char * pSrc, unsigned int nSrcLen);
        bool _DisposeSendQueue();
        bool _SendBufferData();
        bool _SendWSQ();
        bool _SendElement(CSendDataElement *pOut, int nCount);
        void _ClearSendMsgQueue();
        void _Pretreat(MSG_BASE * &pMsg);
        bool _AddRecvMsg(MSG_BASE * pMsg);
        //void _Uncompress(unsigned char * pDes, unsigned int nNewLen, const unsigned char * pSrc, unsigned int nLen);
        void _GenerateSecretKey();
        bool _AddRecvData(const unsigned char * pData, int nLen);
        virtual bool _DisposeRecvMsg(MSG_BASE & rMsg);
        bool _DisposeRecvBuffer();
        bool _SetState(SocketState eSrcState, SocketState eDesState);
        bool _SetState(SocketState eSrcState, SocketState eDesState, int nReason);

        char m_ClientPublicKey[cMAX_CLIENT_PUBLIC_KEY_LEN];
        char m_EncryptKey[cMAX_SECRETKEY_LEN];
        char m_DecryptKey[cMAX_SECRETKEY_LEN];

        int m_nFd;
        unsigned short m_nPort;
        in_addr m_addr;
        unsigned int m_nSlotIndex;
        SocketState m_State;
        Mutex m_MutexForState;
        int m_nDisconnectReason;

        CSocketSlot * m_next;
        Mutex m_MutexForSend;

        bool m_bConnectSuccess;

        bool m_bEncrypt;
        bool m_bCompress;
        unsigned char * m_pSendDataBuffer;
        unsigned char * m_pUncompressBuffer;

        // for recv
        char m_RecvBuffer[cMAX_RECV_BUFFER_SIZE];
        char * m_pLastBuffer;
        char * m_pRecvHead;
        int m_nBytesRemain;

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

        // check alive
        time_t m_tLatestAliveTime;
        CSocketMgr * m_pSocketMgr;
    };

    class CSocketSlotMgr
    {
    public:
        CSocketSlotMgr();
        ~CSocketSlotMgr();

        bool Init(unsigned int nMaxSlotNum, LoopQueue< CRecvDataElement * > * pRecvQueue, CSocketMgr * pSocketMgr, bool bEncrypt, bool bCompress, unsigned char * pSendDataBuffer, unsigned char * pUncompressBuffer);
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

