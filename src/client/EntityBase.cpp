#include "EntityBase.h"
#include "GameDefine.h"
#include "SoundSystem.h"
#include "lua/CLuaParse.h"


#pragma warning(push)
#pragma warning( disable:4244)
float EntityBase::m_LogicOrdx =0;
float EntityBase::m_LogicOrdy = 0;
EntityBase::EntityBase(void):m_x(0.0),m_y(0.0),m_CurrentSprID(0),m_fWidth(0.0),m_fHeight(0.0),m_nLife(1),m_sprDebug(NULL),m_nEntityStat(ENUM_ENTITY_NORMAL),m_fDesotryTime(0.3f)
{

}

EntityBase::~EntityBase(void)
{
	map<int,hgeSprite*>::iterator itBeg =m_sprMap.begin();
	map<int,hgeSprite*>::iterator itEnd =m_sprMap.end();
	for(;itBeg!=itEnd;itBeg++)
	{
		delete itBeg->second;
	}
}

void EntityBase::SetTexture(HTEXTURE tex,list<EntitySize*>& listEntitySize )
{
	list<EntitySize*>::iterator itBeg = listEntitySize.begin();
	list<EntitySize*>::iterator itEnd = listEntitySize.end();
	for(;itBeg!=itEnd;itBeg++)
	{
		EntitySize* pEntitySize = *itBeg;
		if(pEntitySize->nIndex >= MAX_ENTITYSPR_COUNT || tex == NULL)
		{
			continue;
		}

		hgeSprite* spr = new hgeSprite(tex,pEntitySize->x,pEntitySize->y,pEntitySize->w,pEntitySize->h);
//		spr->SetZ(0.5);
		spr->SetHotSpot(pEntitySize->w/2,pEntitySize->h/2);//定位在中间的位置

		if(G_bDebug)
		{
			m_sprDebug = new hgeSprite(tex,493, 744, 48, 48);
			m_sprDebug->SetHotSpot(24,24);
		}
		m_fWidth = pEntitySize->w;
		m_fHeight = pEntitySize->h;
		m_tex = tex;

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

void EntityBase::SetTexture(HTEXTURE tex,float fLeftupX,float fLeftupY,float fWidth,float fHeight,unsigned int nID)
{
	if(nID >= MAX_ENTITYSPR_COUNT || tex == NULL)
	{
		return;
	}

	hgeSprite* spr = new hgeSprite(tex,fLeftupX,fLeftupY,fWidth,fHeight);
	spr->SetHotSpot(fWidth/2,fHeight/2);//定位在中间的位置
	
	if(m_sprDebug == NULL)
	{
		m_sprDebug = new hgeSprite(tex,493, 744, 48, 48);
		m_sprDebug->SetHotSpot(24,24);
	}
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_tex = tex;

	if( m_sprMap[nID] == NULL)
	{
		delete m_sprMap[nID];
		m_sprMap[nID] = spr;
	}
	else
	{
		m_sprMap[nID] = spr;
	}
}
void EntityBase::UpdateSpr()
{
}

void EntityBase::Update(float dt)
{
	if(m_nEntityStat == ENUM_ENTITY_DESTORYING )
	{
		m_fDesotryTime -= dt;
		if(m_fDesotryTime<=0.0) 
		{
			m_nEntityStat = ENUM_ENTITY_NEEDDESTORY ;
		}
	}
}
void EntityBase::SetCurrentSprID(int nID) 
{
	if(nID < MAX_ENTITYSPR_COUNT)	
	{
		m_CurrentSprID = nID;
		if( GetCurrentSpr())
		{
			m_fHeight = GetCurrentSpr()->GetHeight();
			m_fWidth = GetCurrentSpr()->GetWidth();
		}
	}
}

hgeSprite* EntityBase::GetCurrentSpr()
{
	if( m_sprMap[m_CurrentSprID])
	{
		return m_sprMap[m_CurrentSprID];
	}
	return NULL;
}

void EntityBase::Render()
{
	if(m_nEntityStat != ENUM_ENTITY_NEEDDESTORY && GetCurrentSpr())
		GetCurrentSpr()->Render(GetRendX(),GetRendY());
	if(m_sprDebug && G_bDebug)
	{
		m_sprDebug->RenderEx(GetRendX(),GetRendY(),m_fWidth/48);
	}

}
void EntityBase::Render(float x, float y)
{
	if(m_nEntityStat != ENUM_ENTITY_NEEDDESTORY&&GetCurrentSpr())
		GetCurrentSpr()->Render(x-m_LogicOrdx,y-m_LogicOrdy);
	if(m_sprDebug && G_bDebug)
	{
		m_sprDebug->RenderEx(x-m_LogicOrdx,y-m_LogicOrdy,m_fWidth/48);
	}
}
void EntityBase::RenderEx(float x, float y, float rot, float hscale, float vscale)
{
	if(m_nEntityStat != ENUM_ENTITY_NEEDDESTORY&&GetCurrentSpr())
	{
		GetCurrentSpr()->RenderEx(x-m_LogicOrdx, y-m_LogicOrdy, rot, hscale,vscale);
		if(m_sprDebug&&G_bDebug)
			m_sprDebug->RenderEx(x-m_LogicOrdx,y-m_LogicOrdy,m_fWidth*hscale/48);
	}
}

void EntityBase::SetLocation(float nX,float nY)
{
	m_x = nX;
	m_y = nY;
}

void EntityBase::SetDamage(int nDamage)
{
	m_nLife -= nDamage;
	if ( m_nLife <=0 )
	{
		m_nEntityStat = ENUM_ENTITY_NOTALIVE;
	}
}


bool EntityBase::IsAlive()
{
	return m_nEntityStat == ENUM_ENTITY_NORMAL; 
}

void EntityBase::OnDestory()
{
	m_nEntityStat = ENUM_ENTITY_DESTORYING;
	map<int,hgeSprite*>::iterator it = m_sprMap.find(DESTORY_ENTITYSPR_COUNT);
	if(it!= m_sprMap.end())
		SetCurrentSprID(DESTORY_ENTITYSPR_COUNT);
}


void EntityBase::PlaySound(int nID,int nPre)
{
	SoundSystem::Instance().PlaySound(nID,nPre);
}
#pragma warning(pop)