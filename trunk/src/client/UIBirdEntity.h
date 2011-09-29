/************************************************************************/
/*				����,�в��״̬,,���ƶ�Ч��,�����,                     */
/************************************************************************/

#pragma  once 

#include <map>
#include <list>
#include <hge.h>
class hgeSprite;
struct EntitySize;
class CCloudEntity;

class CBirdEntity
{
public:
	CBirdEntity(int nBirdType);
	~CBirdEntity();
	hgeSprite* GetCurrentSpr();
	void Update(float dt);
	void Render();

	void SetTexture(HTEXTURE tex,float fLeftupX,float fLeftupY,float fWidth,float fHeight,unsigned int nID = 0);
	void SetTexture(HTEXTURE tex,std::list<EntitySize*>& listEntitySize);
	void SetCurrentSprID(int nID) ;
	int GetCurrentSprID(){return m_CurrentSprID;};
	float GetSprHeightBuSprID(int nIndex);

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

	void MoveTo(float fx,float fy);//�������������ٶ�
	void SetNext(CBirdEntity* pBird);
	CBirdEntity* GetNext() {return pNextBird;};

	int GetBirdType(){return m_nBirdType;};
	static bool GetBirdMoveAble(){return m_CanBirdMove;};

	int GetCloudIndex(){return m_nCloudIndex;};
	void  SetCloudIndex(int nCloudIndex){ m_nCloudIndex = nCloudIndex;};

	void SetCloud(CCloudEntity* pCloud){m_pCloud = pCloud;};
	CCloudEntity*  GetCloud(){return m_pCloud;};
	void  SetFace(bool nFace);
	void ChangeSpr();

private:
	int m_nBirdType;
	int m_nCloudIndex;

	int m_CurrentSprID;
	float m_x;
	float m_y;

	static float m_LogicOrdx;
	static float m_LogicOrdy;

	float m_fWidth;
	float m_fHeight;
	
	int m_nStateID;
	std::map<int,hgeSprite*> m_sprMap;    // 0-10 ������֡������   10-20������֡������  20-30����������
	//CBirdEntity* pPreBird;
	CBirdEntity* pNextBird;


	//���߼����õ�����
	float m_MoveTime;   //�켣�õ�ʱ��
	float m_Vx;			//X���ٶ�
	float m_Vy;			//Y���ٶ�
	float m_Vyz;		//Y����ٶ�
	
	float m_Orderx;
	float m_Ordery;

	bool m_bFaceRight;
	static bool m_CanBirdMove;

	CCloudEntity*	m_pCloud;
};