///************************************************************************/
///* 打鸟鸟的游戏房间,包含所有信息,接收消息并且分发                       */
///************************************************************************/
//

#pragma once;

#include <vector>
#include <map>
#include <deque>
#include <list>
#include <string>

#include"./NonoHGECtrlEx.h"

class CCloudEntity;
class CBirdEntity;
class UIPlayerManage;
class CardCmdBase;
struct MSG_CARDGAME_S2C_PlayerInfo;
struct MSG_BASE;
class GfxEdit;

class hgeFont;

class CUIGround
{
public:
	CUIGround();
	~CUIGround();
	void Render();
	void Update(float dt);
	void Response(int nCardType,int nCardInstruction,int nTargetCardType );
	void SetOrderY(float fOrdy);
	void SetOrderX(float fOrdx);

	void SetMainPlayerInfo(MSG_CARDGAME_S2C_PlayerInfo& rPlayerInfo);
	void SetPlayerInfo(MSG_CARDGAME_S2C_PlayerInfo& rPlayerInfo);
	void SetPlayerBirdID(int nPlayerCardType,bool MainPlayer = true);
	void SetPlayerCardInfo(int nPosID,int nType,int nInstruction);
	void SetRoomMater(int nID);
	void PlayerLeave(int nID);
	void SetCurrentTurnPlayer(int nPlayerID);
	
	void SetReady(bool bReady){m_bReady = bReady;};
	void SetGameState(bool bGameState);
	void SetRoomID(int nRoomID);
	void Init();
	void SetWinner(int nPlayerID);
	void ShowChat(const std::string& strChat,int nType = 0);
	//UIPlayerManage* GetPlayerManager(){return m_PlayerManager;}; //为了个提示这么改不好,有时间改掉.让提示是随处可用的
	static void SendChat(const std::string& strChat,int nType = 0);
	static void SendMsg(MSG_BASE& pMsg);
	static void OnEnter(GfxEdit* m_Edit);
	static void CmdProcess(const std::string& strCmd);
private:
	typedef  NONOGFXUIText<GfxFont> UIChatTxt ;
	std::vector<CCloudEntity*> m_vecCloud;
	std::vector<CBirdEntity*> m_vecBird;

	//std::map<int,std::list<CBirdEntity*> > m_mapCloundState;
	UIPlayerManage* m_PlayerManager;
	std::deque<CardCmdBase* > m_vecCardCmd;
	std::deque<CardCmdBase* > m_vecCardCmdback;
	std::deque<UIChatTxt*> m_vecChat;
	GfxEdit* m_Edit;
	bool m_bReady;
	static char m_mainPlayerName[128];
	static int m_nRoomID;
};
extern CUIGround* g_UIGround;