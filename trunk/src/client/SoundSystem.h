#pragma once
#include "hge.h"

#include <map>

using namespace std;

#define PlaySoundByID(x,y) SoundSystem::Instance().PlaySound((x),(y))
#define PlaySoundByID(x) SoundSystem::Instance().PlaySound((x))

class SoundSystem
{
public:
	SoundSystem(void);
	static SoundSystem& Instance();
	static void init(HGE* pHge);

	~SoundSystem(void);
	void PlaySound(int nID,int nPer = 100);
	void PlayMusic(int nID);
	void StopMusic(int nID);

private:
	map<int,HEFFECT> m_mapSound;
	map<int,HEFFECT> m_mapMusic;
	HCHANNEL m_pBg;
	static HGE* m_pHge;
};
