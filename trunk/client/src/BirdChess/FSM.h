//////////////////////////////////////////////////////////////////////////
//
//			ÓÐÏÞ×´Ì¬»ú
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <queue>

#include "InfoManagerTemplate.h"
enum DirectionID
{
	DirectionID_Jump,
	DirectionID_Left,
	DirectionID_Right,
	DirectionID_Up,
	DirectionID_Down,
	DirectionID_StopLeft,
	DirectionID_StopRight,
	DirectionID_StopUp,
	DirectionID_StopDown,
};
enum EventID
{
	EventID_StartRun,
	EventID_StartWalk,
	EventID_StartJump,
	EventID_StopJump,
	EventID_StopRun,
	EventID_StopWalk,
};

class IEvent
{
public:
	IEvent(){};
	virtual ~IEvent(){};
	virtual int ID() = 0;
	virtual int GetKey() = 0;
	virtual IEvent* Clone() =0 ;
	// 	virtual bool ForceState() = 0;
	// 	virtual void DesStateID() = 0;
};

class CGEEvent:public IEvent
{
public:
	CGEEvent(int nID,int nKey = 0):m_ID(nID),m_nKey(nKey){};
	virtual int ID() 
	{
		return m_ID;
	};
	virtual int GetKey() 
	{
		return m_nKey;
	}
	virtual IEvent* Clone()
	{
		IEvent* pNew = new CGEEvent(this->ID());
		return pNew;
	}
private:
	int m_ID;
	int m_nKey;
};

template<typename T_STATE>
class CFSM_T
{
	struct EventShell
	{
		EventShell(int nID,IEvent* pEvent)
			:m_nEventID(nID),m_pEvent(pEvent)
		{
		}
		int m_nEventID;
		IEvent* m_pEvent;
	};

public:
	typedef CInfoManagerTemplate<T_STATE> Container;

	CFSM_T(int nStateCount = 0)
		:m_Container(nStateCount)
	{
		Clear();
	}
	virtual ~CFSM_T(){};
	void Add(T_STATE* pS,int nID)
	{
		GetContainer().Add(pS,nID);
	}

	void  FirstStart(int nID)
	{
		m_nFirstStart = nID;
	}
	void Start(int nID)
	{
		m_nToProcess = nID;
	}
	void Stop(int nID)
	{
		m_bStop = true;
		return ;
	}
	void Jump(int nID)
	{
		if(m_pCurrentState && nID != m_pCurrentState->ID())
		{
			m_bStop= true;
			m_nToProcess = nID;
			Refresh();
		}
	}
	void Interrupt(int nID)
	{
		return;
	}

	void Resume()
	{
		if (m_bPausing && m_pCurrentState)
		{
			m_bPausing = false;
			m_pCurrentState->On_Resume();
		}
	}

	void Pause()
	{
		if (!m_bPausing && m_pCurrentState)
		{
			m_bPausing = true;
			m_pCurrentState->On_Resume();
		}
	}
	void Stop()
	{
		if(m_pCurrentState)
		{
			if (m_bPausing)
			{
				Resume();
			}
			m_pCurrentState->On_Stop();
			m_pCurrentState = 0;
		}
	}

	void Process()
	{
		if(m_pCurrentState)
		{
			m_bInProcessing = true;
			m_pCurrentState->On_Process();
			m_bInProcessing = false;
			_ResponseDelayed();
			Refresh();
		}
	}

	bool Response(IEvent& rEvent)
	{
		if(m_pCurrentState ==0)
		{
			return false;
		}
		// 		if(m_bInProcessing)
		// 		{
		// 			m_eventDelayed.push(EventShell(-1,rEvent.Clone()));
		// 			return true;
		// 		}
		bool bResult = m_pCurrentState->Transition(rEvent);
		if(bResult)
		{
			Refresh();
		}
		return bResult;
	}
	bool ResponseEX(int nID)
	{
		if(m_pCurrentState == 0)
		{
			return false;
		}
		if(m_bInProcessing)
		{
			m_eventDelayed.push(EventShell(nID,0));
			return true;
		}
		bool bResult = m_pCurrentState->TransitionEX(nID);
		if(bResult)
		{
			Refresh();
		}
		return bResult;
	}
	void Start()
	{
		if(m_nFirstStart != -1)
		{
			Stop();
			Clear();
			Start(m_nFirstStart);
			Refresh();
		}
	}

	T_STATE& Get(int nID)
	{
		T_STATE* p = GetContainer().GetByID(nID);
		if(p)
		{
			return *p;
		}
	}

	T_STATE* CurrentStatePtr()
	{
		return m_pCurrentState;
	}

	void _ResponseDelayed()
	{
		while(!m_eventDelayed.empty())
		{
			EventShell es(m_eventDelayed.front());
			m_eventDelayed.pop();
			if(es.m_nEventID == -1)
			{
				IEvent * pEvent(es.m_pEvent);
				if (pEvent)
				{
					Response(*pEvent);
					delete pEvent;
				}
			}
			else
			{
				ResponseEX(es.m_nEventID);
			}
		}
	}

	Container& GetContainer(){return m_Container;};
	void Clear()
	{
		m_pCurrentState = 0;
		m_nToProcess = -1;
		m_bPausing = false;
		m_bStop = false;
	}

	void Refresh()
	{
		if(m_bPausing)
		{
			return;
		}
		if(m_bStop)
		{
			if(m_pCurrentState)
			{
				T_STATE* pCurrentState = m_pCurrentState;
				m_pCurrentState = 0;
				pCurrentState->On_Stop();
			}
			m_bStop = false;
		}
		if(m_nToProcess != -1)
		{
			if(m_pCurrentState)
			{
				m_pCurrentState->On_Stop();				 
			}
			m_pCurrentState = &Get(m_nToProcess);
			m_nToProcess = -1;
			if(m_pCurrentState)
			{
				m_pCurrentState->On_Start();
				//if(m_bStartProcessThisFrame)
				// {
				//	 Process();
				//}
			}
		}
	}

private:
	T_STATE* m_pCurrentState;
	std::queue<EventShell> m_eventDelayed;
	Container m_Container;
	int m_nFirstStart;
	int m_nToProcess;

	bool m_bPausing;
	bool m_bInProcessing;
	bool m_bStop;
	bool m_bStartProcessThisFrame;
};

