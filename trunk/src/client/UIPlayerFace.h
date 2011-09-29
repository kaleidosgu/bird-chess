/************************************************************************/
/*                                                                      */
/*		玩家的头像,图片,名字,战绩,当前牌                                */
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
	hgeSprite* m_FaceType; //类型
	hgeSprite* m_FaceBg; //背景
	NONOUIText* m_Name;
	//hgeGUIText* m_Score;//战绩暂时不开放
};