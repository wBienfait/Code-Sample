#ifndef SPRITES_H_INCLUDED
#define SPRITES_H_INCLUDED

#include "flib.h"
#include "Constantes.h"

class TSprites
{
public:
	TGfxImage * m_pBackImage = nullptr;
	TGfxImage * m_pEndImage = nullptr;
	TGfxImage * m_pAttackImage = nullptr;
	TGfxImage * m_pStartImage = nullptr;
	TGfxImage * m_pBreakImage = nullptr;
	TGfxImage * m_pContinueImage = nullptr;
	TGfxImage * m_pRestartImage = nullptr;
	TGfxImage * m_pDeckListImage = nullptr;
	TGfxImage * m_pChangeImage = nullptr;
	TGfxImage * m_pBackCardImg = nullptr;
	TGfxImage * m_pCardImage = nullptr;
	TGfxImage * m_pWpnImage = nullptr;
	TGfxImage * m_pEqImage = nullptr;
	TGfxImage * m_pSymImage = nullptr;
	TGfxImage * m_pPersoImage = nullptr;
	TGfxImage * m_pMainMenuImg = nullptr;

	TGfxTexture * m_pCardTexture = nullptr;
	TGfxTexture * m_pBackCardTex = nullptr;
	TGfxTexture * m_pWpnTexture = nullptr;
	TGfxTexture * m_pEqTexture = nullptr;
	TGfxTexture * m_pSymTexture = nullptr;
	TGfxTexture * m_pPersoTexture = nullptr;
	TGfxTexture * m_pMainMenuTex = nullptr;

	TGfxSprite * m_pActionFeedBack = nullptr;
	TGfxSprite * m_pDeckList = nullptr;
	TGfxSprite * m_pEffectSprite = nullptr;
	TGfxSprite * m_pDeadSprite = nullptr;
	TGfxSprite * m_pBackCard = nullptr;
	TGfxSprite * m_pMainMenuSprt = nullptr;

	TSprites();
	~TSprites();

	void Initialize();
	void Update();
	void Render();

};

#endif
