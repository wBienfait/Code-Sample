#include "Sprites.h"
#include "Player.h"
#include "Button.h"
#include "GameState.h"

extern TPlayer * g_tPlayer[MAX_PLAYER];
extern EState g_eState;

extern TButton g_tBack;

TSprites::TSprites()
{



}


TSprites::~TSprites()
{
}

void TSprites::Initialize()
{
	if (m_pDeadSprite == nullptr)
	{
		m_pDeadSprite = GfxTextSpriteCreate();
		m_pEffectSprite = GfxTextSpriteCreate();
		m_pDeckList = GfxTextSpriteCreate();

		m_pBackImage = GfxImageLoad("BackButton.tga");
		m_pEndImage = GfxImageLoad("EndButton.tga");
		m_pAttackImage = GfxImageLoad("AttackButton.tga");
		m_pStartImage = GfxImageLoad("StartButton.tga");
		m_pBreakImage = GfxImageLoad("BreakButton.tga");
		m_pContinueImage = GfxImageLoad("ContinueButton.tga");
		m_pRestartImage = GfxImageLoad("RestartButton.tga");
		m_pDeckListImage = GfxImageLoad("DeckListButton.tga");
		m_pChangeImage = GfxImageLoad("ChangeButton.tga");


		


		m_pCardImage = GfxImageLoad("cartes_tileset.tga");
		m_pCardTexture = GfxTextureCreate(m_pCardImage);

		m_pSymImage = GfxImageLoad("Player_Sym.tga");
		m_pSymTexture = GfxTextureCreate(m_pSymImage);

		m_pWpnImage = GfxImageLoad("Player_Arm.tga");
		m_pWpnTexture = GfxTextureCreate(m_pWpnImage);

		m_pEqImage = GfxImageLoad("Player_Eq.tga");
		m_pEqTexture = GfxTextureCreate(m_pEqImage);

		m_pPersoImage = GfxImageLoad("PlayerBackground.tga");
		m_pPersoTexture = GfxTextureCreate(m_pPersoImage);
		
		m_pMainMenuImg = GfxImageLoad("MainMenu.tga");
		m_pMainMenuTex = GfxTextureCreate(m_pMainMenuImg);
		m_pMainMenuSprt = GfxSpriteCreate(m_pMainMenuTex);
		m_pBackCardImg = GfxImageLoad("BackCard.tga");
		m_pBackCardTex = GfxTextureCreate(m_pBackCardImg);
		m_pBackCard = GfxSpriteCreate(m_pBackCardTex);



	}



	GfxSpriteSetPosition(m_pEffectSprite, float(SELECTED_POS_X + CARD_SIZE_X + INTERCARD_X), float(SELECTED_POS_Y + INTERCARD_X));


}

void TSprites::Update()
{

	if (g_eState == State_Dead)
	{
		GfxTextSpritePrintf(m_pDeadSprite, "Peace and love, nobody's dead");
		if (g_tPlayer[PLAYER_2]->GetLifePoints() <= 0)
		{
			GfxTextSpritePrintf(m_pDeadSprite, "you win");
		}
		if (g_tPlayer[PLAYER_1]->GetLifePoints() <= 0)
		{
			GfxTextSpritePrintf(m_pDeadSprite, "you lose");
		}
	}
	

}

void TSprites::Render()
{
	
	if (g_eState == State_Dead)
	{
		GfxSpriteRender(m_pDeadSprite);
	}

	else
	{

		if (g_tPlayer[PLAYER_1]->m_tGameState == GAME_Neutral || g_tPlayer[PLAYER_1]->m_tGameState == GAME_Locked || g_tPlayer[PLAYER_1]->m_tGameState == GAME_End || g_tPlayer[PLAYER_1]->m_tGameState == GAME_Action || g_tPlayer[PLAYER_1]->m_tGameState == GAME_Initialize)
		{
			GfxSpriteRender(g_tPlayer[PLAYER_1]->m_pLifeSprite);
			GfxSpriteRender(g_tPlayer[PLAYER_2]->m_pLifeSprite);
			GfxSpriteRender(g_tPlayer[PLAYER_1]->m_pEnergySprite);
			GfxSpriteRender(g_tPlayer[PLAYER_2]->m_pEnergySprite);
			GfxSpriteRender(g_tPlayer[PLAYER_1]->m_pSprite);
			GfxSpriteRender(g_tPlayer[PLAYER_2]->m_pSprite);

			g_tPlayer[PLAYER_1]->Render();
			g_tPlayer[PLAYER_2]->Render();

			for (int i = 0; i < MAX_HAND; i++)
			{
				
				if (g_tPlayer[PLAYER_1]->m_pCard[i]->GetState() == false)
				{
					GfxSpriteRender(g_tPlayer[PLAYER_1]->m_pCard[i]->m_pCardSprite);
				}
			}


		}
		else
		{
			g_tBack.Draw();
			for (int i = 0; i < MAX_HAND; i++)
			{
				if (i == g_tPlayer[PLAYER_1]->GetSelected() || g_tPlayer[PLAYER_1]->m_tGameState == GAME_Neutral)
				{
					if (g_tPlayer[PLAYER_1]->m_pCard[i]->GetState() == false)
					{
						//GfxSpriteRender(m_pNameCard[i]);
						GfxSpriteRender(g_tPlayer[PLAYER_1]->m_pCard[i]->m_pCardSprite);
						if (g_tPlayer[PLAYER_1]->m_tGameState == GAME_CardSelected)
						{
							GfxSpriteRender(m_pEffectSprite);
						}
					}

				}

			}
			
			//GfxSpriteRender(m_pInformations[PLAYER_2]);
		}
	}

}
