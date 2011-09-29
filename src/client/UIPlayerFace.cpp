#include "UIPlayerFace.h"
#include "NonoHGECtrlEx.h"
#include "EntityFactory.h"


extern hgeFont* fnt;
extern HGE* hge;

const int nFaceTypefix_X = 35;
const int nFaceTypefix_Y = 13;


UIPlayerFace::UIPlayerFace(const std::string& sName)
{
	m_Name = new NONOUIText(100,154,14,100,100,fnt);
	m_Name->SetText(sName.c_str());
	m_Name->SetMode(HGETEXT_CENTER);

	char Char[256];
	sprintf_s(Char,"PlayerFace%d",-1);
	//sFaceName
	m_FaceType =EntityFactory::Instance().CreateSprite(Char);
	m_FaceBg = EntityFactory::Instance().CreateUISprite("PlayerFaceBG");
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

const char* UIPlayerFace::GetName()
{
	return m_Name->GetText();
	//return str;
}
void UIPlayerFace::Render(float m_x,float m_y,float fSacle)
{
	//m_FaceBg->Render(m_x,m_y);
	//m_FaceType->Render(m_x+nFaceTypefix_X,m_y+nFaceTypefix_Y);
	m_FaceBg->RenderEx(m_x,m_y,0,fSacle,fSacle);
	m_FaceType->RenderEx(m_x+nFaceTypefix_X,m_y+nFaceTypefix_Y,0,fSacle,fSacle);
	m_Name->Render(m_x+nFaceTypefix_X,m_y);
}

void UIPlayerFace::SetBirdID(int nFaceID)
{
	char cName[256];
	sprintf_s(cName,"PlayerFace%d",nFaceID);
	//sFaceName
	delete m_FaceType;
	m_FaceType =EntityFactory::Instance().CreateSprite(cName);
}

void UIPlayerFace::SetScale(float fScale)
{

}

//
//hgeSprite* m_Facebg; //±³¾°
//hgeGUIText* m_Name;
//hgeGUIText* m_Score;