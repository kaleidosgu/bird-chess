#include "UICard.h"

UICard::UICard(int _id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty)
{
	id=_id;
	bStatic=false;
	bVisible=true;
	bEnabled=true;
	rect.Set(x, y, x+w, y+h);

	bPressed=false;
	bTrigger=false;

	sprUp = new hgeSprite(tex, tx, ty, w, h);
	sprDown = new hgeSprite(tex, tx+w, ty, w, h);
};

UICard::~UICard()
{
	if(sprUp) delete sprUp;
	if(sprDown) delete sprDown;
};

void UICard::SetCardType(int nType)
{
	//changetex
	//SetText
	m_nCardType = nType;
};


void UICard::SetTexture(float w, float h, HTEXTURE tex, float tx, float ty)
{
	delete sprUp;
	delete sprDown;
	sprUp = new hgeSprite(tex, tx, ty, w, h);
	sprDown = new hgeSprite(tex, tx+w, ty, w, h);
};


void UICard::Render()
{
	if(bPressed) sprDown->Render(rect.x1, rect.y1);
	else sprUp->Render(rect.x1, rect.y1);
}

bool UICard::MouseLButton(bool bDown)
{
	if(bDown)
	{
		bOldState=bPressed; bPressed=true;
		return false;
	}
	else
	{
		if(bTrigger) bPressed=!bOldState;
		else bPressed=false;
		return true; 
	}
}