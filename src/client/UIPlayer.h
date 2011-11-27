///************************************************************************/
///*				玩家,包含了头像信息,玩家卡片信息,出牌信息				*/
///************************************************************************/

#pragma once
#include <map>

class UIPlayerFace;
class UICard;
class NONOUIText;

class UIPlayer
{
public:
	UIPlayer(const std::string& sName);
	virtual ~UIPlayer();
	virtual void Render();
	virtual void Update(float dt);

	void SetScale(float fScale);
	//void SetName(const std::string& sName);
	void SetPlayerIndex(int nIndex);
	void SetBirdID(int nBirdID);
	int GetBirdID(){return m_nPlayerBirdID;};
	void SetPlayerInfo(const std::string& sName,int nPlayerID);
	int GetPlayerID(){return m_nPlayerID;};
	virtual void SetCurrentPlayer(bool bIsCurrentPlayer);
	void SetVisiable(bool bShow){m_bShow = bShow;};
	void SetSendCard(int nCardType,int nCardInstruction,int nTargetCardType );
	const char* GetName();
private:
	float m_x;
	float m_y;
	float m_fScale;
	int m_nPlayerID;						//玩家ID
	int m_nPlayerIndex;						//玩家在房间的顺序
	int m_nPlayerBirdID;					//玩家鸟的类型
	UIPlayerFace* m_PlayerFace;				//玩家头像
	std::map<int,int> m_mapCardInfo;		//拥有的牌的索引,类型
	//std::vector<UICard*> m_VecCard		//玩家的卡牌列表
	UICard* m_SendCard;						//当前发送牌

	NONOUIText* m_ShowReadyAndCurPly;		//显示是否出牌
	bool m_nIsMainRole;						//是否主角...主角的头像更大一些
	int m_nReadState;						//是否准备	
	bool m_bIsCurrentPlayer;				//是否轮到出牌
	bool m_bShow;
	float m_fSendCardShowTime;
};

class hgeGUI;
class NONOhgeGUIText;
class hgeSprite;
class MainUIPlayer:public UIPlayer
{
public:
	MainUIPlayer(std::string sName);
	virtual ~MainUIPlayer();
	virtual void Render();
	virtual void Update(float dt);

	void SetPlayerCardInfo(int nPosID,int nType,int nInstruction);

	void SendCard2Server(int m_nCardID);
	void SetRoomID(int nRoomID);
	void SetMaster(bool bIsMaster);
	void SetGameStart(bool bStarted);
	void SetWinGame();
	virtual void SetCurrentPlayer(bool bIsCurrentPlayer);
private:
	enum{ePlayCard1= 1,ePlayCard2,ePlayCard3,ePlayCardType1,ePlayCardType2,ePlayCardType3,ePlayCardType4,ePlayCardType5,eChangeRoom,eTextRoomID,eBeginGame};
	hgeGUI* m_PlayerUI;						//玩家的UI,主角才有
	UICard* m_BeginOrReady;					//准备按钮,房主的开始按钮
	NONOhgeGUIText* m_RoomIDText;				//房间ID
	hgeSprite* pStrWinPrincess ;
	int m_nRoomID;
	int nCurCardPos;
	bool m_bIsMaster;
	float m_fWinGame;
};