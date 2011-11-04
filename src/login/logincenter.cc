#include "logincenter.h"
#include "loginmsg.h"
#include "../base/log.h"
#include "../base/ini.h"
#include "../base/md5.h"
#include "../base/assert.h"
#include "dbquery/logindbquery.h"
#include "dbquery/cardgamedbquerytasktype.h"
#include "dbquery/createplayerdbquery.h"

using namespace base;

CLoginCenter::CLoginCenter() :
    CServerCirculator("LoginCenter")
{
}

CLoginCenter::~CLoginCenter()
{
}

bool CLoginCenter::Initialize()
{
    bool bRes = CServerCirculator::Initialize();

    if (bRes)
    {
        DBQueryFactory::Instance(bRes);
    }
    return bRes;
}

void CLoginCenter::Destory()
{
    DBQueryFactory::DestoryInstance();
}

void CLoginCenter::Pump()
{
    CServerCirculator::Pump();
    Task * pTask = NULL;
    while(true)
    {
        pTask = DBQueryFactory::Instance().GetCompletedTask();
        if (pTask == NULL)
        {
            break;
        }
        else
        {
            ProcessDBQueryResult(*((DBQueryTask *)pTask));
            delete pTask;
            pTask = NULL;
        }
    }
}

void CLoginCenter::ProcessMsg(MSG_BASE & rMsg, unsigned int nSlotIndex)
{
    WriteLog(LEVEL_DEBUG, "ProcessMsg. SlotIndex =%d, MSGID = %d.\n", nSlotIndex, rMsg.nMsg);

    switch (rMsg.nMsg)
    {
        case MSGID_SYSTEM_ConnectSuccess:
            {
                WriteLog(LEVEL_DEBUG, "One login agent connnect to the login center.(%u).\n", nSlotIndex);
            }
            break;
        case MSGID_LOGIN_LA2LC_LoginRequest:
            {
                MSG_LOGIN_LA2LC_LoginRequest & rLRMsg = (MSG_LOGIN_LA2LC_LoginRequest &)rMsg;

                LoginDBQuery * pDBQuery = new LoginDBQuery();
                pDBQuery->m_strUserName = rLRMsg.lrMsg.szUsername;
                string strPassword = rLRMsg.lrMsg.szPassword;
                pDBQuery->m_strPassword = MD5String(strPassword);
                long long nTaskID = DBQueryFactory::Instance().AddTask(CGDBQTT_LoginRequest, pDBQuery);
                RequestInfo ri(nSlotIndex, rLRMsg.nSlotIndex);
                m_TaskIDMapRequestInfo[nTaskID] = ri;
            }
            break;
        case MSGID_SYSTEM_Disconnect:
            {
                // clear the tasks of the login agent
                map < long long, RequestInfo >::iterator it = m_TaskIDMapRequestInfo.begin();
                for (; it!= m_TaskIDMapRequestInfo.end();)
                {
                    if (it->second.m_nLoginAgentSlotIndex == (int)nSlotIndex)
                    {
                        m_TaskIDMapRequestInfo.erase(it++);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            break;
        default:
            {
                WriteLog(LEVEL_WARNING, "CLoginCenter::ProcessMsg. Get an error msg(%d).\n", rMsg.nMsg);
            }
            break;
    }
}

void CLoginCenter::HeartBeat(unsigned int nIntervalMS)
{
    //WriteLog("HeartBeat.\n");
}

void CLoginCenter::ProcessDBQueryResult(DBQueryTask & rDBQueryTask)
{
    WriteLog("CGameServer::ProcessDBQueryResult.\n");
    switch (rDBQueryTask.m_nDBQueryTaskType)
    {
        case CGDBQTT_LoginRequest:
            {
                map< long long, RequestInfo >::iterator it = m_TaskIDMapRequestInfo.find(rDBQueryTask.GetTaskID());
                if (it != m_TaskIDMapRequestInfo.end())
                {
                    RequestInfo & rRI = it->second;
                    const list <DBQuery * > & rDBQueryList = rDBQueryTask.GetDBQueryList();
                    if (rDBQueryList.size() == 1)
                    {
                        LoginDBQuery * pLoginDBQuery = (LoginDBQuery *)(*(rDBQueryList.begin()));
                        if (pLoginDBQuery)
                        {
                            if (pLoginDBQuery->IsUserExist())
                            {
                                if (pLoginDBQuery->IsValidUser())
                                {
                                    map< int, RequestInfo >::iterator itUserID = m_UserIDMapRequestInfo.find(pLoginDBQuery->m_nDBUserID);
                                    if (itUserID == m_UserIDMapRequestInfo.end())
                                    {
                                        MSG_LOGIN_LC2LA_LoginResult lrMsg;
                                        lrMsg.nSlotIndex = rRI.m_nSlotIndex;
                                        SendMsg(lrMsg, rRI.m_nLoginAgentSlotIndex);
                                        m_UserIDMapRequestInfo[pLoginDBQuery->m_nDBUserID] = rRI;
                                    }
                                    else
                                    {
                                        // TODO: need drop the client

                                        // the user has been in game.
                                        MSG_LOGIN_LC2LA_LoginResult lrMsg;
                                        lrMsg.nSlotIndex = rRI.m_nSlotIndex;
                                        lrMsg.lrMsg.nResult = MSG_LOGIN_S2C_LoginResult::Result_Relogin;
                                        SendMsg(lrMsg, rRI.m_nLoginAgentSlotIndex);
                                        WriteLog(LEVEL_DEBUG, "The user(%d) relogin the game.\n", pLoginDBQuery->m_nDBUserID);
                                    }
                                }
                                else
                                {
                                    MSG_LOGIN_LC2LA_LoginResult lrMsg;
                                    lrMsg.nSlotIndex = rRI.m_nSlotIndex;
                                    lrMsg.lrMsg.nResult = MSG_LOGIN_S2C_LoginResult::Result_Failed;
                                    SendMsg(lrMsg, rRI.m_nLoginAgentSlotIndex);
                                }
                            }
                            else
                            {
                                // not valid, then create the user
                                CreatePlayerDBQuery * pDBQuery = new CreatePlayerDBQuery();
                                pDBQuery->m_strUserName = pLoginDBQuery->m_strUserName;
                                pDBQuery->m_strPassword = pLoginDBQuery->m_strPassword;
                                long long nTaskID = DBQueryFactory::Instance().AddTask(CGDBQTT_CreatePlayer, pDBQuery);
                                m_TaskIDMapRequestInfo[nTaskID] = rRI;
                            }
                        }
                        else
                        {
                            MSG_LOGIN_LC2LA_LoginResult lrMsg;
                            lrMsg.nSlotIndex = rRI.m_nSlotIndex;
                            lrMsg.lrMsg.nResult = MSG_LOGIN_S2C_LoginResult::Result_LoginRequestDBQueryIsNULL;
                            SendMsg(lrMsg, rRI.m_nLoginAgentSlotIndex);
                            WriteLog(LEVEL_ERROR, "Dispose CGDBQTT_LoginRequest. The login dbquery is null.\n");
                        }
                    }
                    else
                    {
                        MSG_LOGIN_LC2LA_LoginResult lrMsg;
                        lrMsg.nSlotIndex = rRI.m_nSlotIndex;
                        lrMsg.lrMsg.nResult = MSG_LOGIN_S2C_LoginResult::Result_TheLoginRequestDBQueryNumberError;
                        SendMsg(lrMsg, rRI.m_nLoginAgentSlotIndex);
                        WriteLog(LEVEL_ERROR, "The number of dbquery for loginrequest is not correct.\n");
                    }
                    m_TaskIDMapRequestInfo.erase(it);
                }
                else
                {
                    WriteLog(LEVEL_DEBUG, "The login agent has disconnected before get the user info.\n");
                }
            }
            break;
        case CGDBQTT_CreatePlayer:
            {
                map< long long, RequestInfo >::iterator it = m_TaskIDMapRequestInfo.find(rDBQueryTask.GetTaskID());
                if (it != m_TaskIDMapRequestInfo.end())
                {
                    RequestInfo & rRI = it->second;
                    const list <DBQuery * > & rDBQueryList = rDBQueryTask.GetDBQueryList();
                    if (rDBQueryList.size() == 1)
                    {
                        CreatePlayerDBQuery * pCreatePlayerDBQuery = (CreatePlayerDBQuery *)(*(rDBQueryList.begin()));
                        if (pCreatePlayerDBQuery)
                        {
                            // create user then login.
                            LoginDBQuery * pDBQuery = new LoginDBQuery();
                            pDBQuery->m_strUserName = pCreatePlayerDBQuery->m_strUserName;
                            pDBQuery->m_strPassword = pCreatePlayerDBQuery->m_strPassword;
                            long long nTaskID = DBQueryFactory::Instance().AddTask(CGDBQTT_LoginRequest, pDBQuery);
                            m_TaskIDMapRequestInfo[nTaskID] = rRI;
                        }
                        else
                        {
                            MSG_LOGIN_LC2LA_LoginResult lrMsg;
                            lrMsg.nSlotIndex = rRI.m_nSlotIndex;
                            lrMsg.lrMsg.nResult = MSG_LOGIN_S2C_LoginResult::Result_CreateUserDBQueryIsNULL;
                            SendMsg(lrMsg, rRI.m_nLoginAgentSlotIndex);
                            WriteLog(LEVEL_ERROR, "Dispose CGDBQTT_LoginRequest. The createplayer dbquery is null.\n");
                        }
                    }
                    else
                    {
                        MSG_LOGIN_LC2LA_LoginResult lrMsg;
                        lrMsg.nSlotIndex = rRI.m_nSlotIndex;
                        lrMsg.lrMsg.nResult = MSG_LOGIN_S2C_LoginResult::Result_TheCreateUserDBQueryNumberError;
                        SendMsg(lrMsg, rRI.m_nLoginAgentSlotIndex);
                        WriteLog(LEVEL_ERROR, "The number of dbquery for createplayer is not correct.\n");
                    }
                    m_TaskIDMapRequestInfo.erase(it);
                }
                else
                {
                    WriteLog(LEVEL_DEBUG, "The login agent has disconnected before create the user.\n");
                }
            }
            break;
        default :
            break;
    }
}
