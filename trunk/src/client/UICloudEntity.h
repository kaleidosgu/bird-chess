/************************************************************************/
/* СС��һƬ��ѽ,�������߹���                                          */
/************************************************************************/
#pragma  once 


#include <map>
#include <list>
#include <hge.h>


class hgeSprite;
struct EntitySize;
class CBirdEntity;
class CCloudEntity
{
public:
	CCloudEntity(int nCloudIndex);
	~CCloudEntity();
	hgeSprite* GetCurrentSpr();
	void Update(float dt);
	void Render();

	void SetTexture(HTEXTURE tex,float fLeftupX,float fLeftupY,float fWidth,float fHeight,unsigned int nID = 0);
	void SetTexture(HTEXTURE tex,std::list<EntitySize*>& listEntitySize);
	void SetCurrentSprID(int nID) ;
	int GetCurrentSprID(){return m_CurrentSprID;};

	float GetX(){return m_x;}
	float GetY(){return m_y;}
	void SetOrderX(float fOrdx){ m_LogicOrdx= fOrdx;};
	void  SetOrderY(float fOrdy){ m_LogicOrdy=fOrdy; };
	float GetWidth(){ return m_fWidth;};
	float GetHeight(){ return m_fHeight;};
	float GetRendX() {return m_x - m_LogicOrdx;};
	float GetRendY() {return m_y - m_LogicOrdy;};
	void SetLocation(float fX,float fY)
	{
		m_x = fX;
		m_y = fY;
	};

	void ResetCloudLocate();
	void MoveTo(float fx,float fy);//�������������ٶ�
	void SetNext(CCloudEntity* pBird) {pNextBird = pBird;};
	CCloudEntity* GetNext(CCloudEntity* pBird) {return pNextBird;};
	
	void PushBird(CBirdEntity* pBird);
	void PopBird(CBirdEntity* pBird);
	
	std::list<CBirdEntity* >& GetBirdList() {return m_lstBird;};

	float GetHeightIncludeBird();
	int GetCloudIndex(){return m_nCloudIndex;};
	void  Init();
private:
	int m_nCloudIndex;
	int m_nCloudID;

	int m_CurrentSprID;
	float m_x;
	float m_y;

	static float m_LogicOrdx;
	static float m_LogicOrdy;

	float m_fWidth;
	float m_fHeight;

	int m_nStateID;
	std::map<int,hgeSprite*> m_sprMap;    // 0-10 ������֡������   10-20������֡������  20-30����������
	CCloudEntity* pNextBird;


	//���߼����õ�����
	float m_MoveTime;   //�켣�õ�ʱ��
	float m_Vx;			//X���ٶ�
	float m_Vy;			//Y���ٶ�
	float m_Vyz;		//Y����ٶ�

	std::list<CBirdEntity* > m_lstBird;
};