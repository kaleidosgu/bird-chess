///************************************************************************/
///*				���,������ͷ����Ϣ,��ҿ�Ƭ��Ϣ,������Ϣ				*/
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
	int m_nPlayerID;						//���ID
	int m_nPlayerIndex;						//����ڷ����˳��
	int m_nPlayerBirdID;					//����������
	UIPlayerFace* m_PlayerFace;				//���ͷ��
	std::map<int,int> m_mapCardInfo;		//ӵ�е��Ƶ�����,����
	//std::vector<UICard*> m_VecCard		//��ҵĿ����б�
	UICard* m_SendCard;						//��ǰ������

	NONOUIText* m_ShowReadyAndCurPly;		//��ʾ�Ƿ����
	bool m_nIsMainRole;						//�Ƿ�����...���ǵ�ͷ�����һЩ
	int m_nReadState;						//�Ƿ�׼��	
	bool m_bIsCurrentPlayer;				//�Ƿ��ֵ�����
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
	hgeGUI* m_PlayerUI;						//��ҵ�UI,���ǲ���
	UICard* m_BeginOrReady;					//׼����ť,�����Ŀ�ʼ��ť
	NONOhgeGUIText* m_RoomIDText;				//����ID
	hgeSprite* pStrWinPrincess ;
	int m_nRoomID;
	int nCurCardPos;
	bool m_bIsMaster;
	float m_fWinGame;
};