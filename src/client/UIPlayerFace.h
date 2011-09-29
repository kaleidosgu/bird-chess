/************************************************************************/
/*                                                                      */
/*		��ҵ�ͷ��,ͼƬ,����,ս��,��ǰ��                                */
/*                                                                      */
/*                                                                      */
/************************************************************************/
#pragma once

#include <string>

class hgeSprite;
class NONOUIText;

class UIPlayerFace
{
public:
	UIPlayerFace(const std::string& sName);
	~UIPlayerFace();
	void Render(float x,float y,float fSacle);

	void SetBirdID(int nFaceID);
	void SetScale(float fScale);
	void SetName(const std::string& sName);
	const char* GetName();

private:
	hgeSprite* m_FaceType; //����
	hgeSprite* m_FaceBg; //����
	NONOUIText* m_Name;
	//hgeGUIText* m_Score;//ս����ʱ������
};