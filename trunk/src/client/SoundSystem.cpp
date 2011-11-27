#include "SoundSystem.h"

HGE* SoundSystem::m_pHge=0;
SoundSystem::SoundSystem(void):m_pBg(NULL),m_bEnableSound(true)
{
	m_mapSound[0] = m_pHge->Effect_Load("Res/GameSound/bird 01 flying.wav");
	m_mapSound[1] = m_pHge->Effect_Load("Res/GameSound/bird 02 flying.wav");
	m_mapSound[2] = m_pHge->Effect_Load("Res/GameSound/bird 04 flying.wav");
	m_mapSound[3] = m_pHge->Effect_Load("Res/GameSound/bird 05 flying.wav");
	m_mapSound[4] = m_pHge->Effect_Load("Res/GameSound/bird pig flying.wav");


	m_mapSound[5] = m_pHge->Effect_Load("Res/GameSound/BirdDead0.wav");
	m_mapSound[6] = m_pHge->Effect_Load("Res/GameSound/BirdDead1.wav");
	m_mapSound[7] =	m_pHge->Effect_Load("Res/GameSound/BirdDead2.wav");
	m_mapSound[8] = m_pHge->Effect_Load("Res/GameSound/BirdDead3.wav");
	//m_mapSound[9] = m_pHge->Effect_Load("Res/GameSound/BirdDead4.wav");
	m_mapSound[9] = m_pHge->Effect_Load("Res/GameSound/BirdDead5.wav");
	m_mapSound[10] = m_pHge->Effect_Load("Res/GameSound/BirdDead6.wav");

	m_mapSound[11] = m_pHge->Effect_Load("Res/GameSound/PlayDead.mp3");
	m_mapSound[12] = m_pHge->Effect_Load("Res/GameSound/LevelUp.wav");
	m_mapSound[13] = m_pHge->Effect_Load("Res/GameSound/CreatBird.wav");
	m_mapSound[14] = m_pHge->Effect_Load("Res/GameSound/CreateItem.wav");
	m_mapSound[15] = m_pHge->Effect_Load("Res/GameSound/PlayDamage.wav");

	m_mapSound[21] = m_pHge->Effect_Load("Res/GameSound/CreatBullet.wav");
	m_mapSound[22] = m_pHge->Effect_Load("Res/GameSound/CreatBullet2.wav");
	m_mapSound[22] = m_pHge->Effect_Load("Res/GameSound/CreatBullet3.wav");
	m_mapSound[23] = m_pHge->Effect_Load("Res/GameSound/UseBomb.wav");
	m_mapSound[24] = m_pHge->Effect_Load("Res/GameSound/UseBombbb.wav");
	

	m_mapSound[31] = m_pHge->Effect_Load("Res/GameSound/Power.wav");
	m_mapSound[32] = m_pHge->Effect_Load("Res/GameSound/AddSpeedItem.wav");
	m_mapSound[33] = m_pHge->Effect_Load("Res/GameSound/SlowBirdItem.wav");
	m_mapSound[34] = m_pHge->Effect_Load("Res/GameSound/QueenKissItem.wav");
	m_mapSound[35] = m_pHge->Effect_Load("Res/GameSound/AddLifeItem.wav");
	m_mapSound[36] = m_pHge->Effect_Load("Res/GameSound/RealPigItem.wav");
	m_mapSound[37] = m_pHge->Effect_Load("Res/GameSound/BecomeBirdItem.wav");

	m_mapSound[38] = m_pHge->Effect_Load("Res/GameSound/StartGame.mp3");


	m_mapMusic[1] = m_pHge->Effect_Load("Res/GameSound/BGMusic.mp3");
	m_mapMusic[2] = m_pHge->Effect_Load("Res/GameSound/ReadyBGMusic.mp3");
	m_mapMusic[3] = m_pHge->Effect_Load("Res/GameSound/she_sailed_away.mp3");
}

SoundSystem::~SoundSystem(void)
{
}


void SoundSystem::PlaySound(int nID,int nPer)
{
	if(m_bEnableSound)
	{
		map<int,HEFFECT>::iterator it = m_mapSound.find(nID);
		if(it!= m_mapSound.end())
			m_pHge->Effect_PlayEx(m_mapSound[nID],nPer);
	}
	
}
void SoundSystem::PlayMusic(int nID)
{
	//if(m_mapMusic[nID])
	//{
	//	if(m_pBg)
	//	{
	//		m_pHge->Channel_Stop(m_pBg);
	//	}
	if(m_bEnableSound)
	{
		map<int,HEFFECT>::iterator it = m_mapMusic.find(nID);
		if(it!= m_mapMusic.end())
			m_pBg =m_pHge->Effect_PlayEx(m_mapMusic[nID],70);
	}
}
void SoundSystem::init(HGE* pHge)
{
	m_pHge = pHge;
}

SoundSystem& SoundSystem::Instance()
{
	static SoundSystem SoundSys;
	return SoundSys;
}

void SoundSystem::StopMusic(int nID)
{
	//map<int,HEFFECT>::iterator it = m_mapMusic.find(nID);
	//if(it!= m_mapMusic.end())
	//	m_pHge->Channel_Stop(m_mapMusic[nID]);
	m_pHge->Channel_Stop(m_pBg);
}

void SoundSystem::SetEnable(bool bEnableSound)
{
	StopMusic(-1);
	m_bEnableSound = bEnableSound;
}

