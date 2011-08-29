///************************************************************************/
///* 打鸟鸟的游戏房间,包含所有信息,接收消息并且分发                       */
///************************************************************************/
//

#pragma once;

#include <vector>
#include <map>
#include <list>

class CCloudEntity;
class CBirdEntity;
class UIPlayerManage;
class UIGround
{
public:
	UIGround();
	~UIGround();
	void Render();
	void Update();
	void Response();
private:
	std::vector<CBirdEntity*> m_vecBird;
	std::vector<CCloudEntity*> m_vecCloud;
	std::map<int,std::list<CBirdEntity*> > m_mapCloundState;
	UIPlayerManage* m_PlayerManage;
	
};