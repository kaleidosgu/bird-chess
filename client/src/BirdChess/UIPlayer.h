///************************************************************************/
///*				���,������ͷ����Ϣ,��ҿ�Ƭ��Ϣ,������Ϣ				*/
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
	int m_nPlayerID;						//���ID
	int m_nPlayerIndex;						//����ڷ����˳��
	int m_nPlayerBirdID;					//����������
	UIPlayerFace* m_PlayerFace;				//���ͷ��
	std::map<int,int> m_mapCardInfo;		//ӵ�е��Ƶ�����,����
	//std::vector<UICard*> m_VecCard;			//��ҵĿ����б�
	UICard* m_SendCard;						//��ǰ������

	bool m_nIsMainRole;						//�Ƿ�����...���ǵ�ͷ�����һЩ
	int m_nReadState;						//�Ƿ�Ready
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
	hgeGUI* m_PlayerUI;						//��ҵ�UI,���ǲ���
	UICard* m_BeginOrReady;					//׼����ť,�����Ŀ�ʼ��ť
};