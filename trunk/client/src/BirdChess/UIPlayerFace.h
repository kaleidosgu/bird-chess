/************************************************************************/
/*                                                                      */
/*		玩家的头像,图片,名字,战绩,当前牌                                */
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
	hgeSprite* m_Facebg; //背景
	hgeGUIText* m_Name;
	//hgeGUIText* m_Score;//战绩暂时不开放
};