#ifndef __login_loginagent_h_
#define __login_loginagent_h_

#include "../core/servercirculator.h"
#include "../net/clientsocketmgr.h"

using namespace core;
using namespace net;

const int cFRAMES_FOR_CHECK_LOGIN_CENTER = 200;

class UserSlot
{
public:
    enum State
    {
        State_Free,
        State_Logining,
        State_LoginSuccess,
        State_Disconnected,
    };
public:
    UserSlot()
    {
        m_nSlotIndex = -1;
        m_nSeqID = 0;
        m_State = State_Free;
    };
    ~UserSlot(){};
    int m_nSlotIndex;
    long long m_nSeqID;
    State m_State;
};

class CLoginAgent : public CServerCirculator
{
public:
    CLoginAgent();
    virtual ~CLoginAgent();
protected:
    virtual bool Initialize();
    virtual void Pump();
    virtual void ProcessMsg(MSG_BASE & rMsg, unsigned int nSlotIndex);
    void ProcessLoginCenterMsg(MSG_BASE & rMsg);
    virtual void HeartBeat(unsigned int nIntervalMS);
    virtual void Destory();

private:
    unsigned int m_nMaxClient;
    UserSlot * m_aUserSlot;
    CClientSocketMgr m_LoginCenterCSM;
    int m_nCheckLoginCenterPeriod;
    long long m_nSeqID;
};

#endif // __login_loginagent_h_
