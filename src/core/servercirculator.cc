#include "servercirculator.h"
#include "../base/log.h"

using namespace core;

CServerCirculator::CServerCirculator(string strServerName)
{
    m_strServerName = strServerName;
}

CServerCirculator::~CServerCirculator()
{
}

bool CServerCirculator::_InitSocketMgr()
{
    bool bRes = true;
    unsigned short nPort = GIni.GetInt(m_strServerName.c_str(), "Port", 8888);
    unsigned int nMaxClient = GIni.GetInt(m_strServerName.c_str(), "MaxClient", 32);
    bool bEncrypt = GIni.GetInt(m_strServerName.c_str(), "Encrypt", 0) == 0 ? false : true;
    bool bCompress = GIni.GetInt(m_strServerName.c_str(), "Compress", 0) == 0 ? false : true;
    unsigned int nAliveCheckInterval = GIni.GetInt(m_strServerName.c_str(), "AliveCheckInterval", 5);
    unsigned int nAliveTimeOut = GIni.GetInt(m_strServerName.c_str(), "AliveTimeOut", 30);
    bRes = m_SocketMgr.Init(nPort, nMaxClient, bEncrypt, bCompress, nAliveCheckInterval, nAliveTimeOut);
    if (bRes)
    {
        WriteLog("Initialize the socket mgr success. Port = %d, MaxClient = %d.\n", nPort, nMaxClient);
    }
    else
    {
        WriteLog(LEVEL_ERROR, "Initialize the socket mgr failed. Port = %d, MaxClient = %d.\n", nPort, nMaxClient);
    }
    return bRes;
}

bool CServerCirculator::Initialize()
{
    bool bRes = Circulator::Initialize();
    unsigned int nPeriod = GIni.GetInt(m_strServerName.c_str(), "Period", 25);
    SetPeriod(nPeriod);
    if (bRes)
    {
        bRes = _InitSocketMgr();
    }
    return bRes;
}

bool CServerCirculator::PreProcess()
{
    bool bRes = true;
    
    m_SocketMgr.Run();
    return bRes;
}

void CServerCirculator::EndProcess()
{
    m_SocketMgr.Stop();
}

void CServerCirculator::Pump()
{
    Circulator::Pump();
    MSG_BASE * pMsg = NULL;
    unsigned int nSlotIndex = 0;

    while (m_SocketMgr.GetMsg(pMsg, nSlotIndex))
    {
        if (pMsg != NULL)
        {
            ProcessMsg(*pMsg, nSlotIndex);
            delete pMsg;
            pMsg = NULL;
        }
    }
}

void CServerCirculator::ProcessMsg(MSG_BASE & rMsg, unsigned int nSlotIndex)
{
}

void CServerCirculator::HeartBeat(unsigned int nIntervalMS)
{
}

bool CServerCirculator::SendMsg(MSG_BASE & rMsg, unsigned int nSlotIndex)
{
    return m_SocketMgr.SendMsg(rMsg, nSlotIndex);
}

void CServerCirculator::Destory()
{
}

