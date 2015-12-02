#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "flib.h"
#include "flib_vec2.h"

class TButton
{
	TGfxVec2 m_tPosition;

	TGfxTexture * m_pTexture = nullptr;
	TGfxSprite * m_pSprite = nullptr;

	int m_iSizeX;
	int m_iSizeY;



public:

	void Initialize(TGfxImage * pImage, TGfxVec2 tPosition, int iSizeX, int iSizeY);
	void Draw();
	bool Collision();


	TButton();
	~TButton();
};

#endif

