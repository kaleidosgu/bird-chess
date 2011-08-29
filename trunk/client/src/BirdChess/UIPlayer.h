///************************************************************************/
///*				玩家,包含了头像信息,玩家卡片信息,出牌信息				*/
///************************************************************************/

#pragma once
#include <map>

class UIPlayerFace;
class UICard;

class UIPlayer
{
public:
	UIPlayer(const std::string& sName);
	virtual ~UIPlayer();
	virtual void Render();
	virtual void Update(float dt);

	void SetScale(float fScale);
	void SetName(const std::string& sName);
	void SetPlayerIndex(int nIndex){ m_nPlayerIndex = nIndex;};
	void SetBirdID(int nBirdID);
private:
	int m_nPlayerID;						//玩家ID
	int m_nPlayerIndex;						//玩家在房间的顺序
	int m_nPlayerBirdID;					//玩家鸟的类型
	UIPlayerFace* m_PlayerFace;				//玩家头像
	std::map<int,int> m_mapCardInfo;		//拥有的牌的索引,类型
	//std::vector<UICard*> m_VecCard;			//玩家的卡牌列表
	UICard* m_SendCard;						//当前发送牌

	bool m_nIsMainRole;						//是否主角...主角的头像更大一些
	int m_nReadState;						//是否Ready
};

class hgeGUI;
class MainUIPlayer:public UIPlayer
{
public:
	MainUIPlayer(std::string sName);
	virtual ~MainUIPlayer();
	virtual void Render();
	virtual void Update(float dt);

	void SendCard2Server(int m_nCardID);
private:
	hgeGUI* m_PlayerUI;						//玩家的UI,主角才有
	UICard* m_BeginOrReady;					//准备按钮,房主的开始按钮
};