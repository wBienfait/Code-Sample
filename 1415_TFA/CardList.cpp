#include "CardList.h"
#include "Player.h"

TDiarhea::TDiarhea()
{
	m_iDamages = 1;
	m_iDurability = 3;
	m_eType = TYP_SYMPTOME;
	m_iName = SYM_DIARHEA;
	m_iCombo = COM_1;
	m_iCost = 3;
	m_bUsed = false;

	m_pName = "Diarhea";
	m_pEffect = "Deal 1 damage to your opponent \n\nat the end of every of his turn \n\nduring 3 turns";

}

TDiarhea::~TDiarhea()
{
}

void TDiarhea::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Diarhea\n");
	const int iEnergy = -tMe->GetEnergy();
	if (iEnergy <= m_iCost)
	{
		tTarget->SetSymptome(m_iName, m_iDamages, m_iDurability, m_iCombo);
		tMe->ChangeEnergy(m_iCost);
	}
}

TVomiting::TVomiting()
{
	m_iDamages = 1;
	m_iDurability = 1;
	m_eType = TYP_SYMPTOME;
	m_iName = SYM_VOMITING;
	m_iCombo = COM_1;
	m_iCost = 2;
	m_bUsed = false;
	m_pName = "Vomiting";
	m_pEffect = "Deal 1 damage to your opponent \n\nat the end of every of his turn \n\nduring 1 turns";
}

TVomiting::~TVomiting()
{
}

void TVomiting::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Vomiting\n");
	const int iEnergy = -tMe->GetEnergy();
	if (iEnergy <= m_iCost)
	{
		tTarget->SetSymptome(m_iName, m_iDamages, m_iDurability, m_iCombo);
		tMe->ChangeEnergy(m_iCost);
	}
}

TArm::TArm()
{
	m_iDamages = 2;
	m_iDurability = 2;
	m_eType = TYP_WEAPON;
	m_iName = WPN_ARM;
	m_iCombo = COM_NO;
	m_iCost = 2;
	m_bUsed = false;
	m_pName = "Arm";
	m_pEffect = "equip you : arm of a zombie that deals \n\n2 physical damages, available \n\nfor two attacks";
}

TArm::~TArm()
{
}

void TArm::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Arm\n");
	const int iEnergy = -tMe->GetEnergy();
	if (iEnergy <= m_iCost)
	{
		tTarget->SetWeapon(m_iName, m_iDamages, m_iDurability);
		tMe->ChangeEnergy(m_iCost);
	}

}

TTransix::TTransix()
{
	//m_iDamages = -2;
	m_iDurability = 2;
	m_eType = TYP_EQUIPEMENT;
	m_iName = EQ_DIARHEA;
	m_iCombo = COM_NO;
	m_iCost = 2;
	m_bUsed = false;
	m_pName = "Transix";
	m_pEffect = "Immune yoursef against diarhea for 2 turns";
}

TTransix::~TTransix()
{
}

void TTransix::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Transix\n");
	const int iEnergy = -tMe->GetEnergy();
	if (iEnergy <= m_iCost)
	{
		tTarget->SetEquipement(m_iName, m_iDurability);
		tMe->ChangeEnergy(m_iCost);
	}

}

TPunch::TPunch()
{
	m_iDamages = 1;
	m_iDurability = 0;
	m_eType = TYP_ATTACK;
	m_iName = ATT_FIST;
	m_iCombo = COM_NO;
	m_iCost = 1;
	m_bUsed = false;
	m_pName = "Punch";
	m_pEffect = "Allow you to deal one physical damage this turn";
}

TPunch::~TPunch()
{
}

void TPunch::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Fist\n");
	const int iEnergy = -tMe->GetEnergy();
	if (iEnergy <= m_iCost)
	{
		tTarget->SetAttack(m_iDamages);
		tMe->ChangeEnergy(m_iCost);
	}

}

TSneeze::TSneeze()
{
	m_eType = TYP_COMBO;
	m_iName = COM_1;
	m_iCombo = COM_1;
	m_iCost = 2;
	m_bUsed = false;
	m_pName = "Sneeze";
	m_pEffect = "if your opponent has diarhea or vomiting, \n\ndeal 3 damages but cure his symptom";
}


TSneeze::~TSneeze()
{

}

void TSneeze::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Sneeze\n");
	if (tTarget->GetCombo() == m_iCombo)
	{
		if (tTarget->GetSymptome() == SYM_DIARHEA)
		{
			tTarget->ChangeLifePoint(COM_1_DAMAGES);
			tTarget->SetSymptome(SYM_CLEAN, 0, 0, 0);
		}

		if (tTarget->GetSymptome() == SYM_VOMITING)
		{
			tTarget->ChangeLifePoint(COM_1_DAMAGES);
			tTarget->SetAttack(COM_1_ATTACK);
			tTarget->SetSymptome(SYM_CLEAN, 0, 0, 0);
		}
	}

	tMe->ChangeEnergy(m_iCost);

}

TZombie::TZombie()
{
	m_eType = TYP_SYMPTOME;
	m_iName = SYM_ZOMBIE;
	m_iDurability = 2;
	m_iCombo = COM_2;
	m_iCost = 3;
	m_bUsed = false;
	m_pName = "Zombie";
	m_pEffect = "Transform your opponent to a zombie, \n\nhe takes the double of physical damages deal to him \n\nbut equip him with his own zombie arm";
}


TZombie::~TZombie()
{

}

void TZombie::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Zombie\n");
	tTarget->SetSymptome(m_iName, m_iDamages, m_iDurability, m_iCombo);
	tTarget->SetSensibility(2);
	TArm * pNewWeapon = new TArm();
	pNewWeapon->ActivateEffect(tTarget, tTarget);
	tMe->ChangeEnergy(m_iCost);

}

TDiarium::TDiarium()
{
	m_eType = TYP_HEAL;
	m_iName = HEAL_DIARHEA;
	m_iDurability = 0;
	m_iCombo = COM_NO;
	m_iCost = 3;
	m_bUsed = false;

	m_pName = "Diarium";
	m_pEffect = "Cure your diarhea";
}


TDiarium::~TDiarium()
{

}

void TDiarium::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Diarium\n");
	if (tTarget->GetSymptome() == m_iName)
	{
		tTarget->SetSymptome(SYM_CLEAN, 0, 0, 0);
	}
	tMe->ChangeEnergy(m_iCost);

}

THealthStick::THealthStick()
{
	m_eType = TYP_HEAL;
	m_iName = HEAL_HEALTH;
	m_iDurability = 0;
	m_iDamages = -5;
	m_iCombo = COM_NO;
	m_iCost = 3;
	m_bUsed = false;

	m_pName = "Health Stick";
	m_pEffect = "Restore you 5 health";
}


THealthStick::~THealthStick()
{

}

void THealthStick::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Health\n");
	tTarget->ChangeLifePoint(m_iDamages);
	if (tTarget->GetLifePoints() > MAX_LIFE)
	{
		tTarget->ChangeLifePoint(tMe->GetLifePoints() - MAX_LIFE);
	}
	tMe->ChangeEnergy(m_iCost);

}

TBerserk::TBerserk()
{
	m_eType = TYP_SYMPTOME;
	m_iName = SYM_BERSERK;
	m_iDurability = 2;
	m_iDamages = 2;
	m_iCombo = COM_NO;
	m_iCost = 3;
	m_bUsed = false;
	m_pName = "Berserk";
	m_pEffect = "Deal 2 damages to your opponent at the end \n\nof every of his turn during 2 turns but \n\nallow his to deal you 3 physical \n\ndamages his next turn";
}


TBerserk::~TBerserk()
{

}

void TBerserk::ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const
{
	//GfxDbgPrintf("Berserk\n");
	tTarget->SetSymptome(SYM_BERSERK, m_iDamages, m_iDurability, m_iCombo);
	tTarget->SetAttack(3);
	tMe->ChangeEnergy(m_iCost);

}



