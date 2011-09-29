/************************************************************************/
/*					简单提示单件,showmessage用,在界面显示3秒后消失        */
/************************************************************************/
#pragma once
//#include 
#include <string>

class NONOUIText;
class CShowMessage
{
private:
	CShowMessage();
public:
	~CShowMessage();
	static CShowMessage& Instance();
	void SetShowMesage(const std::string& str,float fTime = 3.0f);
	void Render();
	void Update(float dt);
private:
	NONOUIText* m_ShowMesage;
	static CShowMessage* m_pSelf;
	float m_fTime;
	bool m_bVisable;
};