//#include <errno.h>

#include <zlib.h>
#include "ClientSocketMgr.h"
#include "../net/systemmsg.h"
#include "../net/socketutil.h"
#include <time.h>

using namespace clinetnet;
void EncryptData(unsigned char * pData, unsigned int nLen)
{
    unsigned char szData[65535] = {0};
    memcpy(szData, nLen, pData);
    m_EncryptRC4(szData, nLen, pData);
}

void DecryptData(unsigned char * pData, unsigned int nLen)
{
    unsigned char szData[65535] = {0};
    memcpy(szData, nLen, pData);
    m_DecryptRC4(szData, nLen, pData);
}

CClientSocketMgr::CClientSocketMgr()
{
    m_hWndNotify = NULL;
    m_strIP = "";
    m_nPort = 0;
    m_hSocket = INVALID_SOCKET;

    m_eSocketState = SocketState_Free;

    m_hConnectEvent = WSA_INVALID_EVENT;
    m_hSocketEvent = WSA_INVALID_EVENT;

    memset(m_Buffer, 0, cMAX_RECV_BUFFER_SIZE);
    m_nBytesRemain = 0;

    m_hSendEvent = NULL;
    InitializeCriticalSection(&m_SendMsgListCS);
    InitializeCriticalSection(&m_RecvMsgListCS);

    m_hSendThread = NULL;
    m_nSendThreadID = 0;
    m_hRecvThread = NULL;
    m_nRecvThreadID = 0;
    m_bCompress = true;
    m_bEncrypt = true;
    memset(m_SendBuffer, 0, cMAX_SEND_BUFFER_SIZE);
    memset(m_UncompressBuffer, 0, cMAX_COMPRESSED_DATA_SIZE);
    m_ClientRSA.GenerateKey();
    m_ServerRSA.InitKey();
}

CClientSocketMgr::~CClientSocketMgr()
{
    Disconnect();
    WSACloseEvent(m_hSocketEvent);
    WSACloseEvent(m_hConnectEvent);
    DeleteCriticalSection(&m_RecvMsgListCS);
    DeleteCriticalSection(&m_SendMsgListCS);
    if (m_hSendEvent)
    {
        CloseHandle(m_hSendEvent);
        m_hSendEvent = NULL;
    }
    while (!m_SendMsgList.empty())
    {
        delete m_SendMsgList.front();
        m_SendMsgList.pop_front();
    }
    while (!m_RecvMsgList.empty())
    {
        delete m_RecvMsgList.front();
        m_RecvMsgList.pop_front();
    }
    m_hWndNotify = NULL;
}


void CClientSocketMgr::_AddSendMsg(MSG_BASE * pMsg)
{
    EnterCriticalSection(&m_SendMsgListCS);
    m_SendMsgList.push_back(pMsg);
    LeaveCriticalSection(&m_SendMsgListCS);
    SetEvent(m_hSendEvent);
}

bool CClientSocketMgr::_AddRecvData(const unsigned char * pData, int nLen)
{
    const unsigned char * pHeader = pData;
    while (pHeader < pData + nLen)
    {
        MSG_BASE * pHeaderMsg = (MSG_BASE *)pHeader;
        _AddRecvMsg(*pHeaderMsg);
        pHeader += pHeaderMsg->nSize;
    }
    if (pHeader - pData != nLen)
    {
        //WriteLog(LEVEL_WARNING, "CClientSocketMgr(%d)::_AddRecvData. pHeader(%p) - pData(%p) != nNewLen(%lu).\n", m_nSlotIndex, pHeader, pData, nNewLen);
        return false;
    }
    else
    {
        return true;
    }
}
bool CClientSocketMgr::_DisposeRecvMsg(const MSG_BASE & rMsg)
{
    switch (rMsg.nMsg)
    {
        case MSGID_SYSTEM_S2C_SecretKey:
            {
                MSG_SYSTEM_S2C_SecretKey rSKMsg = (MSG_SYSTEM_S2C_SecretKey &)rMsg;

                unsigned char szPublicKey1[64]
                unsigned char szPublicKey2[64]
                int nKeySize1 = m_ServerRSA.DecrypPublic(rSKMsg.key, 64, szPublicKey1);
                if (nKeySize1 != 32)
                {
                    return false;
                }
                int nKeySize2 = m_ServerRSA.DecrypPublic(rSKMsg.key + 64, 64, szPublicKey2);
                if (nKeySize2 != 32)
                {
                    return false;
                }
                unsigned char key[64];
                memcpy(key, szPublicKey1, 32);
                memcpy(key, szPublicKey2, 32);
                int nKeySize = m_ClientRSA.DecryptPrivate(szPublicKey, 64, key);
                if (nKeySize != cSECRET_KEY_LEN)
                {
                    return false;
                }

                m_RC4.RC4_Init(nSrcKeySize, key);


                m_eSocketState = SocketState_Normal;
                MSG_SYSTEM_ConnectSuccess msg;
                _AddRecvMsg(msg);
            }
            break;
        case MSGID_SYSTEM_CompressedAndEncrypted:
            {
                if (!m_bCompress || !m_bEncrypt)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Received CompressedAndEncrypted msg. but m_bCompress= %d and m_bEncrypt = %d.\n", m_nSlotIndex, m_bCompress, m_bEncrypt);
                    return false;
                }
                //net::Decrypt((unsigned char *)(&rMsg) + sizeof(MSG_BASE), rMsg.nSize - sizeof(MSG_BASE));
                DecryptData((unsigned char *)(&rMsg) + sizeof(MSG_BASE), rMsg.nSize - sizeof(MSG_BASE));

                MSG_SYSTEM_CompressedAndEncrypted & rCEMsg = (MSG_SYSTEM_CompressedAndEncrypted &)rMsg;
                if (rCEMsg.nSize > cMAX_MSG_SIZE)
                {
                    return false;
                }

                if (rCEMsg.nSrcDataSize > cMAX_C_AND_E_DATA_SIZE)
                {
                    return false;
                }
                unsigned long nNewLen = rCEMsg.nSrcDataSize;
                int nRet = uncompress(m_UncompressBuffer, &nNewLen, rCEMsg.data, rCEMsg.nSize - sizeof(MSG_SYSTEM_CompressedAndEncrypted) + 1);
                if (nRet != Z_OK)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. uncompress failed. nRet=%d, nNewLen=%lu, nLen=%lu.\n", m_nSlotIndex, nRet, nNewLen, rCEMsg.nSize - sizeof(MSG_SYSTEM_CompressedAndEncrypted) + 1);
                    return false;
                }

                if (nNewLen != rCEMsg.nSrcDataSize)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. nNewLen(%lu) != rCEMsg.nSrcDataSize(%d).\n", m_nSlotIndex, nNewLen, rCEMsg.nSrcDataSize);
                    return false;
                }
                if (net::CheckSum(m_UncompressBuffer, nNewLen) != rCEMsg.nCheckSum)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. net::CheckSum(m_UncompressBuffer, nNewLen(%lu)) !=rCEMsg.nCheckSum.\n", m_nSlotIndex, nNewLen);
                    return false;
                }
                if (!_AddRecvData(m_UncompressBuffer, nNewLen))
                {
                    return false;
                }
                //           unsigned char * pUBHeader = m_UncompressBuffer;
                //           while (pUBHeader < m_UncompressBuffer + nNewLen)
                //           {
                //               MSG_BASE * pHeaderMsg = (MSG_BASE *)pUBHeader;
                //_AddRecvMsg(*pHeaderMsg);
                //               pUBHeader += pHeaderMsg->nSize;
                //           }
                //           if (pUBHeader - m_UncompressBuffer != nNewLen)
                //           {
                //               //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. pUBHeader(%p) - m_UncompressBuffer(%p) != nNewLen(%lu).\n", m_nSlotIndex, pUBHeader, m_UncompressBuffer, nNewLen);
                //               return false;
                //           }
            }
            break;
        case MSGID_SYSTEM_Compressed:
            {
                if (!m_bCompress)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Received Compressed msg. but m_bCompress is false.\n", m_nSlotIndex);
                    return false;
                }
                MSG_SYSTEM_Compressed & rCompressedMsg = (MSG_SYSTEM_Compressed &)rMsg;
                if (rCompressedMsg.nSrcDataSize > cMAX_COMPRESSED_DATA_SIZE)
                {
                    return false;
                }
                unsigned long nNewLen = rCompressedMsg.nSrcDataSize;
                int nRet = uncompress(m_UncompressBuffer, &nNewLen, rCompressedMsg.data, rCompressedMsg.nSize - sizeof(MSG_SYSTEM_Compressed) + 1);
                if (nRet != Z_OK)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. uncompress failed. nRet=%d, nNewLen=%lu, nLen=%lu.\n", m_nSlotIndex, nRet, nNewLen, rCompressedMsg.nSize - sizeof(MSG_SYSTEM_Compressed) + 1);
                    return false;
                }
                if (nNewLen != rCompressedMsg.nSrcDataSize)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. nNewLen(%lu) != rCompressedMsg.nSrcDataSize(%d).\n", m_nSlotIndex, nNewLen, rCompressedMsg.nSrcDataSize);
                    return false;
                }
                if (net::CheckSum(m_UncompressBuffer, nNewLen) != rCompressedMsg.nCheckSum)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. net::CheckSum(m_UncompressBuffer, nNewLen(%lu)) !=rCompressedMsg.nCheckSum.\n", m_nSlotIndex, nNewLen);
                    return false;
                }
                if (!_AddRecvData(m_UncompressBuffer, nNewLen))
                {
                    return false;
                }
                //           unsigned char * pUBHeader = m_UncompressBuffer;
                //           while (pUBHeader < m_UncompressBuffer + nNewLen)
                //           {
                //               MSG_BASE * pHeaderMsg = (MSG_BASE *)pUBHeader;
                //_AddRecvMsg(*pHeaderMsg);
                //               pUBHeader += pHeaderMsg->nSize;
                //           }
                //           if (pUBHeader - m_UncompressBuffer != nNewLen)
                //           {
                //               //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. pUBHeader(%p) - m_UncompressBuffer(%p) != nNewLen(%lu).\n", m_nSlotIndex, pUBHeader, m_UncompressBuffer, nNewLen);
                //               return false;
                //           }
            }
            break;
        case MSGID_SYSTEM_Encrypted:
            {
                if (!m_bEncrypt)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. Received Encrypted msg. but m_bEncrypt is false.\n", m_nSlotIndex);
                    return false;
                }
                //net::Decrypt((unsigned char *)(&rMsg) + sizeof(MSG_BASE), rMsg.nSize - sizeof(MSG_BASE));
                DecryptData((unsigned char *)(&rMsg) + sizeof(MSG_BASE), rMsg.nSize - sizeof(MSG_BASE));
                MSG_SYSTEM_Encrypted & rEncryptedMsg = (MSG_SYSTEM_Encrypted &)rMsg;

                int nDataSize = rEncryptedMsg.nSize - sizeof(MSG_SYSTEM_Encrypted) + 1;
                if (net::CheckSum(rEncryptedMsg.data, nDataSize) != rEncryptedMsg.nCheckSum)
                {
                    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. net::CheckSum(rEncryptedMsg.data, nDataSize(%d)) != rEncryptedMsg.nCheckSum.\n", m_nSlotIndex, nDataSize);
                    return false;
                }

                if (!_AddRecvData(rEncryptedMsg.data, nDataSize))
                {
                    return false;
                }

                //           unsigned char * pHeader = rEncryptedMsg.data;
                //           MSG_BASE * pHeaderMsg = NULL;
                //           while (pHeader - rEncryptedMsg.data < nDataSize)
                //           {
                //               pHeaderMsg = (MSG_BASE *)pHeader;
                //_AddRecvMsg(*pHeaderMsg);
                //pHeader += pHeaderMsg->nSize;
                //           }
                //           if (pHeader - rEncryptedMsg.data != nDataSize)
                //           {
                //               //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeRecvMsg. pHeader(%p) - rEncryptedMsg.data(%p) != nDataSize(%d).\n", m_nSlotIndex, pHeader, rEncryptedMsg.data, nDataSize);
                //               return false;
                //           }
            }
            break;
        default:
            _AddRecvMsg(rMsg);
            break;
    }
    return true;
}

void CClientSocketMgr::_AddRecvMsg(const MSG_BASE & rMsg)
{
    MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
    memcpy(pMsg, &rMsg, rMsg.nSize);
    EnterCriticalSection(&m_RecvMsgListCS);
    m_RecvMsgList.push_back(pMsg);
    LeaveCriticalSection(&m_RecvMsgListCS);
    if (m_hWndNotify)
    {
        PostMessage(m_hWndNotify, UM_MSG, pMsg->nMsg, pMsg->nSize);
    }
}

bool CClientSocketMgr::Init(bool bCompress/* = true*/, bool bEncrypt/* = true*/, HWND hWndNotify/* = NULL*/)
{
    m_bCompress = bCompress;
    m_bEncrypt = bEncrypt;
    m_hWndNotify = hWndNotify;
    WSADATA WSAData;
    if (WSAStartup(WINSOCK_VERSION, &WSAData))
    {
        return false;
    }
    //ASSERT(m_hEventConnect == WSA_INVALID_EVENT);
    m_hSocketEvent = WSACreateEvent();
    TCHAR szName[MAX_PATH];
    sprintf_s(szName, "SendEvent%d%d", this, (int)time(NULL));
    m_hSendEvent = CreateEvent(NULL, FALSE, FALSE, szName);
    return true;
}

bool CClientSocketMgr::Connect(string strIP, unsigned short nPort)
{
    if (m_eSocketState != SocketState_Free)
    {
        return false;
    }
    if (strIP == "")
    {
        return false;
    }
    if (nPort == 0)
    {
        return false;
    }
    m_strIP = strIP;
    m_nPort = nPort;
    return Reconnect();
}

bool CClientSocketMgr::Reconnect()
{
    if (m_nPort == 0 || m_strIP == "")
    {
        //WriteLog(LEVEL_ERROR,"CClientSocketMgr::Connect. You may have not initialized the clientsocketmgr.\n");
        return false;
    }
    if (m_eSocketState != SocketState_Free)
    {
        return false;
    }
    m_eSocketState = SocketState_Connecting;
    if (m_hSocket != INVALID_SOCKET)
    {
        closesocket(m_hSocket);
        m_hSocket = INVALID_SOCKET;
    }

    // reset recv buffer
    memset(m_Buffer, 0, cMAX_RECV_BUFFER_SIZE);
    m_nBytesRemain = 0;

    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (m_hSocket == INVALID_SOCKET)
    {
        //WriteLog(LEVEL_ERROR,"CClientSocketMgr::Connect. socket() error.\n");
        m_eSocketState = SocketState_Free;
        return false;
    }
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
    sockAddr.sin_port = htons(m_nPort);

    if (m_hConnectEvent != WSA_INVALID_EVENT)
    {
        WSACloseEvent(m_hConnectEvent);
    }
    m_hConnectEvent = WSACreateEvent();
    int nErrorCode = WSAEventSelect(m_hSocket, m_hConnectEvent, FD_CONNECT);
    if (nErrorCode == SOCKET_ERROR)
    {
        //TRACE(_T("__ClientSocketMgr WSAEventSelect connect failed.\n"));
        closesocket(m_hSocket);
        m_hSocket = INVALID_SOCKET;
        m_eSocketState = SocketState_Free;
        return false;
    }

    int nConnect = connect(m_hSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
    if (nConnect == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
    {
        //TRACE(_T("__ClientSocketMgr Socket connect error.\n"));
        closesocket(m_hSocket);
        m_hSocket = INVALID_SOCKET;
        m_eSocketState = SocketState_Free;
        return false;
    }
    else
    {
        const DWORD cCONNECT_TIMEOUT = 5000;
        DWORD dwWaitRes = WaitForSingleObject(m_hConnectEvent, cCONNECT_TIMEOUT);
        if (dwWaitRes != WAIT_OBJECT_0)
        {
            // log connect failed
            closesocket(m_hSocket);
            m_hSocket = INVALID_SOCKET;
            m_eSocketState = SocketState_Free;
            return false;
        }

        WSANETWORKEVENTS events;
        if (0 == WSAEnumNetworkEvents(m_hSocket, m_hConnectEvent, &events))
        {
            if (events.lNetworkEvents & FD_CONNECT)
            {
                if ( 0 != events.iErrorCode[FD_CONNECT_BIT])
                {
                    closesocket(m_hSocket);
                    m_hSocket = INVALID_SOCKET;
                    m_eSocketState = SocketState_Free;
                    return false;
                }
            }
        }

        nErrorCode = WSAEventSelect(m_hSocket, m_hSocketEvent, FD_READ | FD_CLOSE);
        if (nErrorCode == SOCKET_ERROR)
        {
            // log
            closesocket(m_hSocket);
            m_hSocket = INVALID_SOCKET;
            m_eSocketState = SocketState_Free;
            return false;
        }

        m_hSendThread = CreateThread(NULL, 0, SendThreadProc, this, 0, &m_nSendThreadID);
        if (m_hSendThread == NULL)
        {
            //TRACE(_T("__ClientSocketMgr Create Send thread failed.\n"));
            Disconnect();
            return false;
        }
        m_hRecvThread = CreateThread(NULL, 0, RecvThreadProc, this, 0, &m_nRecvThreadID);
        if (m_hRecvThread == NULL)
        {
            //TRACE(_T("__ClientSocketMgr Create Recv thread failed.\n"));
            Disconnect();
            return false;
        }

        m_eSocketState = SocketState_Connecting;


        ////MSG_SYSTEM_ConnectSuccess msg;
        //MSG_SYSTEM_SocketConnectSuccess msg;
        //_AddRecvMsg(msg);

        // TODO : Generate public key
        unsigned char * pPublicKey = NULL;
        int nKeySize = m_ClientRSA.GetPublicKey(&pPublicKey);
        // ASSERT(nKeySize > cMAX_HALF_OF_PUBLIC_KEY_LEN);
        MSG_SYSTEM_ClientPublicKey * pCPKMsg = new MSG_SYSTEM_ClientPublicKey();
        pCPKMsg.nSrcKeySize = nKeySize;
        pCPKMsg.nSrcKeySize1 = cMAX_HALF_OF_PUBLIC_KEY_LEN;
        m_ServerRSA.PublicEncrypt(pPublicKey, cMAX_HALF_OF_PUBLIC_KEY_LEN, pCPKMsg.szPublicKey1);
        pCPKMsg.nSrcKeySize2 = nKeySize - cMAX_HALF_OF_PUBLIC_KEY_LEN;
        m_ServerRSA.PublicEncrypt(pPublicKey + cMAX_HALF_OF_PUBLIC_KEY_LEN, nKeySize - cMAX_HALF_OF_PUBLIC_KEY_LEN, pCPKMsg.szPublicKey2);
        _AddSendMsg(pCPKMsg);
        OPENSSL_free(pPublicKey);

        int nOptVal = 1;
        setsockopt(m_hSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&nOptVal, sizeof(int));

        return true;
    }
}

bool CClientSocketMgr::IsConnected()
{
    return m_eSocketState == SocketState_Normal;
}

MSG_BASE * CClientSocketMgr::GetMsg()
{
    MSG_BASE * pMsg = NULL;
    while (true)
    {
        EnterCriticalSection(&m_RecvMsgListCS);
        if (!m_RecvMsgList.empty())
        {
            pMsg = m_RecvMsgList.front();
            m_RecvMsgList.pop_front();
        }
        LeaveCriticalSection(&m_RecvMsgListCS);

        if (pMsg != NULL)
        {
            _Pretreat(pMsg);
            if (pMsg != NULL)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if (pMsg == NULL)
    {
        // Check Ping ?
    }
    return pMsg;
}

void CClientSocketMgr::_Pretreat(MSG_BASE * &pMsg)
{
    switch (pMsg->nMsg)
    {
        //case MSGID_SYSTEM_SocketConnectSuccess:
        //    {
        //        //MSG_SYSTEM_ClientPublicKey cpkMsg;
        //        //SendMsg(cpkMsg);
        //        //MSG_SYSTEM_C2S_SecretKey skMsg;
        //        //SendMsg(skMsg);
        //        delete pMsg;
        //        pMsg = NULL;
        //    }
        //    break;
        case MSGID_SYSTEM_ConnectSuccess:
            {
                //m_eSocketState = SocketState_Normal;
            }
            break;
        case MSGID_SYSTEM_Disconnect:
            {
                Disconnect();
            }
            break;
        case  MSGID_SYSTEM_CheckAlive:
            {
                MSG_SYSTEM_CheckAliveReply msg;
                SendMsg(msg);
                delete pMsg;
                pMsg = NULL;
            }
            break;
        default:
            break;
    }
}

bool CClientSocketMgr::SendMsg(const MSG_BASE & rMsg, bool bToSelf/* = false */)
{
    while (m_eSocketState == SocketState_Connecting)
    {
        Sleep(5);
    }
    if (m_eSocketState != SocketState_Normal)
    {
        // log the state is not normal
        return false;
    }
    if (rMsg.nMsg < sizeof(MSG_BASE) && rMsg.nSize > cMAX_MSG_SIZE)
    {
        // log msg's size is error.
        return false;
    }

    if (bToSelf)
    {
        _AddRecvMsg(rMsg);
    }
    else
    {
        // if the msg is system msg, need dispose with it?
        // need compress and encrypt ?
        MSG_BASE * pMsg = CreateDynamicLengthMsg(rMsg.nSize, (MSG_BASE *)0);
        memcpy(pMsg, &rMsg, rMsg.nSize);
        _AddSendMsg(pMsg);
    }
    return true;
}

MSG_BASE * CClientSocketMgr::_GetSendMsg()
{
    MSG_BASE * pMsg = NULL;
    EnterCriticalSection(&m_SendMsgListCS);
    if (!m_SendMsgList.empty())
    {
        pMsg = m_SendMsgList.front();
        m_SendMsgList.pop_front();
    }
    LeaveCriticalSection(&m_SendMsgListCS);
    return pMsg;
}

void CClientSocketMgr::Disconnect()
{
    m_eSocketState = SocketState_Closed;

    if (m_hSocket != INVALID_SOCKET)
    {
        closesocket(m_hSocket);
        m_hSocket = INVALID_SOCKET;
    }
    if (m_hSendThread)
    {
        WaitForSingleObject(m_hSendThread, INFINITE);
        CloseHandle(m_hSendThread);
        m_hSendThread = NULL;
        m_nSendThreadID = 0;
    }
    if (m_hRecvThread)
    {
        WaitForSingleObject(m_hRecvThread, INFINITE);
        CloseHandle(m_hRecvThread);
        m_hRecvThread = NULL;
        m_nRecvThreadID = 0;
    }
    list< MSG_BASE * > tmpRecvMsgList;
    EnterCriticalSection(&m_RecvMsgListCS);
    if (!m_RecvMsgList.empty())
    {
        tmpRecvMsgList = m_RecvMsgList;
        m_RecvMsgList.clear();
    }
    LeaveCriticalSection(&m_RecvMsgListCS);

    while (!tmpRecvMsgList.empty())
    {
        delete tmpRecvMsgList.front();
        tmpRecvMsgList.pop_front();
    }

    EnterCriticalSection(&m_SendMsgListCS);
    while (!m_SendMsgList.empty())
    {
        delete m_SendMsgList.front();
        m_SendMsgList.pop_front();
    }
    LeaveCriticalSection(&m_SendMsgListCS);
    m_eSocketState = SocketState_Free;
}

DWORD CClientSocketMgr::SendThreadProc(LPVOID pParam)
{
    CClientSocketMgr * pCSM = (CClientSocketMgr *)pParam;
    pCSM->ProcessSend();
    return 0;
}
DWORD CClientSocketMgr::RecvThreadProc(LPVOID pParam)
{
    CClientSocketMgr * pCSM = (CClientSocketMgr *)pParam;
    pCSM->ProcessRecv();
    return 0;
}

void CClientSocketMgr::ProcessSend()
{
    while (m_eSocketState != SocketState_Closed)
    {
        DWORD dwWaitRes = WaitForSingleObject(m_hSendEvent, 20);
        if (dwWaitRes == WAIT_OBJECT_0)
        {
            if (!_Send())
            {
                m_eSocketState = SocketState_Closed;
            }
        }
    }
    MSG_SYSTEM_Disconnect msg;
    _AddRecvMsg(msg);
}

void CClientSocketMgr::ProcessRecv()
{
    WSANETWORKEVENTS events;
    while (m_eSocketState != SocketState_Closed)
    {
        DWORD dwWaitRes = WaitForSingleObject(m_hSocketEvent, 20);
        if (dwWaitRes == WAIT_OBJECT_0)
        {
            if (0 == WSAEnumNetworkEvents(m_hSocket, m_hSocketEvent, &events))
            {
                if (events.lNetworkEvents & FD_READ)
                {
                    if (!_Recv())
                    {
                        m_eSocketState = SocketState_Closed;
                    }
                }
                if (events.lNetworkEvents & FD_CLOSE)
                {
                    int nErrorCode = events.iErrorCode[FD_CLOSE_BIT];
                    switch (nErrorCode)
                    {
                        case WSAENETDOWN:
                            // log
                            break;
                        case WSAECONNRESET:
                            // log
                            break;
                        case WSAECONNABORTED:
                            // log
                            break;
                    }
                    // log socket closed[%d] nErrorCode
                    m_eSocketState = SocketState_Closed;
                }
            }
        }
    }
}


bool CClientSocketMgr::_SendData(unsigned char * pData, int nLen)
{
    char * p = (char *)pData;
    int nBytes = 0;
    while (nBytes < nLen)
    {
        int nL = send(m_hSocket, p + nBytes, nLen - nBytes, 0);
        if (nL != SOCKET_ERROR)
        {
            nBytes += nL;
        }
        else if (WSAGetLastError() == WSAEWOULDBLOCK)
        {
            Sleep(10);
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool CClientSocketMgr::_EncryptDataToSend(const unsigned char * pData, int nLen)
{
    MSG_SYSTEM_Encrypted * pEncryptedMsg = CreateDynamicLengthMsg(nLen + sizeof(MSG_SYSTEM_Encrypted) - 1, (MSG_SYSTEM_Encrypted *)0);
    memcpy(pEncryptedMsg->data, pData, nLen);
    pEncryptedMsg->nCheckSum = net::CheckSum(pData, nLen);
    //net::Encrypt((unsigned char *)pEncryptedMsg + sizeof(MSG_BASE), pEncryptedMsg->nSize - sizeof(MSG_BASE));
    EncryptData((unsigned char *)pEncryptedMsg + sizeof(MSG_BASE), pEncryptedMsg->nSize - sizeof(MSG_BASE));

    return _SendData((unsigned char *)pEncryptedMsg,pEncryptedMsg->nSize);
}

bool CClientSocketMgr::_CompressDataToSend(const unsigned char * pData, int nLen)
{
    unsigned long nNewLen = compressBound(nLen);
    //ASSERT(nNewLen <= cMAX_COMPRESSED_DATA_SIZE_BOUND);

    MSG_SYSTEM_Compressed * pCompressedMsg = CreateDynamicLengthMsg(nNewLen + sizeof(MSG_SYSTEM_Compressed) - 1, (MSG_SYSTEM_Compressed *)0);
    pCompressedMsg->nCheckSum = net::CheckSum(pData, nLen);
    int nRet = compress(pCompressedMsg->data, &nNewLen, pData, nLen);
    if (nRet == Z_OK)
    {
        pCompressedMsg->nSize = nNewLen + sizeof(MSG_SYSTEM_Compressed) - 1;
        pCompressedMsg->nSrcDataSize = nLen;
        return _SendData((unsigned char *)pCompressedMsg,pCompressedMsg->nSize);
    }
    else
    {
        //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_CompressDataToWSQ. Compress error. nRet=%d, nLen=%lu, nNewLen=%lu.\n", m_nSlotIndex, nRet, nLen, nNewLen);
        delete pCompressedMsg;
        pCompressedMsg = NULL;

        const unsigned char * pHeader = pData;
        MSG_BASE * pHeaderMsg = (MSG_BASE *)pHeader;
        while (pHeader - pData < nLen)
        {
            pHeaderMsg = (MSG_BASE *)pHeader;

            MSG_BASE * pMsg = CreateDynamicLengthMsg(pHeaderMsg->nSize, (MSG_BASE *)0);

            if (!_SendData((unsigned char *)pMsg,pMsg->nSize))
            {
                delete pMsg;
                pMsg = NULL;
                return false;
            }
            pHeader += pHeaderMsg->nSize;
        }
        return (pHeader - pData) == nLen;
        //if (pHeader - pData != nLen)
        //{
        //    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_CompressDataToWSQ. pData is error. pHeader=%p, pData=%p, nLen=%lu.\n", m_nSlotIndex, pHeader, pData, nLen);
        //    return false;
        //}
    }
    //return true;
}
bool CClientSocketMgr::_CompressAndEncryptDataToSend(const unsigned char * pData, int nLen)
{
    unsigned long nNewLen = compressBound(nLen);
    //ASSERT(nNewLen <= cMAX_COMPRESSED_DATA_SIZE_BOUND);

    MSG_SYSTEM_CompressedAndEncrypted * pCEMsg = CreateDynamicLengthMsg(nNewLen + sizeof(MSG_SYSTEM_CompressedAndEncrypted) - 1, (MSG_SYSTEM_CompressedAndEncrypted *)0);
    pCEMsg->nCheckSum = net::CheckSum(pData, nLen);
    int nRet = compress(pCEMsg->data, &nNewLen, pData, nLen);
    if (nRet == Z_OK)
    {
        pCEMsg->nSize = nNewLen + sizeof(MSG_SYSTEM_CompressedAndEncrypted) - 1;
        pCEMsg->nSrcDataSize = nLen;
        //net::Encrypt((unsigned char *)pCEMsg + sizeof(MSG_BASE), pCEMsg->nSize - sizeof(MSG_BASE));
        EncryptData((unsigned char *)pCEMsg + sizeof(MSG_BASE), pCEMsg->nSize - sizeof(MSG_BASE));
        return _SendData((unsigned char *)pCEMsg,pCEMsg->nSize);
    }
    else
    {
        //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_CompressAndEncryptDataToWSQ. Compress error. nRet=%d, nLen=%lu, nNewLen=%lu.\n", m_nSlotIndex, nRet, nLen, nNewLen);
        delete pCEMsg;
        pCEMsg = NULL;

        return _EncryptDataToSend(pData, nLen);
    }
}

bool CClientSocketMgr::_DisposeSendBuffer(unsigned char * pData, int nLen)
{
    if (m_bCompress)
    {
        if (m_bEncrypt)
        {
            return _CompressAndEncryptDataToSend(pData, nLen);
        }
        else
        {
            return _CompressDataToSend(pData, nLen);
        }
    }
    else
    {
        if (m_bEncrypt)
        {
            return _EncryptDataToSend(pData, nLen);
        }
        else
        {
            return _SendData(pData,nLen);
        }
    }
}

bool CClientSocketMgr::_Send()
{
    MSG_BASE * pMsg = NULL;
    int nTotalSize = 0;
    unsigned char * pCBHeader = m_SendBuffer;

    while ((pMsg = _GetSendMsg()) != NULL)
    {
        switch (pMsg->nMsg)
        {
            case MSGID_SYSTEM_ClientPublicKey:
            //case MSGID_SYSTEM_C2S_SecretKey:
                {
                    if (!_SendData((unsigned char *)pMsg,pMsg->nSize))
                    {
                        delete pMsg;
                        pMsg = NULL;
                        return false;
                    }
                }
                break;
            default:
                {
                    if (nTotalSize + pMsg->nSize <= cMAX_SEND_BUFFER_SIZE)
                    {
                        nTotalSize += pMsg->nSize;
                        memcpy(pCBHeader, pMsg, pMsg->nSize);
                        pCBHeader += pMsg->nSize;
                    }
                    else
                    {
                        if (nTotalSize <= 0)
                        {
                            delete pMsg;
                            pMsg = NULL;
                            return false;
                        }
                        if (!_DisposeSendBuffer(m_SendBuffer, nTotalSize))
                        {
                            delete pMsg;
                            pMsg = NULL;
                            return false;
                        }

                        nTotalSize = pMsg->nSize;
                        pCBHeader = m_SendBuffer + pMsg->nSize;
                        memcpy(m_SendBuffer, pMsg, pMsg->nSize);
                    }
                }
                break;
        }
        delete pMsg;
        pMsg = NULL;
    }

    if (nTotalSize > 0)
    {
        return _DisposeSendBuffer(m_SendBuffer, nTotalSize);
    }
    else
    {
        return true;
    }
}

bool CClientSocketMgr::_Recv()
{
    //if (m_eSocketState != SocketState_Normal)
    //{
    //    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::RevData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
    //    return false;
    //}

    bool bRes = true;

    int nLen = 0;
    while (true)
    {
        nLen = recv(m_hSocket, m_Buffer + m_nBytesRemain, cMAX_RECV_BUFFER_SIZE - m_nBytesRemain, 0);
        if (nLen > 0)
        {
            m_nBytesRemain += nLen;
            if (m_nBytesRemain >= sizeof(MSG_BASE))
            {
                if (!_DisposeMsgData())
                {
                    bRes = false;
                    break;
                }
            }
        }
        else if (nLen == 0)
        {
            //_SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_Shutdown);
            bRes = false;
            break;
        }
        else
        {
            int nErr = errno;
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                //_SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ReadError);
                //WriteLog(LEVEL_DEBUG, "CSocketSlot(%d)::RecvData. Read error. ErrInfo=(%s); m_nBytesRemain = %d.\n", m_nSlotIndex, strerror(nErr), m_nBytesRemain);
                bRes = false;
            }
            break;
        }

    }
    return bRes;
}

bool CClientSocketMgr::_DisposeMsgData()
{
    //if (m_eSocketState != SocketState_Normal)
    //{
    //    //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData. The socket state is not normal. State = %d.\n", m_nSlotIndex, m_State);
    //    return false;
    //}

    bool bRes = true;
    char * pPos = m_Buffer;

    while (m_nBytesRemain >= sizeof(MSG_BASE))
    {
        MSG_BASE *pMsg = (MSG_BASE *)pPos;

        // if Size is Larger than Max Size, Error
        if (pMsg->nSize > cMAX_MSG_SIZE)
        {
            //_SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData ErrMsgSize: too large. Size(%d), MaxSize(%d).\n", m_nSlotIndex, pMsg->nSize, cMAX_MSG_SIZE);
            bRes = false;
            break;

        }

        if (m_nBytesRemain < pMsg->nSize)
        {
            break;
        }

        if (pMsg->nSize < sizeof(MSG_BASE))
        {
            //_SetState(m_State == SocketState_Accepting ? SocketState_Accepting : SocketState_Normal, SocketState_Broken, DisconnectReason_ErrMsgSize);
            //WriteLog(LEVEL_WARNING, "CSocketSlot(%d)::_DisposeMsgData ErrMsgSize: too small. Size(%d), MinSize(%d).\n", m_nSlotIndex, pMsg->nSize, sizeof(MSG_BASE));
            bRes = false;
            break;
        }
        if (!_DisposeRecvMsg(*pMsg))
        {
            return false;
        }
        m_nBytesRemain -= pMsg->nSize;
        pPos += pMsg->nSize;
    }
    if (m_nBytesRemain > 0 && pPos != m_Buffer)
    {
        memmove(m_Buffer, pPos, m_nBytesRemain);
    }
    return bRes;
}
