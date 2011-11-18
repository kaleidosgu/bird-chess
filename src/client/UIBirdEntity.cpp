#include "UIBirdEntity.h"
#include "hgeSprite.h"
#include "lua/CLuaParse.h"
#include "BirdCardDefine.h"
#include "UICloudEntity.h"
#include "SoundSystem.h"

#pragma warning(push)
#pragma warning( disable:4244)
using namespace std;

const int nMAX_BIRDENTITYSPR_COUNT = 30;
extern float G_OrderLogic;
float CBirdEntity::m_LogicOrdx =0;
float CBirdEntity::m_LogicOrdy = 0;
bool CBirdEntity::m_CanBirdMove = true;
CBirdEntity::CBirdEntity(int nBirdType):m_x(0.0),m_y(0.0),m_CurrentSprID(0),m_fWidth(0.0),m_fHeight(0.0),m_nBirdType(nBirdType),pNextBird(NULL),m_MoveTime(0.0),m_Vx(100),m_Vy(0),m_Vyz(1000),m_nCloudIndex(-1),m_bFaceRight(true),m_pCloud(NULL)
{

}

CBirdEntity::~CBirdEntity()
{

}

void CBirdEntity::SetTexture(HTEXTURE tex,list<EntitySize*>& listEntitySize )
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
		spr->SetHotSpot(0,pEntitySize->h);//��λ�����½�
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

void CBirdEntity::SetCurrentSprID(int nID) 
{
	if(nID < nMAX_BIRDENTITYSPR_COUNT)	
	{
		m_CurrentSprID = nID;
		if( GetCurrentSpr())
		{
			m_fHeight = GetCurrentSpr()->GetHeight();
			m_fWidth = GetCurrentSpr()->GetWidth();
			GetCurrentSpr()->SetFlip(!m_bFaceRight,false,false);
		}
	}
}


hgeSprite* CBirdEntity::GetCurrentSpr()
{
	if( m_sprMap[m_CurrentSprID])
	{
		return m_sprMap[m_CurrentSprID];
	}
	return NULL;
}

void CBirdEntity::Update(float dt)
{
	//��Ҫ��λ�õĸı�
	//�����������������,�����X���ٶ��Լ��������ٶȿ���ȷ��Y��ĳ�ʼ�ٶ�   Vx = x2-x1/t  -> t = x2-x1/Vx  ���t,   Y����ٶ�Vi ,���ݼ��ٶȹ�ʽ�Ƴ�   Vy= ((y2-y1)*2 - Vy*t^2)/2t,


	if(m_MoveTime>0.0)
	{
		
		m_x += m_Vx*dt;
		m_Vy += m_Vyz*dt;
		m_y += m_Vy*dt;
		m_MoveTime -=dt;

		//m_LogicOrdx = m_x-800;
		//m_pCloud->SetOrderX(m_LogicOrdx );
		G_OrderLogic = m_x-600;
		if(m_MoveTime<=0)//����float�ļ������С���,so~~
		{
			m_x = m_Orderx;
			m_y = m_Ordery;
			ChangeSpr();
			if(m_pCloud)
				m_pCloud->ResetCloudLocate();
			m_CanBirdMove = true;

			int nDownSound = rand()%6+5;
			SoundSystem::Instance().PlaySound(nDownSound);
		}
		CBirdEntity* pBird =  pNextBird;
		float tempy = m_y - GetSprHeightBuSprID(BirdState_Up);
		while(pBird) //������������Ŷ�
		{
			pBird->SetLocation(m_x,tempy);
			pBird->SetFace(m_bFaceRight);
			tempy = pBird->GetY()- pBird->GetSprHeightBuSprID(BirdState_Up);
			pBird =  pBird->GetNext();
		}
		
		m_Vx>0?m_bFaceRight = true:m_bFaceRight = false;
		GetCurrentSpr()->SetFlip(!m_bFaceRight,false,false);	
	}
}

void  CBirdEntity::SetFace(bool nFace)
{
	m_bFaceRight = nFace;
	GetCurrentSpr()->SetFlip(!m_bFaceRight,false,false);
};


void CBirdEntity::Render()
{
	if( GetCurrentSpr())
		GetCurrentSpr()->Render(GetRendX(),GetRendY());
}


void CBirdEntity::MoveTo(float fx,float fy)//�������������ٶȺ�ʱ��
{
	m_MoveTime = float(abs(fx-m_x)/300.0); //300���ٶ�
	m_Vx = (fx-m_x)>0?300:-300;
	m_Vy = ((fy - m_y)*2 - m_Vyz*(m_MoveTime*m_MoveTime)) / (2*m_MoveTime);

	m_Orderx = fx;
	m_Ordery = fy;
	m_CanBirdMove = false;
	SetCurrentSprID(BirdState_Fly);
	SoundSystem::Instance().PlaySound(m_nBirdType,70);
}

void CBirdEntity::ChangeSpr()
{
	if(pNextBird)
	{
		SetCurrentSprID(BirdState_Up);
	}
	else
	{
		SetCurrentSprID(BirdState_Top);
	}
}
void CBirdEntity::SetNext(CBirdEntity* pBird)
{
	pNextBird = pBird;
	//ChangeSpr();
}

float CBirdEntity::GetSprHeightBuSprID(int nIndex)
{
	return m_sprMap[nIndex]->GetHeight();
};
#pragma warning (push)