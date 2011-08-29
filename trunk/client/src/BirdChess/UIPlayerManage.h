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
	
	
private:
	std::vector<UIPlayer*>  m_VecPlayer;
	MainUIPlayer * m_MainPlayer;//��������һ��Main,����Ҫɾ��
};