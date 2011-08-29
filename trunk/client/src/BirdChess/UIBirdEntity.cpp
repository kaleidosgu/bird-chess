#include "UIBirdEntity.h"
#include "hgeSprite.h"
#include "lua/CLuaParse.h"

const int nMAX_BIRDENTITYSPR_COUNT = 30;
using namespace std;
CBirdEntity::CBirdEntity(int nBirdType):m_x(0.0),m_y(0.0),m_CurrentSprID(0),m_fWidth(0.0),m_fHeight(0.0),m_nBirdType(nBirdType)
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
		//		spr->SetZ(0.5);
		spr->SetHotSpot(0,0);//¶¨Î»ÔÚ0,0

		//if(G_bDebug)
		//{
		//	m_sprDebug = new hgeSprite(tex,493, 744, 48, 48);
		//	m_sprDebug->SetHotSpot(24,24);
		//}
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