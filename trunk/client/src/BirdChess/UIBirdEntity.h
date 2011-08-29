/************************************************************************/
/*				����,�в��״̬,,���ƶ�Ч��,�����,                     */
/************************************************************************/

#pragma  once 

#include <map>
#include <list>
#include <hge.h>
class hgeSprite;
class EntitySize;
class CBirdEntity
{
public:
	CBirdEntity(int nBirdType);
	~CBirdEntity();
	void SetTexture(HTEXTURE tex,std::list<EntitySize*>& listEntitySize );
	hgeSprite* GetCurrentSpr();
	//MoveTo()
private:
	int m_nBirdType;

	int m_CurrentSprID;
	float m_x;
	float m_y;

	static float m_LogicOrdx;
	static float m_LogicOrdy;

	float m_fWidth;
	float m_fHeight;
	
	int m_nStateID;
	std::map<int,hgeSprite*> m_sprMap;    // 0-10 ������֡������   10-20������֡������  20-30����������
	CBirdEntity* PreBird;
	CBirdEntity* NextBird;
};