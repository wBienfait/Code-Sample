#ifndef MENU_OPTION_H_INCLUDED
#define MENU_OPTION_H_INCLUDED

struct TGfxSprite;

struct TMenuOption
{
	TGfxSprite * m_pTextSprite;
	char * m_pTextLabel;
	float m_fValue;
	float m_fMinValue;
	float m_fMaxValue;
	float m_fIncrement;
};

#endif
