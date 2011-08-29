/************************************************************************/
/*				鸟类,有层次状态,,带移动效果,会飞行,                     */
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
	std::map<int,hgeSprite*> m_sprMap;    // 0-10 是正常帧数区域   10-20是死亡帧数区域  20-30是特殊区域
	CBirdEntity* PreBird;
	CBirdEntity* NextBird;
};