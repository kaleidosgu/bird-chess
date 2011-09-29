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
	void	Render(float x,float y);
	virtual bool	MouseLButton(bool bDown);

	void SetCardInfo(int nType,int nInstruction);
	int GetCardType(){return m_nCardType;};
	int GetCardInstruction(){return m_nInstruction;};
	void SetTexture(float w, float h, HTEXTURE tex, float tx, float ty);

	void SetEnable(bool bEnable){m_bEnable = bEnable;};
	bool GetEnable(){return m_bEnable ;};

	void SetVisable(bool bVisable){m_bVisable = bVisable;};
	bool GetVisable(){return m_bVisable ;};

	bool GetPress(){ return  bPressed;};
	void SetCardState(bool bReady);

	
private:
	int m_nCardType;
	int m_nInstruction;
	bool m_bVisable;
	bool m_bEnable;

	NONOUIText* text;
private:
	bool			bTrigger;
	bool			bPressed;
	bool			bOldState;
	hgeSprite		*sprUp, *sprDown;
	hgeSprite		*sprReady,*sprWaiting;
	hgeSprite		*sprBird, *sprInstruction;
};