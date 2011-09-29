#include "AddPlayerSpeedItem.h"
#include "GameDefine.h"


AddPlayerSpeedItem::AddPlayerSpeedItem(HeroPig& rMainPlayer):m_rMainPlayer(rMainPlayer),MainPar(NULL)
{
	SetEffectTime(15);
}

AddPlayerSpeedItem::~AddPlayerSpeedItem(void)
{
	delete MainPar;
}

void AddPlayerSpeedItem::ItemEffect()
{
	PlaySound(32);
	m_rMainPlayer.SetScale(0.5);
	MainPar=new hgeParticleSystem("PigMove.psi",m_rMainPlayer.GetCurrentSpr());
	MainPar->info.colColorStart = 0x60FFFFFF;
	MainPar->info.fSizeStart = 0.3;
	MainPar->info.fSizeEnd = 0.3;
	MainPar->Fire();

	m_rMainPlayer.SetAddSpeed(m_rMainPlayer.GetAddSpeed() + 10.0);
	ItemBase::ItemEffect();
}

void AddPlayerSpeedItem::RemoveItemEffect()
{
	m_rMainPlayer.SetAddSpeed(m_rMainPlayer.GetAddSpeed() - 10.0);
	m_rMainPlayer.SetScale(1.0);

	MainPar->Stop();
}

void AddPlayerSpeedItem::Update(float dt)
{
	if( m_nItemState == ENUM_ITEM_EFFECT)
	{
		//MainPar->TrackBoundingBox(false);
		MainPar->info.nEmission=(int)(m_rMainPlayer.GetRendX()*m_rMainPlayer.GetRendX()+m_rMainPlayer.GetRendY()*m_rMainPlayer.GetRendY())*2;
		MainPar->MoveTo(m_rMainPlayer.GetRendX(),m_rMainPlayer.GetRendY());
		MainPar->Update(dt);
		//MainPar->FireAt(m_rMainPlayer.GetX(),m_rMainPlayer.GetY());;
	}
	ItemBase::Update(dt);
}

void AddPlayerSpeedItem::Render()
{	
	if( m_nItemState == ENUM_ITEM_EFFECT)
		MainPar->Render();
	else
		ItemBase::Render();
}
//void 
