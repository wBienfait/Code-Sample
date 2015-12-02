#include "Card.h"
#include "Player.h"
#include "Sprites.h"
#include "LuaFunctions.h"

extern TSprites g_tSprites;
extern TPlayer * g_tPlayer[MAX_PLAYER];
extern TGfxSprite * g_pBackCard;
extern TLuaFunctions g_tLuaFunctions;


TCard::TCard() :m_iDurability(0),m_iDamages(0),m_iName(0),m_iCost(0),m_iCombo(0),m_bUsed(0)
{
	m_eType = TYP_SYMPTOME;	
}
TCard::~TCard()
{
}

void TCard::Initialize()
{

	m_pCardSprite = GfxSpriteCreate(g_tSprites.m_pCardTexture);

	GfxSpriteSetCutout(m_pCardSprite, (m_iName - 1) * CARD_SIZE_X, m_eType * CARD_SIZE_Y, CARD_SIZE_X, CARD_SIZE_Y);
	GfxSpriteSetPivot(m_pCardSprite, float(CARD_SIZE_X / 2.0f), float(CARD_SIZE_Y / 2.0f));
}

bool TCard::Movement()
{
	return g_tLuaFunctions.Animation(m_pCardSprite);
}

void TCard::Desactivate()
{
	m_bUsed = true;
}

void TCard::Clean()
{
	if (m_pCardSprite != nullptr)
	{

		GfxSpriteDestroy(m_pCardSprite);
		m_pCardSprite = nullptr;

	}
}

bool TCard::GetState() const
{
	return m_bUsed;
}


bool TCard::ActivePosition(const int iPosition) const
{
	if (m_bUsed == false)
	{

		const TGfxVec2 tPosition = TGfxVec2(float(iPosition * CARD_SIZE_X + (iPosition + 1) * INTERCARD_X), float(INTERCARD_Y));

		DrawCard(tPosition);
		if (Collision(tPosition))
		{
			return true;
		}
	}
	else
	{
		return false;
	}
	return false;
}

bool TCard::PassivePosition() const
{
	const TGfxVec2 tPosition = TGfxVec2(float(PASSIVE_POS), float(BORDER));

	GfxSpriteSetPosition(g_tSprites.m_pBackCard, tPosition.x, tPosition.y);

	if (Collision(tPosition))
	{
		return true;
	}
	return false;
}

bool TCard::SelectedPosition() const
{
	if (m_bUsed == false)
	{
		const TGfxVec2 tPosition = TGfxVec2(float(SELECTED_POS_X), float(SELECTED_POS_Y));

		DrawCard(tPosition);
		if (Collision(tPosition))
		{
			return true;
		}
	}
	return false;
}

bool TCard::Collision(const TGfxVec2 tPosition) const
{
	if (GfxInputIsJustPressed(EGfxInputID_MouseLeft))
	{
		const int iX = GfxGetCurrentMouseX();
		const int iY = GfxGetCurrentMouseY();

		if (iX > tPosition.x &&
			iX < tPosition.x + CARD_SIZE_X &&
			iY > tPosition.y &&
			iY < tPosition.y + CARD_SIZE_Y)
		{
			return true;
		}
	}

	return false;
}

void TCard::DrawCard(const TGfxVec2 tPosition)const
{
	GfxSpriteSetPosition(m_pCardSprite, tPosition.x + CARD_SIZE_X/2.0f, tPosition.y + CARD_SIZE_Y/2.0f);

}

int TCard::GetName()const
{
	return m_iName;
}

ETypeCard TCard::GetType()const
{
	return m_eType;
}

int TCard::GetCost() const
{
	return m_iCost;
}

int TCard::GetCombo() const
{
	return m_iCombo;
}



