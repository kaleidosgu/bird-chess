/************************************************************************/
/*                                                                      */
/*		��ҵ�ͷ��,ͼƬ,����,ս��,��ǰ��                                */
/*                                                                      */
/*                                                                      */
/************************************************************************/
#pragma once

#include <string>

class hgeSprite;
class hgeGUIText;

class UIPlayerFace
{
public:
	UIPlayerFace(const std::string& sName);
	~UIPlayerFace();
	void Render();
	void SetBirdID(int nFaceID);
	void SetScale(float fScale);
	void SetName(const std::string& sName);
private:
	hgeSprite* m_Facebg; //����
	hgeGUIText* m_Name;
	//hgeGUIText* m_Score;//ս����ʱ������
};