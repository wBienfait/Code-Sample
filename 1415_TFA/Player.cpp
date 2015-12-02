#include "Player.h"
#include"CardList.h"
#include "Sprites.h"
#include "Button.h"
#include "GameState.h"
#include "LuaFunctions.h"
#include "TXML_Root.h"
#include "Animation.h"
#include <math.h>

TButton g_tBack;
TButton g_tAttack;
TButton g_tEnd;
TButton g_tChangeCards;


extern TCard * g_pCardSample[MAX_DIFF_CARDS];
extern TSprites g_tSprites;
extern TXML_Root * g_pDeckListRoot;

extern EState g_eState;

TPlayer::TPlayer()
{
	m_iLifePoints = MAX_LIFE;
	m_iEnergyPoints = MAX_ENERGY;
	m_iSymptome[TAB_NAME] = SYM_CLEAN;
	m_iEquipement[TAB_NAME] = EQ_NO;
	m_iWeapon[TAB_NAME] = WPN_NO;
	m_tGameState = GAME_Locked;
	m_iAttack = ATT_NO;
	m_bAttack = true;
	m_iDeckSize = MAX_DECK;
	m_iSensibility = 1;
	m_iCardSelected = 0;
	m_iTime = 0;
	m_iPlayer = 0;
	m_bAttack = false;
	m_bPlayed = false;
	UseCardList(g_pDeckListRoot);

	for (int i = 0; i < MAX_HAND; i++)
	{
		m_pCard[i] = nullptr;
	}
}


TPlayer::~TPlayer()
{
}


bool TPlayer::Timer(int iTime)
{
	const int iActTime = GfxTimeGetMilliseconds();
	if (iActTime - m_iTime >= iTime)
	{
		return true;
	}
	return false;
}


void TPlayer::UseCardList(TXML_Root * pRootList)
{
	TXML_Element * pActCard = static_cast<TXML_Element *>(pRootList->m_pRootElementList->GetFirstItem());
	pActCard = pActCard->FindFirstSiblingElement("card", pActCard);

	for (int i = 0; i < MAX_DECK; i++)
	{
		TXML_Attributes * pCardValue = static_cast<TXML_Attributes *>(pActCard->m_pAttributesList->GetFirstItem());
		char * pName = pCardValue->m_cAttValue;
		m_pDeck[i] = FindCardByName(pName);
		if (m_pDeck[i] == nullptr)
		{
			GfxDbgPrintf("Uncomplete Deck\n");
			break;
		}
		pActCard = pActCard->FindNextSiblingElement("card");

	}
}

TCard * TPlayer::FindCardByName(char * pName)
{
	for (int i = 0; i < MAX_DIFF_CARDS; i++)
	{
		bool bSameName = true;
		for (int j = 0; j < MAX_CHAR; j++)
		{
			if (g_pCardSample[i]->m_pName[j] != pName[j])
			{
				bSameName = false;
				break;
			}
			if (g_pCardSample[i]->m_pName[j] == 0)
			{
				break;
			}
		}
		if (bSameName == true)
		{
			TCard * tNewCard = g_pCardSample[i]->Clone();
			return tNewCard;
		}
	}
	GfxDbgPrintf("No Card Found\n");
	return nullptr;
}


void TPlayer::Initialize(const TGfxVec2 tPosition, const int iPlayer)
{
	
	m_pEnergyImage = GfxImageLoad("Energy.tga");
	m_pEnergyTexture = GfxTextureCreate(m_pEnergyImage);
	m_pEnergySprite = GfxSpriteCreate(m_pEnergyTexture);

	GfxSpriteSetColor(m_pEnergySprite, GfxColor(255 + (iPlayer * 100), 255 + (iPlayer * 100), 255 + (iPlayer * 100), 255));

	m_pLifeImage = GfxImageLoad("Life.tga");
	m_pLifeTexture = GfxTextureCreate(m_pLifeImage);
	m_pLifeSprite = GfxSpriteCreate(m_pLifeTexture);
	
	GfxSpriteSetColor(m_pLifeSprite, GfxColor(255 + (iPlayer * 100), 255 + (iPlayer * 100), 255 + (iPlayer * 100), 255));

	m_pSprite = GfxSpriteCreate(g_tSprites.m_pPersoTexture);
	m_pWpnSprite = GfxSpriteCreate(g_tSprites.m_pWpnTexture);
	m_pSymSprite = GfxSpriteCreate(g_tSprites.m_pSymTexture);
	m_pEqSprite = GfxSpriteCreate(g_tSprites.m_pEqTexture);
 
	GfxSpriteSetCutout(m_pWpnSprite, 0, iPlayer * PLAYER_SIZE_Y, 0, 0);
	GfxSpriteSetCutout(m_pSymSprite, -PLAYER_SIZE_X, iPlayer * PLAYER_SIZE_Y, 0, 0);
	GfxSpriteSetCutout(m_pEqSprite, -PLAYER_SIZE_X, iPlayer * PLAYER_SIZE_Y, 0, 0);

	m_iPlayer = iPlayer;
	m_tPosition = tPosition;


	m_pPersoAnim = new TAnimation(ANIM_PLAYER_TILE_X, ANIM_PLAYER_TILE_Y, ANIM_SPEED, iPlayer, m_tPosition);
	m_pPersoAnim->Initialize("Player_Neutral.tga");
	m_pPersoAnim->Update(m_iPlayer);



	m_bPlayed = false;
	m_iTime = GfxTimeGetMilliseconds();
	m_tGameState = GAME_Neutral;
	DrawPlayer(m_tPosition);

	for (int i = 0; i < MAX_HAND; i++)
	{
		NewCard(i);
	}

	for (int i = 0; i < MAX_HAND; i++)
	{
		m_pCard[i]->Initialize();
	}

	g_tAttack.Initialize(g_tSprites.m_pAttackImage, TGfxVec2(float(ATTACK_POS_X), float(ATTACK_POS_Y)), ATTACK_SIZE_X, ATTACK_SIZE_Y);
	g_tBack.Initialize(g_tSprites.m_pBackImage, TGfxVec2(float(BACK_POS_X), float(BACK_POS_Y)), BACK_SIZE, BACK_SIZE);
	g_tEnd.Initialize(g_tSprites.m_pEndImage, TGfxVec2(float(END_POS_X), float(END_POS_Y)), END_SIZE_X, END_SIZE_Y);
	g_tChangeCards.Initialize(g_tSprites.m_pChangeImage, TGfxVec2(float(CHANGE_POS_X), float(CHANGE_POS_Y)), CHANGE_SIZE_X, CHANGE_SIZE_Y);

	
}

void TPlayer::SetSymptome(const int iSymptome, const int iDamages, const int iDurability, const int iCombo)
{
	if (iSymptome <= SYM_MAX && iSymptome >= 0)
	{
		m_iSymptome[TAB_NAME] = iSymptome;
	}
	else
	{
		GfxDbgPrintf("Error : Wrong Symptome\n");
	}
	m_iSymptome[TAB_DURABILITY] = iDurability;
	m_iSymptome[TAB_DAMAGES] = iDamages;
	m_iSymptome[TAB_COMBO] = iCombo;
}
void TPlayer::SetWeapon(const int iWeapon, const int iDamages, const int iDurability)
{
	if (iWeapon <= WPN_MAX && iWeapon >= 0)
	{
		m_iWeapon[TAB_NAME] = iWeapon;
	}
	else
	{
		GfxDbgPrintf("Error : Wrong Weapon\n");
	}
	
	m_iWeapon[TAB_DURABILITY] = iDurability;
	m_iWeapon[TAB_DAMAGES] = iDamages;
}
void TPlayer::SetEquipement(const int iEquipement, const int iDurability)
{
	if (iEquipement <= EQ_MAX && iEquipement >= 0)
	{
		m_iEquipement[TAB_NAME] = iEquipement;
	}
	else
	{
		GfxDbgPrintf("Error : Wrong Equipement\n");
	}
		
	m_iEquipement[TAB_DURABILITY] = iDurability;
}
void TPlayer::SetAttack(const int iAttack)
{
	if (iAttack <= ATT_MAX && iAttack >= 0)
	{
		m_iAttack = iAttack;
	}
	else
	{
		GfxDbgPrintf("Error : Wrong Attack\n");
	}
}
void TPlayer::SetSensibility(const int iSensibility)
{
	if (iSensibility <= 2 && iSensibility > 0)
	{
		m_iSensibility = iSensibility;
	}
	else
	{
		GfxDbgPrintf("Error : Wrong Sensibility\n");
	}
}

bool TPlayer::NewCard(int iIndex)
{
	if (m_iDeckSize > 0)
	{
		const int iRandom = GfxMathGetRandomInteger(0, m_iDeckSize - 1);
		m_pCard[iIndex] = m_pDeck[iRandom];
		m_pDeck[iRandom] = m_pDeck[m_iDeckSize - 1];
		m_pDeck[m_iDeckSize - 1] = 0;
		m_iDeckSize--;
		return true;
	}
	return false;
	
}

TCard * TPlayer::IA()
{
	int iSymptome = MAX_HAND;
	int iWeapon = MAX_HAND;
	int iEquip = MAX_HAND;
	int iAttack = MAX_HAND;
	int iCombo = MAX_HAND;
	int iHeal = MAX_HAND;


	int iCount = 0;
	for (;;)
	{
		iSymptome = MAX_HAND;
		iWeapon = MAX_HAND;
		iEquip = MAX_HAND;
		iAttack = MAX_HAND;
		iCombo = MAX_HAND;
		iHeal = MAX_HAND;

		for (int i = 0; i < MAX_HAND; i++)
		{
			if (m_pCard[i]->GetState() == false)
			{
				if (m_pCard[i]->GetType() == TYP_COMBO && m_iEnergyPoints >= m_pCard[i]->GetCost())
				{
					if (m_pCard[i]->GetName() == m_pEnemy->GetCombo())
					{
						iCombo = i;
					}
				}

				if (m_pCard[i]->GetType() == TYP_SYMPTOME && m_iEnergyPoints >= m_pCard[i]->GetCost())
				{
					if (iSymptome != MAX_HAND)
					{
						if (m_pCard[i]->GetCost() > m_pCard[iSymptome]->GetCost())
						{
							iSymptome = i;
						}
					}
					else
					{
						iSymptome = i;
					}
				}
				if (m_pCard[i]->GetType() == TYP_WEAPON && m_iEnergyPoints >= m_pCard[i]->GetCost())
				{
					iWeapon = i;
				}

				if (m_pCard[i]->GetType() == TYP_HEAL && m_iEnergyPoints >= m_pCard[i]->GetCost())
				{
					if (m_pCard[i]->GetName() == m_iSymptome[TAB_NAME])
					{
						iHeal = i;
					}
					if (m_iEnergyPoints <= MAX_LIFE / 2)
					{
						iHeal = i;
					}
				}
				if (m_pCard[i]->GetType() == TYP_EQUIPEMENT && m_iEnergyPoints >= m_pCard[i]->GetCost())
				{
					iEquip = i;
				}
				if (m_pCard[i]->GetType() == TYP_ATTACK && m_iEnergyPoints >= m_pCard[i]->GetCost())
				{
					iAttack = i;
				}
			}
		}

		if (iCombo != MAX_HAND)
		{
			if (m_pCard[iCombo]->GetCombo() == m_pEnemy->GetCombo())
			{
				m_iCardSelected = iCombo;
				return m_pCard[iCombo];
			}
		}
		if (iSymptome != MAX_HAND && m_pEnemy->GetSymptome() == SYM_CLEAN && m_pEnemy->GetEquipement() != m_pCard[iSymptome]->GetName())
		{
			m_iCardSelected = iSymptome;
			return m_pCard[iSymptome];

		}
		if (iWeapon != MAX_HAND && m_iWeapon[TAB_NAME] == WPN_NO)
		{
			m_iCardSelected = iWeapon;
			return m_pCard[iWeapon];

		}
		if (iHeal != MAX_HAND)
		{
			if (m_iLifePoints <= MAX_LIFE - 5 || m_pCard[iHeal]->GetName() == m_iSymptome[TAB_NAME])
			{
				m_iCardSelected = iHeal;
				return m_pCard[iHeal];
			}

		}
		if (iEquip != MAX_HAND && m_iEquipement[TAB_NAME] == EQ_NO)
		{
			m_iCardSelected = iEquip;
			return m_pCard[iEquip];

		}
		if (iAttack != MAX_HAND && m_iAttack == ATT_NO)
		{
			m_iCardSelected = iAttack;
			return m_pCard[iAttack];

		}
		if (iSymptome == MAX_HAND && iWeapon == MAX_HAND && iEquip == MAX_HAND && iAttack == MAX_HAND && iCombo == MAX_HAND)
		{
			break;
		}
		if (iCount >= MAX_HAND)
		{
			break;
		}
		iCount++;
	}
	if (Timer(TIMER))
	{
		if (m_iWeapon[TAB_NAME] != WPN_NO || m_iAttack != ATT_NO)
		{
			m_pEnemy->ChangeLifePoint(m_iWeapon[TAB_DAMAGES] + m_iAttack);
			m_iAttack = ATT_NO;
			m_iWeapon[TAB_DURABILITY]--;

			if (m_iWeapon[TAB_DURABILITY] == 0)
			{
				m_iWeapon[TAB_NAME] = WPN_NO;
			}
		}
		if (m_iEquipement[TAB_NAME] != EQ_NO)
		{
			m_iEquipement[TAB_DURABILITY]--;
			if (m_iEquipement[TAB_DURABILITY] <= 0)
			{
				m_iEquipement[TAB_NAME] = EQ_NO;
			}
		}
		m_iEnergyPoints = MAX_ENERGY;
		for (int i = 0; i < MAX_HAND; i++)
		{
			if (m_pCard[i]->GetState())
			{
				if (m_pCard[i] != nullptr)
				{
					m_pCard[i]->Clean();
					delete m_pCard[i];
				}
				if (NewCard(i) == false)
				{
					m_iLifePoints--;
					m_pCard[i] = new TVomiting();
					m_pCard[i]->Desactivate();
				}
				else
				{
					m_pCard[i]->Initialize();
				}
			}
		}
		
	}
	return nullptr;
}


void TPlayer::Update()
{
	TCard * CardToActivate = nullptr;

	switch (m_tGameState)
	{
		case GAME_Initialize:
		{
			m_iEnergyPoints = MAX_ENERGY;
			m_bAttack = true;
			m_tGameState = GAME_Neutral;
			for (int i = 0; i < MAX_HAND; i++)
			{
				if (m_pCard[i]->GetState())
				{
					m_pCard[i]->Clean();
					delete m_pCard[i];
					if (NewCard(i))
					{
						m_pCard[i]->Initialize();
					}
					else
					{
						m_iLifePoints--;
						m_pCard[i] = new TDiarhea();
						m_pCard[i]->Desactivate();
					}
				}
			}

			if (m_iEquipement[TAB_NAME] != EQ_NO)
			{
				m_iEquipement[TAB_DURABILITY]--;
				if (m_iEquipement[TAB_DURABILITY] <= 0)
				{
					m_iEquipement[TAB_NAME] = EQ_NO;
				}
			}
			break;
		}
		case GAME_Neutral:
		{
			for (int i = 0; i < MAX_HAND; i++)
			{
				if (m_pCard[i]->ActivePosition(i))
				{
					m_iCardSelected = i;
					m_tGameState = GAME_CardSelected;
				}
			}

			if (GfxInputIsJustPressed(EGfxInputID_MouseLeft))
			{
				if (g_tEnd.Collision())
				{
					m_tGameState = GAME_End;
				}

				if (g_tChangeCards.Collision())
				{
					if (m_iEnergyPoints >= CHANGE_HAND_COST)
					{
						this->ChangeCards();
					}
					
				}

				if (g_tAttack.Collision())
				{
					if ((m_iWeapon[TAB_NAME] != WPN_NO || m_iAttack != ATT_NO) && m_bAttack == true)
					{
						m_pEnemy->ChangeLifePoint(m_iWeapon[TAB_DAMAGES] + m_iAttack);
						m_iAttack = ATT_NO;
						m_iWeapon[TAB_DURABILITY]--;
						m_bAttack = false;
						if (m_iWeapon[TAB_DURABILITY] == 0)
						{
							m_iWeapon[TAB_NAME] = WPN_NO;
						}
					}
				}
			}
			
			
			if (m_iLifePoints <= 0)
			{
				g_eState = State_Dead;
			}


			break;
		}
		case GAME_CardSelected:
		{
			
			if (m_pCard[m_iCardSelected]->SelectedPosition())
			{
				m_tGameState = GAME_Action;
			
			}
			if (GfxInputIsJustPressed(EGfxInputID_MouseLeft))
			{
				if (g_tBack.Collision())
				{
					m_tGameState = GAME_Neutral;
				}
			}

			break;
		}
		case GAME_Action:
		{
			if (m_iEnergyPoints >= m_pCard[m_iCardSelected]->GetCost())
			{
				m_tGameState = GAME_Animation;
			}
			else
			{
				m_tGameState = GAME_CardSelected;
			}
			break;
		}
		case GAME_End:
		{
			if (m_iSymptome[TAB_NAME] != SYM_CLEAN)
			{
				ChangeLifePoint(m_iSymptome[TAB_DAMAGES]/m_iSensibility);
				m_iSymptome[TAB_DURABILITY] --;
				if (m_iSymptome[TAB_DURABILITY] == 0)
				{
					 m_iSymptome[TAB_NAME] = SYM_CLEAN;
					 SetSensibility(1);
				}
			}
			m_iAttack = ATT_NO;
			m_tGameState = GAME_Initialize;
			ChangeTurn();
			break;
		}
		case GAME_Animation:
		{
			if (this->m_pCard[m_iCardSelected]->Movement())
			{
				if (m_pCard[m_iCardSelected]->GetType() == TYP_SYMPTOME || m_pCard[m_iCardSelected]->GetType() == TYP_COMBO)
				{
					m_pCard[m_iCardSelected]->ActivateEffect(m_pEnemy, this);
				}
				else
				{
					m_pCard[m_iCardSelected]->ActivateEffect(this, this);
				}
				m_pCard[m_iCardSelected]->Desactivate();
				m_tGameState = GAME_Neutral;
			}
			break;
		}

		case GAME_Locked:
		{
			

			if (CardToActivate == nullptr)
			{
				CardToActivate = IA();
			}
			if (CardToActivate == nullptr)
			{
				m_bPlayed = false;
				if (m_iSymptome[TAB_NAME] != SYM_CLEAN)
				{
					ChangeLifePoint(m_iSymptome[TAB_DAMAGES] / m_iSensibility);
					m_iSymptome[TAB_DURABILITY] --;
					if (m_iSymptome[TAB_DURABILITY] == 0)
					{
						SetSensibility(1);
						m_iSymptome[TAB_NAME] = SYM_CLEAN;
					}
				}
				ChangeTurn();
			}
			else
			{
				if (CardToActivate->Movement())
				{
					const int iType = CardToActivate->GetType();
					if (iType == TYP_COMBO || iType == TYP_SYMPTOME)
					{
						CardToActivate->ActivateEffect(m_pEnemy, this);
					}
					else
					{
						CardToActivate->ActivateEffect(this, this);
					}
					CardToActivate->Desactivate();
					m_iCardSelected = MAX_HAND;
					CardToActivate = nullptr;
				}
			}

			if (m_iLifePoints <= 0)
			{
				g_eState = State_Dead;
			}

			break;

		}

	}
}

void TPlayer::Render() const
{
	m_pPersoAnim->Render();
	if (m_tGameState == GAME_CardSelected)
	{
		g_tBack.Draw();
	}
	GfxSpriteRender(m_pEqSprite);
	GfxSpriteRender(m_pSymSprite);
	if (m_iWeapon[TAB_NAME] != WPN_NO)
	{
		GfxSpriteRender(m_pWpnSprite);
	}
	if ((m_iAttack != 0 || m_iWeapon[TAB_NAME] != WPN_NO) && m_bAttack == true && m_tGameState == GAME_Neutral)
	{
		
		g_tAttack.Draw();
	}
	if (m_tGameState == GAME_Neutral)
	{
		g_tEnd.Draw();
		

		g_tChangeCards.Draw();
	}
}

void TPlayer::Clean()
{
	for (int i = 0; i < MAX_HAND; i++)
	{
		if (m_pCard[i] != nullptr)
		{
			m_pCard[i]->Clean();
			delete m_pCard[i];
		}
	}
	for (int i = 0; i < MAX_DECK; i++)
	{
		if (m_pDeck[i] != nullptr)
		{
			m_pDeck[i]->Clean();
			delete m_pDeck[i];
		}
	}

	m_pPersoAnim->Clean();
	delete m_pPersoAnim;

	GfxImageDestroy(m_pEnergyImage);
	GfxImageDestroy(m_pLifeImage);
	GfxTextureDestroy(m_pEnergyTexture);
	GfxTextureDestroy(m_pLifeTexture);
	GfxSpriteDestroy(m_pEnergySprite);
	GfxSpriteDestroy(m_pSprite);
	GfxSpriteDestroy(m_pLifeSprite);
	GfxSpriteDestroy(m_pWpnSprite);
	GfxSpriteDestroy(m_pSymSprite);
	GfxSpriteDestroy(m_pEqSprite);
}

void TPlayer::ChangeTurn() const
{
	if (g_eState != State_Dead)
	{
		g_eState = (g_eState == State_Player1) ? State_Player2 : State_Player1;
	}
	

}

void TPlayer::ChangeCards()
{
	for (int i = 0; i < MAX_HAND; i++)
	{
		m_iDeckSize++;
		this->m_pDeck[m_iDeckSize - 1] = this->m_pCard[i];
		this->m_pCard[i]->Clean();
		this->NewCard(i);
		
		this->m_pCard[i]->Initialize();
	}

	this->ChangeEnergy(CHANGE_HAND_COST);
}

void TPlayer::ChangeState()
{
	GfxSpriteSetCutout(m_pSymSprite, (m_iSymptome[TAB_NAME] - 1) * PLAYER_SIZE_X, m_iPlayer * PLAYER_SIZE_Y, PLAYER_SIZE_X, PLAYER_SIZE_Y);
	GfxSpriteSetCutout(m_pEqSprite, (m_iEquipement[TAB_NAME] - 1) * PLAYER_SIZE_X, m_iPlayer * PLAYER_SIZE_Y, PLAYER_SIZE_X, PLAYER_SIZE_Y);
	GfxSpriteSetCutout(m_pWpnSprite, (m_iWeapon[TAB_NAME] - 1) * PLAYER_SIZE_X, m_iPlayer * PLAYER_SIZE_Y, PLAYER_SIZE_X, PLAYER_SIZE_Y);
}

void TPlayer::ChangeEnergy(const int iChange)
{
	m_iEnergyPoints -= iChange;
}

void TPlayer::ChangeLifePoint(const int iChange)
{
	m_iLifePoints -= iChange * m_iSensibility;
}

int TPlayer::GetSelected() const
{
	return m_iCardSelected;
}

int TPlayer::GetLifePoints() const
{
	return m_iLifePoints;
}

int TPlayer::GetEnergy() const
{
	return m_iEnergyPoints;
}

int TPlayer::GetWeapon() const
{
	return m_iWeapon[TAB_NAME];
}

int TPlayer::GetSymptome() const
{
	return m_iSymptome[TAB_NAME];
}

int TPlayer::GetEquipement() const
{
	return m_iEquipement[TAB_NAME];
}

int TPlayer::GetAttack() const
{
	return m_iAttack;
}
int TPlayer::GetDeckSize() const
{
	return m_iDeckSize;
}

int TPlayer::GetCombo() const
{
	return m_iSymptome[TAB_COMBO];
}

bool TPlayer::GetAttacked() const
{
	return m_bAttack;
}

void TPlayer::UpdateCardList()
{
	GfxTextSpritePrintf(g_tSprites.m_pDeckList, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", m_pDeck[0]->m_pName,
		m_pDeck[1]->m_pName, m_pDeck[2]->m_pName, m_pDeck[3]->m_pName, m_pDeck[4]->m_pName, m_pDeck[5]->m_pName, m_pDeck[6]->m_pName, m_pDeck[7]->m_pName,
		m_pDeck[8]->m_pName, m_pDeck[9]->m_pName, m_pDeck[10]->m_pName, m_pDeck[11]->m_pName, m_pDeck[12]->m_pName, m_pDeck[13]->m_pName, m_pDeck[14]->m_pName,
		m_pDeck[15]->m_pName, m_pDeck[16]->m_pName, m_pDeck[17]->m_pName, m_pDeck[18]->m_pName, m_pDeck[19]->m_pName);
}

void TPlayer::DrawPlayer(const TGfxVec2 tPosition) const
{

	GfxSpriteSetPosition(m_pSprite, tPosition.x, tPosition.y);
	GfxSpriteSetScale(m_pEnergySprite, float(m_iEnergyPoints), 1.0f);
	GfxSpriteSetScale(m_pLifeSprite, float(m_iLifePoints), 1.0f);
	GfxSpriteSetPosition(m_pEnergySprite, float(PLAYER_SIZE_X + (m_iPlayer * BAR_SIZE_X) + (m_iPlayer * INTER_BAR)), tPosition.y + ENERGY_BAR_Y);
	GfxSpriteSetPosition(m_pLifeSprite, float(PLAYER_SIZE_X + (m_iPlayer * BAR_SIZE_X) + (m_iPlayer * INTER_BAR)), tPosition.y + LIFE_BAR_Y);
	GfxSpriteSetPosition(m_pWpnSprite, tPosition.x, tPosition.y);
	GfxSpriteSetPosition(m_pSymSprite, tPosition.x, tPosition.y);
	GfxSpriteSetPosition(m_pEqSprite, tPosition.x, tPosition.y);
	
}
