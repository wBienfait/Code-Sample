#include "Button.h"


TButton::TButton()
{
	m_iSizeX = 0;
	m_iSizeY = 0;
}


TButton::~TButton()
{
}

void TButton::Initialize(TGfxImage * pImage, TGfxVec2 tPosition, int iSizeX, int iSizeY)
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = GfxTextureCreate(pImage);
		m_pSprite = GfxSpriteCreate(m_pTexture);
	}

	m_tPosition = tPosition;
	m_iSizeX = iSizeX;
	m_iSizeY = iSizeY;

	GfxSpriteSetPosition(m_pSprite, tPosition.x, tPosition.y);
}

bool TButton::Collision()
{
	const int iX = GfxGetCurrentMouseX();
	const int iY = GfxGetCurrentMouseY();

	if (iX > m_tPosition.x &&
		iX < m_tPosition.x + m_iSizeX &&
		iY > m_tPosition.y &&
		iY < m_tPosition.y + m_iSizeY)
	{
		return true;
	}

	return false;
}

void TButton::Draw()
{
	GfxSpriteRender(m_pSprite);
}