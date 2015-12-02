#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include "Constantes.h"
#include "flib.h"
#include "flib_vec2.h"

class TPlayer;

class TCard
{
protected:

	//TGfxImage * m_pImageCenter;
	//TGfxTexture * m_pTextureCenter;
	ETypeCard m_eType;
	int m_iDurability;
	int m_iDamages;
	int m_iName;
	int m_iCost;
	int m_iCombo;
	bool m_bUsed;

public:
	virtual void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const = 0;
	bool Movement();

	void Initialize();
	bool ActivePosition(const int iPosition) const;
	bool PassivePosition() const;
	bool SelectedPosition() const;
	void DrawCard(const TGfxVec2 tPosition)const;
	bool Collision(const TGfxVec2 tPosition) const;
	void Desactivate();
	void Clean();


	ETypeCard GetType() const;
	int GetName() const;
	int GetCost() const;
	int GetCombo() const;
	bool GetState() const;

	virtual TCard * Clone() const = 0;

	TGfxSprite * m_pCardSprite = nullptr;
	//TGfxSprite * m_pCardSpriteCenter;

	const char * m_pEffect = nullptr;
	const char * m_pName = nullptr;
	

	TCard();
	~TCard();
};






#endif

