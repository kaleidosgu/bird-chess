#include "UICard.h"
#include "EntityFactory.h"

extern hgeFont				*fnt;
UICard::UICard(int _id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty):m_bVisable(true),m_bEnable(true),sprBird(NULL),sprInstruction(NULL),sprReady(NULL),sprWaiting(NULL)
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
	text = new NONOUIText(10,tx,ty,20,10,fnt);
	text->SetMode(HGETEXT_CENTER);
};

UICard::~UICard()
{
	if(sprUp) delete sprUp;
	if(sprDown) delete sprDown;
	if(text) delete text;
};

void UICard::SetCardInfo(int nType,int nInstruction)
{
	//changetex
	//SetText
	delete sprBird,sprBird = NULL;
	delete sprInstruction;sprInstruction = NULL;
	char temp[250];
	switch (nType)//临时测试用
	{
	case 0:
		sprintf_s(temp,"Red %d",nInstruction);
		sprBird = EntityFactory::Instance().CreateSprite("MonsterBase");
		break;
	case 1:
		sprintf_s(temp,"Green %d",nInstruction);
		sprBird = EntityFactory::Instance().CreateSprite("RushAngryBird");
		break;
	case 2:
		sprintf_s(temp,"Yellow %d",nInstruction);
		sprBird =  EntityFactory::Instance().CreateSprite("SmartAngryBird");
		break;
	case 3:
		sprintf_s(temp,"Black %d",nInstruction);
		sprBird =  EntityFactory::Instance().CreateSprite("StrongAngryBird");
		break;
	case 4:
		sprintf_s(temp,"Pig %d",nInstruction);
		sprBird =  EntityFactory::Instance().CreateSprite("HeroPig");
		break;
	case 5:
		sprintf_s(temp,"multicolored %d",nInstruction);
		sprBird =  EntityFactory::Instance().CreateSprite("QueenKissItem");
		break;
	default:
		sprintf_s(temp,"Error %d",nType);
		break;
	}

	switch (nInstruction)
	{
	case -2:
		sprInstruction = EntityFactory::Instance().CreateUISprite("BirdBackBack");
		break;
	case -1:
		sprInstruction = EntityFactory::Instance().CreateUISprite("BirdBack");
		break;
	case 1:
		sprInstruction =  EntityFactory::Instance().CreateUISprite("BirdGo");
		break;
	case 2:
		sprInstruction =  EntityFactory::Instance().CreateUISprite("BirdGoGo");
		break;
	default:
		break;
	}

	//sprintf(temp,"%d  %d",nType,nInstruction);

	sprBird->SetHotSpot(sprBird->GetWidth()/2,sprBird->GetHeight()/2);
	text->SetText(temp);
	m_nCardType = nType;
	m_nInstruction = nInstruction;
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
	if(m_bVisable)
	{
		if(bPressed) sprDown->Render(rect.x1, rect.y1);
		else sprUp->Render(rect.x1, rect.y1);
		if(sprBird) sprBird->Render(rect.x1+sprUp->GetWidth()/2,rect.y1+sprUp->GetHeight()/2);
		if(sprInstruction) sprInstruction->Render(rect.x1+sprUp->GetWidth()/2+13,rect.y1+sprUp->GetHeight()/2+50-10);
		//text->Render(rect.x1+sprUp->GetWidth()/2, rect.y1);//可以不画了
	}
}


void UICard::Render(float x,float y)
{
	if(m_bVisable)
	{
		if(bPressed) sprDown->Render( x,y);
		else sprUp->Render( x,y);
		if(sprBird) sprBird->Render(x+sprUp->GetWidth()/2,y+sprUp->GetHeight()/2);
		if(sprInstruction) sprInstruction->Render(x+sprUp->GetWidth()/2+13,y+sprUp->GetHeight()/2+50-10);
		//text->Render(rect.x1+sprUp->GetWidth()/2, y1);//可以不画了
	}
}
bool UICard::MouseLButton(bool bDown)
{
	if(m_bEnable && m_bVisable)
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
	return false;
}

void UICard::SetCardState(bool bReady)
{
	if(!sprReady && !sprWaiting )
	{
		sprReady = EntityFactory::Instance().CreateUISprite("CardBGReady");
		sprWaiting = EntityFactory::Instance().CreateUISprite("CardBGWaiting");
	}

	if(sprReady && sprWaiting )
	{
		if(bReady)
			sprUp = sprReady;
		else
			sprUp = sprWaiting;
	}
}