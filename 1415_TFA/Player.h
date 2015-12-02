#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "flib.h"
#include "flib_vec2.h"
//#include "Constantes.h"
#include "Card.h"

class TXML_Root;
class TAnimation;

enum EGamePhases
{
	GAME_Initialize = 1,
	GAME_Neutral,
	GAME_Action,
	GAME_CardSelected,
	GAME_End,
	GAME_Animation,
	GAME_Locked
};

class TPlayer
{
private:

	TGfxVec2 m_tPosition;

	TGfxImage * m_pEnergyImage = nullptr;
	TGfxTexture * m_pEnergyTexture = nullptr;

	TGfxImage * m_pLifeImage = nullptr;
	TGfxTexture * m_pLifeTexture = nullptr;

	TCard * m_pDeck[MAX_DECK];

	int m_iLifePoints;
	int m_iEnergyPoints;
	int m_iSymptome[MAX_TAB_SYMPTOME];
	int m_iEquipement[MAX_TAB_EQUIPEMENT];
	int m_iWeapon[MAX_TAB_WEAPON];
	int m_iAttack;
	int m_iSensibility;
	int m_iCardSelected;
	int m_iTime;
	int m_iDeckSize;
	int m_iPlayer;
	bool m_bAttack;
	bool m_bPlayed;




	void DrawPlayer(const TGfxVec2 tPosition) const;
	void ChangeCards();
	void ChangeTurn() const;
	bool NewCard(int iIndex);
	bool Timer(int iTime);

	TCard * IA();
	void UseCardList(TXML_Root * pRootList);
	TCard * FindCardByName(char * pName);
	

public:

	EGamePhases m_tGameState;
	TAnimation * m_pPersoAnim = nullptr;

	void SetSymptome(const int iSymptome, const int iDamages, const int iDurability, const int iCombo);
	void SetEquipement(const int iEquipement, const int iDurability);
	void SetAttack(const int iAttack);
	void SetWeapon(const int iWeapon, const int iDamages, const int iDurability);
	void SetSensibility(const int iSensibility);


	int GetAttack() const;
	int GetSelected() const;
	int GetLifePoints()const;
	bool GetAttacked() const;
	int GetEnergy()const;
	int GetSymptome()const;
	int GetEquipement() const;
	int GetWeapon()const;
	int GetDeckSize() const;
	int GetCombo() const;


	void ChangeLifePoint(const int iChange);
	void ChangeEnergy(const int iChange);

	void Initialize(const TGfxVec2 tPosition, const int iPlayer);
	void Update();
	void Render() const;
	void Clean();

	void UpdateCardList();

	void ChangeState();

	TGfxSprite * m_pSprite = nullptr;
	TGfxSprite * m_pEnergySprite = nullptr;
	TGfxSprite * m_pLifeSprite = nullptr;
	TGfxSprite * m_pWpnSprite = nullptr;
	TGfxSprite * m_pSymSprite = nullptr;
	TGfxSprite * m_pEqSprite = nullptr;


	TCard * m_pCard[MAX_HAND];
	TPlayer * m_pEnemy = nullptr;



	
	
	
	TPlayer();
	~TPlayer();
};

#endif

