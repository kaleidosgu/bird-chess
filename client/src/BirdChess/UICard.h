/************************************************************************/
/*			玩家的卡牌,这个可以考虑放到CtrlEx中去                       */
/************************************************************************/

#pragma  once
#include "NonoHGECtrlEx.h"


class UICard :public hgeGUIObject
{
public:
	UICard(int _id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty);
	~UICard();

	void			SetMode(bool _bTrigger) { bTrigger=_bTrigger; }
	void			SetState(bool _bPressed) { bPressed=_bPressed; }
	bool			GetState() const { return bPressed; }
	virtual void	Render();
	virtual bool	MouseLButton(bool bDown);

	void SetCardType(int nType);
	int GetCardType(){return m_nCardType;};
	void SetTexture(float w, float h, HTEXTURE tex, float tx, float ty);
	
private:
	int m_nCardType;

private:
	bool			bTrigger;
	bool			bPressed;
	bool			bOldState;
	hgeSprite		*sprUp, *sprDown;
};