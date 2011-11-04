#include "loginagent.h"
#include "loginmsg.h"
#include "../base/log.h"
#include "../base/ini.h"
#include "../base/md5.h"
#include "../base/assert.h"

using namespace base;
using namespace core;

CLoginAgent::CLoginAgent() :
    CServerCirculator("LoginAgent")
{
    m_nMaxClient = 0;
    m_aUserSlot = NULL;
    m_nCheckLoginCenterPeriod = 0;
}

CLoginAgent::~CLoginAgent()
{
    if (m_aUserSlot)
    {
        delete [] m_aUserSlot;
        m_aUserSlot = NULL;
    }
}

bool CLoginAgent::Initialize()
{
    bool bRes = CServerCirculator::Initialize();
    if (bRes)
    {
        m_nMaxClient = GIni.GetInt(m_strServerName.c_str(), "MaxClient", 32);
        WriteLog("The max client of server(%s) is %u.\n", m_strServerName.c_str(), m_nMaxClient);

        m_aUserSlot = new UserSlot[m_nMaxClient];
        for (unsigned int i=0; i<m_nMaxClient; i++)
        {
            m_aUserSlot[i].m_nSlotIndex = i;
        }
    }

    bool bEncryptToLoginCenter = GIni.GetInt(m_strServerName.c_str(), "EncryptToLoginCenter", 0) == 0 ? false : true;
    bool bCompressToLoginCenter = GIni.GetInt(m_strServerName.c_str(), "CompressToLoginCenter", 0) == 0 ? false : true;
    m_LoginCenterCSM.Init(bEncryptToLoginCenter, bCompressToLoginCenter);

    string strLoginCenterIP = "";
    GIni.GetString(m_strServerName.c_str(), "LoginCenterIP", strLoginCenterIP, "127.0.0.1");
    unsigned short nLoginCenterPort = GIni.GetInt(m_strServerName.c_str(), "LoginCenterPort", 8888);
    m_LoginCenterCSM.Connect(strLoginCenterIP, nLoginCenterPort);
    m_LoginCenterCSM.Run();

    return bRes;
}

void CLoginAgent::Destory()
{
    if (m_aUserSlot)
    {
        delete [] m_aUserSlot;
        m_aUserSlot = NULL;
    }
    m_LoginCenterCSM.Disconnect();
}

void CLoginAgent::Pump()
{
    CServerCirculator::Pump();
    while (true)
    {
        MSG_BASE * pMsg = m_LoginCenterCSM.GetMsg();
        if (pMsg == NULL)
        {
            break;
        }
        ProcessLoginCenterMsg(*pMsg);
        delete pMsg;
        pMsg = NULL;
    }
}

void CLoginAgent::ProcessLoginCenterMsg(MSG_BASE & rMsg)
{
    WriteLog(LEVEL_DEBUG, "CLoginAgent::ProcessLoginCenterMsg. MsgID=%d.\n", rMsg.nMsg);
    switch (rMsg.nMsg)
    {
        case MSGID_SYSTEM_ConnectSuccess:
            {
                WriteLog(LEVEL_DEBUG, "Connect to Login Center Success.\n");
            }
            break;
        case MSGID_LOGIN_LC2LA_LoginResult:
            {
                MSG_LOGIN_LC2LA_LoginResult & rLRMsg = (MSG_LOGIN_LC2LA_LoginResult &)rMsg;
                if (m_aUserSlot[rLRMsg.nSlotIndex].m_State == UserSlot::State_Logining)
                {
                    SendMsg(rLRMsg.lrMsg, rLRMsg.nSlotIndex);
                    if (rLRMsg.lrMsg.nResult == MSG_LOGIN_S2C_LoginResult::Result_Success)
                    {
                        m_aUserSlot[rLRMsg.nSlotIndex].m_State = UserSlot::State_LoginSuccess;
                    }
                    else
                    {
                        m_aUserSlot[rLRMsg.nSlotIndex].m_State = UserSlot::State_Free;
                    }
                }
                else if (m_aUserSlot[rLRMsg.nSlotIndex].m_State == UserSlot::State_Disconnected)
                {
                    m_aUserSlot[rLRMsg.nSlotIndex].m_State = UserSlot::State_Free;
                }
                else
                {
                    WriteLog(LEVEL_WARNING, "CLoginAgent::ProcessLoginCenterMsg. Error Slot State(%d).\n", m_aUserSlot[rLRMsg.nSlotIndex].m_State);
                }
            }
            break;
        case MSGID_SYSTEM_Disconnect:
            {
                for (unsigned int i=0; i<m_nMaxClient; i++)
                {
                    if (m_aUserSlot[i].m_State == UserSlot::State_Logining)
                    {
                        MSG_LOGIN_S2C_LoginResult loginResultMsg;
                        loginResultMsg.nResult = MSG_LOGIN_S2C_LoginResult::Result_DisconnectWithLoginCenter;
                        SendMsg(loginResultMsg, i);
                        m_aUserSlot[i].m_State = UserSlot::State_Free;
                    }
                    else if (m_aUserSlot[i].m_State == UserSlot::State_Disconnected)
                    {
                        m_aUserSlot[i].m_State = UserSlot::State_Free;
                    }
                }
            }
            break;
        default:
            {
            }
            break;
    }
}

void CLoginAgent::ProcessMsg(MSG_BASE & rMsg, unsigned int nSlotIndex)
{
    WriteLog(LEVEL_DEBUG, "ProcessMsg. SlotIndex =%d, MSGID = %d.\n", nSlotIndex, rMsg.nMsg);

    switch (rMsg.nMsg)
    {
        case MSGID_SYSTEM_ConnectSuccess:
            {
                WriteLog(LEVEL_DEBUG, "One client connnect to the server.(%u).\n", nSlotIndex);
            }
            break;
        case MSGID_LOGIN_C2S_LoginRequest:
            {

                MSG_LOGIN_C2S_LoginRequest & rLoginRequestMsg = (MSG_LOGIN_C2S_LoginRequest &)rMsg;
                MSG_LOGIN_LA2LC_LoginRequest lrMsg;
                lrMsg.nSlotIndex = nSlotIndex;
                memcpy(&(lrMsg.lrMsg), &rLoginRequestMsg, rLoginRequestMsg.nSize);
                if (!m_LoginCenterCSM.SendMsg(lrMsg))
                {
                    MSG_LOGIN_S2C_LoginResult loginResultMsg;
                    loginResultMsg.nResult = MSG_LOGIN_S2C_LoginResult::Result_SendRequest2LoginCenterFailed;
                    SendMsg(loginResultMsg, nSlotIndex);
                }
                else
                {
                    m_aUserSlot[nSlotIndex].m_State = UserSlot::State_Logining;
                }
            }
            break;
        case MSGID_SYSTEM_Disconnect:
            {
                if (m_aUserSlot[nSlotIndex].m_State == UserSlot::State_Logining)
                {
                    m_aUserSlot[nSlotIndex].m_State = UserSlot::State_Disconnected;
                }
                else if (m_aUserSlot[nSlotIndex].m_State == UserSlot::State_LoginSuccess)
                {
                    m_aUserSlot[nSlotIndex].m_State = UserSlot::State_Free;
                }
                else
                {
                    WriteLog(LEVEL_WARNING, "CLoginAgent::ProcessMsg. Error Slot State(%d).\n", m_aUserSlot[nSlotIndex].m_State);
                }
            }
            break;
        default:
            {
                WriteLog(LEVEL_WARNING, "CLoginAgent::ProcessMsg. Get an error msg(%d).\n", rMsg.nMsg);
            }
            break;
    }
}

void CLoginAgent::HeartBeat(unsigned int nIntervalMS)
{
    //WriteLog("HeartBeat.\n");
    m_nCheckLoginCenterPeriod++;
    if (m_nCheckLoginCenterPeriod > cFRAMES_FOR_CHECK_LOGIN_CENTER)
    {
        if (m_LoginCenterCSM.IsDisconnected())
        {
            m_LoginCenterCSM.Reconnect();
        }
        m_nCheckLoginCenterPeriod = 0;
    }
}
