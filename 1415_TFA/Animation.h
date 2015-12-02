#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include "flib.h"
#include "flib_vec2.h"

class TAnimation
{

	TGfxVec2 m_tPosition;

	TGfxImage * m_pImage = nullptr;
	TGfxTexture * m_pTexture = nullptr;

	int m_iTileX;
	int m_iTileY;
	int m_iSizeX;
	int m_iSizeY;
	int m_iSpeed;
	int m_iAnim;
	int m_iSpeedCount;
	int m_iAnimPosition;

public:

	TGfxSprite * m_pSprite = nullptr;

	void Initialize(char * pName);
	void Update(int iAnim);
	void Render();

	void Clean();

	void Animate();

	TAnimation(int iTileX, int iTileY, int iAnimSpeed, int iAnim, TGfxVec2 tposition);
	~TAnimation();
};

#endif

