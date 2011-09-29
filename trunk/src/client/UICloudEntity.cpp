#include "UICloudEntity.h"
#include "hgeSprite.h"
#include "lua/CLuaParse.h"
#include "UIBirdEntity.h"
#include "BirdCardDefine.h"

const int nMAX_BIRDENTITYSPR_COUNT = 30;
const int nMOVE_SPEED_X = 300;


#pragma warning(push)
#pragma warning( disable:4244)
using namespace std;

float CCloudEntity::m_LogicOrdx =0;
float CCloudEntity::m_LogicOrdy = 0;
CCloudEntity::CCloudEntity(int nCloudIndex):m_x(0.0),m_y(0.0),m_CurrentSprID(0),m_fWidth(0.0),m_fHeight(0.0),m_nCloudIndex(nCloudIndex),pNextBird(NULL),m_MoveTime(0.0),m_Vx(300),m_Vy(0),m_Vyz(700)
{

}

CCloudEntity::~CCloudEntity()
{

}

void CCloudEntity::SetTexture(HTEXTURE tex,list<EntitySize*>& listEntitySize )
{
	list<EntitySize*>::iterator itBeg = listEntitySize.begin();
	list<EntitySize*>::iterator itEnd = listEntitySize.end();
	for(;itBeg!=itEnd;itBeg++)
	{
		EntitySize* pEntitySize = *itBeg;
		if(pEntitySize->nIndex >= nMAX_BIRDENTITYSPR_COUNT || tex == NULL)
		{
			continue;
		}

		hgeSprite* spr = new hgeSprite(tex,pEntitySize->x,pEntitySize->y,pEntitySize->w,pEntitySize->h);
		spr->SetHotSpot(0,pEntitySize->h);//��λ��0,pEntitySize->h
		m_fWidth = pEntitySize->w;
		m_fHeight = pEntitySize->h;

		if( m_sprMap[pEntitySize->nIndex] == NULL)
		{
			delete m_sprMap[pEntitySize->nIndex];
			m_sprMap[pEntitySize->nIndex] = spr;
		}
		else
		{
			m_sprMap[pEntitySize->nIndex] = spr;
		}
	}
}

void CCloudEntity::SetCurrentSprID(int nID) 
{
	if(nID < nMAX_BIRDENTITYSPR_COUNT)	
	{
		m_CurrentSprID = nID;
		if( GetCurrentSpr())
		{
			m_fHeight = GetCurrentSpr()->GetHeight();
			m_fWidth = GetCurrentSpr()->GetWidth();
		}
	}
}


hgeSprite* CCloudEntity::GetCurrentSpr()
{
	if( m_sprMap[m_CurrentSprID])
	{
		return m_sprMap[m_CurrentSprID];
	}
	return NULL;
}

void CCloudEntity::Update(float dt)
{
	//��Ҫ��λ�õĸı�
	//�����������������,�����X���ٶ��Լ��������ٶȿ���ȷ��Y��ĳ�ʼ�ٶ�   Vx = x2-x1/t  -> t = x2-x1/Vx  ���t,   Y����ٶ�Vi ,���ݼ��ٶȹ�ʽ�Ƴ�   Vy= ((y2-y1)*2 - Vy*t^2)/2t,
	if(pNextBird) //������������Ŷ�
		pNextBird->SetLocation(m_x,m_y + GetHeight());

	if(m_MoveTime>0.0)
	{
		m_x += m_Vx*dt;
		m_Vy += m_Vyz*dt;
		m_y += m_Vy*dt;
		m_MoveTime -=dt;
	}
}


void CCloudEntity::Render()
{
	if( GetCurrentSpr())
		GetCurrentSpr()->Render(GetRendX(),GetRendY());
}


void CCloudEntity::MoveTo(float fx,float fy)//�������������ٶȺ�ʱ��
{
	m_MoveTime =  abs(fx-m_x)/nMOVE_SPEED_X;
	m_Vx = (fx-m_x)>0?nMOVE_SPEED_X:-nMOVE_SPEED_X;
	m_Vy = ((fy - m_y)*2 - m_Vyz*(m_MoveTime*m_MoveTime)) / (2*m_MoveTime); //Vy= ((y2-y1)*2 - Vy*t^2)/2t,
}

void CCloudEntity::PushBird(CBirdEntity* pBird)
{
	if(!m_lstBird.empty())
	{
		CBirdEntity* pBacBird = m_lstBird.back(); //�������������nex���ó��µ�
		pBacBird->SetNext(pBird);
	}

	CBirdEntity* pCurBird = pBird;
	while(pCurBird)
	{
		m_lstBird.push_back(pCurBird);			//push����
		pCurBird->SetCloudIndex(m_nCloudIndex); //����cloud
		pCurBird->SetCloud(this); //����cloud
		pCurBird= pCurBird->GetNext();			//push������һ��
	}
}

void CCloudEntity::PopBird(CBirdEntity* pBird)
{
	list<CBirdEntity* >::iterator itBegin = m_lstBird.begin();
	bool bDelNext = false;
	for(;itBegin!=m_lstBird.end();)
	{
		if(*itBegin == pBird || bDelNext)
		{
			bDelNext = true;
			m_lstBird.erase(itBegin++);
		}
		else
			itBegin++;
	}
	if(!m_lstBird.empty())
	{
		CBirdEntity* pBird = m_lstBird.back();
		pBird->SetNext(NULL);
	}
	ResetCloudLocate();
}
void CCloudEntity::ResetCloudLocate()
{
	list<CBirdEntity* >::iterator itBegin = m_lstBird.begin();
	for(;itBegin!=m_lstBird.end();++itBegin)
	{
		(*itBegin)->ChangeSpr();
		(*itBegin)->SetLocation((*itBegin)->GetX(),(*itBegin)->GetY()+5);
	}
	if(m_lstBird.empty())
		SetLocation(m_x,m_y-5);
	else
		SetLocation(m_x,m_y+5);
}

float CCloudEntity::GetHeightIncludeBird()
{
	if(m_lstBird.empty())
	{
		return m_y-GetHeight()+25;
	}
	else
	{
		CBirdEntity* pBird = m_lstBird.back();
		return pBird->GetY() - pBird->GetSprHeightBuSprID(BirdState_Up)+8;
	}
}


void CCloudEntity::Init()
{
	m_lstBird.clear();
}

#pragma warning (push)