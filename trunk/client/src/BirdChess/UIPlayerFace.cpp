#include "UIPlayerFace.h"
#include "NonoHGECtrlEx.h"

extern hgeFont* fnt;
extern HGE* hge;
UIPlayerFace::UIPlayerFace(const std::string& sName)
{
	m_Name = new hgeGUIText(100,100,100,100,100,fnt);
	m_Name->SetText(sName.c_str());

	HTEXTURE tex=hge->Texture_Load("res/Pic/UILoginBG.png");
	m_Facebg= new hgeSprite(tex,54,357,147,114);
	

}

UIPlayerFace::~UIPlayerFace()
{
}

void UIPlayerFace::SetName(const std::string& sName)
{
	if(m_Name)
	{
		m_Name->SetText(sName.c_str());
	}
}
void UIPlayerFace::Render()
{
	m_Name->Render();
}

void UIPlayerFace::SetBirdID(int nFaceID)
{
	
}

void UIPlayerFace::SetScale(float fScale)
{

}

//
//hgeSprite* m_Facebg; //±³¾°
//hgeGUIText* m_Name;
//hgeGUIText* m_Score;