/************************************************************************/
/* 小小的一片云呀,慢慢的走过来                                          */
/************************************************************************/
#pragma  once 

#include <map>

class hgeSprite;
class CCloudEntity
{
public:
	CCloudEntity();
	~CCloudEntity();
	void Render();
	void Update();
	//MoveTo()
private:
	int m_nCloudIndex;

	float m_x;
	float m_y;

	static float m_LogicOrdx;
	static float m_LogicOrdy;

	float m_fWidth;
	float m_fHeight;

	hgeSprite* m_Spr;
};