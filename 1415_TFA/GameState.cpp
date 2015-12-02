#include "GameState.h"
#include "Button.h"
#include "Sprites.h"
#include "Player.h"
#include "LuaFunctions.h"
#include "TXML_Root.h"
#include "CardList.h"
#include "Animation.h"

TButton g_tStart;
TButton g_tBreak;
TButton g_tContinue;
TButton g_tRestart;
TButton g_tDeckList;

TXML_Root * g_pDeckListRoot;

TLuaFunctions g_tLuaFunctions;

TCard * g_pCardSample[MAX_DIFF_CARDS];


TPlayer * g_tPlayer[MAX_PLAYER];
TSprites g_tSprites;
EState g_eState;

TGameState::TGameState()
{
}


TGameState::~TGameState()
{
}

void TGameState::Initialize()
{
	g_eState = State_MainMenu;

	if (g_pDeckListRoot == nullptr)
	{
		g_pDeckListRoot = new TXML_Root;
	}

	if (g_pDeckListRoot->m_pRootElementList->GetFirstItem() == nullptr)
	{
		g_pDeckListRoot->ParseXml("DeckList.xml");
	}


	g_pCardSample[0] = new TZombie();
	g_pCardSample[1] = new TPunch();
	g_pCardSample[2] = new TSneeze();
	g_pCardSample[3] = new TDiarium();
	g_pCardSample[4] = new TDiarhea();

	g_pCardSample[5] = new TVomiting();
	g_pCardSample[6] = new THealthStick();
	g_pCardSample[7] = new TArm();
	g_pCardSample[8] = new TTransix();
	g_pCardSample[9] = new TBerserk();

	g_tPlayer[PLAYER_1] = new TPlayer;
	g_tPlayer[PLAYER_2] = new TPlayer;





	g_tLuaFunctions.Initialize();



	g_tSprites.Initialize();

	g_tStart.Initialize(g_tSprites.m_pStartImage, TGfxVec2(float(START_POS_X), float(START_POS_Y)), START_SIZE_X, START_SIZE_Y);
	g_tBreak.Initialize(g_tSprites.m_pBreakImage, TGfxVec2(float(BREAK_POS_X), float(BREAK_POS_Y)), BREAK_SIZE_X, BREAK_SIZE_Y);
	g_tContinue.Initialize(g_tSprites.m_pContinueImage, TGfxVec2(float(CONTINUE_POS_X), float(CONTINUE_POS_Y)), CONTINUE_SIZE_X, CONTINUE_SIZE_Y);
	g_tRestart.Initialize(g_tSprites.m_pRestartImage, TGfxVec2(float(RESTART_POS_X), float(RESTART_POS_Y)), RESTART_SIZE_X, RESTART_SIZE_Y);
	g_tDeckList.Initialize(g_tSprites.m_pDeckListImage, TGfxVec2(float(DECK_LIST_POS_X), float(DECK_LIST_POS_Y)), DECK_LIST_SIZE_X, DECK_LIST_SIZE_Y);

}

void TGameState::Update()
{
	g_tSprites.Update();


	switch (g_eState)
	{
		case State_MainMenu:
		{
			if (GfxInputIsJustPressed(EGfxInputID_MouseLeft))
			{
				if (g_tStart.Collision())
				{
					g_tPlayer[PLAYER_1]->Initialize(TGfxVec2(0, 0), PLAYER_1);
					g_tPlayer[PLAYER_1]->m_pEnemy = g_tPlayer[PLAYER_2];
					g_tPlayer[PLAYER_2]->Initialize(TGfxVec2(float(SIZE_X - PLAYER_SIZE_X), 0), PLAYER_2);
					g_tPlayer[PLAYER_2]->m_pEnemy = g_tPlayer[PLAYER_1];
					g_tPlayer[PLAYER_2]->m_tGameState = GAME_Locked;
					g_eState = State_Player1;
					
				}
				if (g_tDeckList.Collision())
				{
					g_eState = State_DeckMenu;
				}
			}
			break;
		}
		case State_Player1:
		{
			g_tPlayer[PLAYER_1]->ChangeState();
			g_tPlayer[PLAYER_2]->ChangeState();
			if (g_tPlayer[PLAYER_2]->GetLifePoints() > 0 && g_tPlayer[PLAYER_1]->GetLifePoints() > 0)
			{
				GfxSpriteSetScale(g_tPlayer[PLAYER_2]->m_pLifeSprite, float(g_tPlayer[PLAYER_2]->GetLifePoints()), 1.0f);
				GfxSpriteSetScale(g_tPlayer[PLAYER_1]->m_pLifeSprite, float(g_tPlayer[PLAYER_1]->GetLifePoints()), 1.0f);
			}
			GfxSpriteSetScale(g_tPlayer[PLAYER_2]->m_pEnergySprite, float(g_tPlayer[PLAYER_2]->GetEnergy()), 1.0f);
			GfxSpriteSetScale(g_tPlayer[PLAYER_1]->m_pEnergySprite, float(g_tPlayer[PLAYER_1]->GetEnergy()), 1.0f);
			GfxTextSpritePrintf(g_tSprites.m_pEffectSprite, "%s", g_tPlayer[PLAYER_1]->m_pCard[g_tPlayer[PLAYER_1]->GetSelected()]->m_pEffect);
			g_tPlayer[PLAYER_1]->Update();
			if (GfxInputIsJustPressed(EGfxInputID_MouseLeft))
			{
				if (g_tBreak.Collision())
				{
					g_eState = State_BreakMenu;
				}
			}
			g_tPlayer[PLAYER_1]->m_pPersoAnim->Animate();
			g_tPlayer[PLAYER_2]->m_pPersoAnim->Animate();

			break;
		}


		case State_Player2:
		{
			g_tPlayer[PLAYER_1]->ChangeState();
			g_tPlayer[PLAYER_2]->ChangeState();
			if (g_tPlayer[PLAYER_2]->GetLifePoints() > 0 && g_tPlayer[PLAYER_1]->GetLifePoints() > 0)
			{
				GfxSpriteSetScale(g_tPlayer[PLAYER_2]->m_pLifeSprite, float(g_tPlayer[PLAYER_2]->GetLifePoints()), 1.0f);
				GfxSpriteSetScale(g_tPlayer[PLAYER_1]->m_pLifeSprite, float(g_tPlayer[PLAYER_1]->GetLifePoints()), 1.0f);
			}
			GfxSpriteSetScale(g_tPlayer[PLAYER_2]->m_pEnergySprite, float(g_tPlayer[PLAYER_2]->GetEnergy()), 1.0f);
			GfxSpriteSetScale(g_tPlayer[PLAYER_1]->m_pEnergySprite, float(g_tPlayer[PLAYER_1]->GetEnergy()), 1.0f);
			g_tPlayer[PLAYER_2]->Update();

			g_tPlayer[PLAYER_1]->m_pPersoAnim->Animate();
			g_tPlayer[PLAYER_2]->m_pPersoAnim->Animate();


			if (g_tPlayer[PLAYER_2]->GetLifePoints() <= 0 && g_tPlayer[PLAYER_1]->GetLifePoints() <= 0)
			{
				g_eState = State_Dead;
			}

			break;
		}
		case State_BreakMenu:
		{
			
			if (GfxInputIsJustPressed(EGfxInputID_MouseLeft))
			{
				if (g_tContinue.Collision())
				{
					g_eState = State_Player1;
				}
				if (g_tRestart.Collision())
				{
					g_eState = State_Dead;
				}
			}
			break;
		}
		case State_DeckMenu:
		{
			g_tPlayer[PLAYER_1]->UpdateCardList();

			if (GfxInputIsJustPressed(EGfxInputID_MouseLeft))
			{
				if (g_tRestart.Collision())
				{
					g_eState = State_MainMenu;
				}
			}
			break;
		}
		case State_Dead:
		{		   
			if (GfxInputIsJustPressed(EGfxInputID_MouseLeft))
			{
				g_tPlayer[PLAYER_1]->Clean();
				g_tPlayer[PLAYER_2]->Clean();
				delete g_tPlayer[PLAYER_1];
				delete g_tPlayer[PLAYER_2];
				for(int i = 0; i < MAX_DIFF_CARDS; i++)
				{
					delete g_pCardSample[i];
				}
				Initialize();


				g_eState = State_MainMenu;

			}
			break;
		}
		default:
			break;
	}
}

void TGameState::Render() const
{
	if (g_eState == State_MainMenu)
	{
		GfxSpriteRender(g_tSprites.m_pMainMenuSprt);
		g_tStart.Draw();
		g_tDeckList.Draw();
	}
	if (g_eState == State_Dead)
	{
		GfxSpriteRender(g_tSprites.m_pDeadSprite);
	}
	if (g_eState == State_BreakMenu)
	{
		g_tContinue.Draw();
		g_tRestart.Draw();
	}

	if (g_eState == State_DeckMenu)
	{
		g_tRestart.Draw();
		GfxSpriteRender(g_tSprites.m_pDeckList);
	}

	if (g_eState == State_Player1 || g_eState == State_Player2)
	{
		g_tSprites.Render();
		if (g_eState == State_Player1)
		{
			g_tBreak.Draw();
		}
		if (g_eState == State_Player2)
		{
			g_tBreak.Draw();
			if (g_tPlayer[PLAYER_2]->GetSelected() != MAX_HAND)
			{
				g_tPlayer[PLAYER_2]->m_pCard[g_tPlayer[PLAYER_2]->GetSelected()]->SelectedPosition();
				GfxSpriteRender(g_tPlayer[PLAYER_2]->m_pCard[g_tPlayer[PLAYER_2]->GetSelected()]->m_pCardSprite);
			}
			
		}
		
	}
	

}
