////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		登录界面,期望的是一个带背景图片的控件,可以随时调节位置,输入帐号回车就跳到密码,输入密码回车直接跳到登陆
///		点击发送后传参数给一个网络层的一个函数,登录成功后被隐藏
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#pragma  once
class hgeGUI;
class NONOhgeGUIText;
class NONOhgePassWordGUIText;

class hgeSprite;
class hgeFont;
class UICard;

class CUILoginGame
{
public:
	CUILoginGame(int x,int y,hgeFont* fnt);
	~CUILoginGame();
	void SendLog2Server(const std::string& name ,const std::string& Account);
	void Render();
	void Update(float dt);
	void SetEnable(bool bEnable);
private:
	enum{EIDT_ACCOUNTNAME  = 1 ,EDIT_PASSWORD,BUTTON_ENTERGAME};
	void _CreateBG(int x,int y);
	void _CreateEdit(int x,int y,hgeFont*fnt);
	void _OnEnter(int nCtlID);

private:
	hgeGUI* m_LogUI;
	NONOhgeGUIText* m_AccountName;
	NONOhgePassWordGUIText* m_Password;
	UICard* m_EnterButton;
	hgeSprite* m_bg; //背景
	int m_x;
	int m_y;
	bool m_bEnable;
};
