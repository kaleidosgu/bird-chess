///************************************************************************/
///* ��ҵĹ�����,������ҵ�������Ϣ,�������紫�͹�������Ϣ				*/
///************************************************************************/
//
#pragma once
#include <vector>

class UIPlayer;
class MainUIPlayer;
class UIPlayerManage
{
public:
	UIPlayerManage();
	~UIPlayerManage();
	void Render();
	void Update(float fTime);
	//void SetPlayerInfo();  //�����������������Ϣ,����������Ϣ,���½�ɫ�б�,����ͷ��
	//UIPlayerManage& Instance();
	MainUIPlayer * GetMainPlayer(){return m_MainPlayer;};
	std::vector<UIPlayer*>& GetPlayerList(){return m_VecPlayer;};
	void SetRoomMater(int nMasterID);
	void SetCurrentPlayerID(int nCurrentPlayerID);
	bool IsMaster();
	void Response(int nCardType,int nCardInstruction,int nTargetCardType );

private:
	std::vector<UIPlayer*>  m_VecPlayer;
	MainUIPlayer * m_MainPlayer;//��������һ��Main,����Ҫɾ��
	int m_nMasterID;
	int m_nCurrentPlayerID;
};