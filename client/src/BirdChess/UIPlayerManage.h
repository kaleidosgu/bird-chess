///************************************************************************/
///* 玩家的管理器,管理玩家的所有信息,处理网络传送过来的消息				*/
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
	//void SetPlayerInfo();  //设置这里面的人物信息,更新主角信息,更新角色列表,更新头像
	//UIPlayerManage& Instance();
	
	
private:
	std::vector<UIPlayer*>  m_VecPlayer;
	MainUIPlayer * m_MainPlayer;//单独存了一个Main,不需要删除
};