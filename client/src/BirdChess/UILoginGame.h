////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		��¼����,��������һ��������ͼƬ�Ŀؼ�,������ʱ����λ��,�����ʺŻس�����������,��������س�ֱ��������½
///		������ͺ󴫲�����һ��������һ������,��¼�ɹ�������
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#pragma  once
class hgeGUI;
class NONOhgeGUIText;
class hgeSprite;
class hgeFont;


class CUILoginGame
{
public:
	CUILoginGame(int x,int y,hgeFont* fnt);
	~CUILoginGame();
	void SendLog2Server(const std::string& name ,const std::string& Account);//�����ʱ������,���ܻ��õ�һ��ȫ�ֵ�ɶɶ��
	void Render();
	void Update(float dt);
private:
	enum{EIDT_ACCOUNTNAME  = 1 ,EDIT_PASSWORD};
	void _CreateBG(int x,int y);
	void _CreateEdit(int x,int y,hgeFont*fnt);
	void _OnEnter(int nCtlID);

private:
	hgeGUI* m_LogUI;
	NONOhgeGUIText* m_AccountName;
	NONOhgeGUIText* m_Password;
	hgeSprite* m_bg; //����
	int m_x;
	int m_y;
};
