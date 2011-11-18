#ifndef __login_logincenter_h_
#define __login_logincenter_h_

#include "../core/servercirculator.h"
#include "../db/dbqueryfactory.h"

using namespace core;
using namespace db;

struct RequestInfo
{
    RequestInfo()
    {
        m_nLoginAgentSlotIndex = -1;
        m_nSeqID = 0;
        m_nSlotIndex = -1;
    }
    RequestInfo(int nLoginAgentSlotIndex, long long nSeqID, int nSlotIndex)
    {
        m_nLoginAgentSlotIndex = nLoginAgentSlotIndex;
        m_nSeqID = nSeqID;
        m_nSlotIndex = nSlotIndex;
    }
    int m_nLoginAgentSlotIndex;
    long long m_nSeqID;
    int m_nSlotIndex;
};

class CLoginCenter : public CServerCirculator
{
public:
    CLoginCenter();
    virtual ~CLoginCenter();
protected:
    virtual bool Initialize();
    virtual void Pump();
    virtual void ProcessMsg(MSG_BASE & rMsg, unsigned int nSlotIndex);
    virtual void HeartBeat(unsigned int nIntervalMS);
    virtual void Destory();
    void ProcessDBQueryResult(DBQueryTask & rDBQueryTask);

private:

    map < int, RequestInfo > m_UserIDMapRequestInfo;
    map < long long, RequestInfo > m_TaskIDMapRequestInfo;
};

#endif // __login_logincenter_h_
