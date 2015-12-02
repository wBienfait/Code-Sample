#include "Animation.h"


TAnimation::TAnimation(int iTileX, int iTileY, int iAnimSpeed, int iAnim, TGfxVec2 tPosition)
{
	m_iSizeX = 0;
	m_iSizeY = 0;
	m_iTileX = iTileX;
	m_iTileY = iTileY;
	m_iSpeed = iAnimSpeed;
	m_iAnim = iAnim;
	m_tPosition = tPosition;
	m_iSpeedCount = 0;
	m_iAnimPosition = 2 * iAnim;
}


TAnimation::~TAnimation()
{
}

void TAnimation::Initialize(char * pName)
{
	m_pImage = GfxImageLoad(pName);
	m_pTexture = GfxTextureCreate(m_pImage);
	m_pSprite = GfxSpriteCreate(m_pTexture);

	m_iSizeX = (GfxSpriteGetSizeX(m_pSprite) / m_iTileX);
	m_iSizeY = (GfxSpriteGetSizeY(m_pSprite) / m_iTileY);

	GfxSpriteSetCutout(m_pSprite, m_iAnimPosition * m_iSizeX, m_iAnim * m_iSizeY, m_iSizeX, m_iSizeY);
	GfxSpriteSetPosition(m_pSprite, m_tPosition.x, m_tPosition.y);
}

void TAnimation::Update(int iAnim)
{
	m_iAnim = iAnim;
}

void TAnimation::Render()
{
	GfxSpriteRender(m_pSprite);
}

void TAnimation::Animate()
{
	const int iAnimPosX = m_iAnimPosition * m_iSizeX;
	const int iAnimPosY = m_iAnim * m_iSizeY;

	GfxSpriteSetCutout(m_pSprite, iAnimPosX, iAnimPosY, m_iSizeX, m_iSizeY);
	m_iSpeedCount++;
	if (m_iSpeedCount >= m_iSpeed)
	{
		m_iAnimPosition++;
		m_iSpeedCount = 0;
	}
	if (m_iAnimPosition >= m_iTileX - 1)
	{
		m_iAnimPosition = 0;
	}

}

void TAnimation::Clean()
{
	if (m_pImage != nullptr)
	{
		GfxImageDestroy(m_pImage);
		GfxTextureDestroy(m_pTexture);
		GfxSpriteDestroy(m_pSprite);
	}
}
