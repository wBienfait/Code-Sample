#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED


const int SYM_CLEAN = 0;
const int SYM_DIARHEA = 1;
const int SYM_ZOMBIE = 2;
const int SYM_VOMITING = 3;
const int SYM_BERSERK = 4;
const int SYM_MAX = 4;


const int EQ_NO = 0;
const int EQ_DIARHEA = 1;
const int EQ_ZOMBIE = 2;
const int EQ_VOMITING = 3;
const int EQ_MAX = 3;

const int HEAL_DIARHEA = 1;
const int HEAL_ZOMBIE = 3;
const int HEAL_HEALTH = 2;
const int HEAL_MAX = 3;


const int WPN_NO = 0;
const int WPN_ARM = 1;
const int WPN_MAX = 1;

const int ATT_NO = 0;
const int ATT_FIST = 1;
const int ATT_MAX = 3;

enum ETypeCard
{
	TYP_SYMPTOME = 0,
	TYP_EQUIPEMENT = 1,
	TYP_WEAPON = 2,
	TYP_ATTACK = 3,
	TYP_COMBO = 4,
	TYP_HEAL = 5
};

const int TAB_NAME = 0;
const int TAB_DURABILITY = 1;
const int TAB_DAMAGES = 2;
const int TAB_COMBO = 3;

const int COM_NO = 0;
const int COM_1 = 1;
const int COM_1_DAMAGES = 3;
const int COM_1_ATTACK = 2;
const int COM_2 = 2;

const int PLAYER_1 = 0;
const int PLAYER_2 = 1;

const int TIMER = 0;
const int MAX_DECK = 20;
const int MAX_CHAR = 100;
const int MAX_TAB_SYMPTOME = 4;
const int MAX_TAB_WEAPON = 3;
const int MAX_TAB_EQUIPEMENT = 2;
const int MAX_LIFE = 20;
const int MAX_ENERGY = 4;
const int MAX_DIFF_CARDS = 10;
const int MAX_HAND = 5;
const int MAX_PLAYER = 2;
const int SIZE_X = 480;
const int SIZE_Y = 320;
const int MAX_SIZE = SIZE_X * SIZE_Y;
const int PLAYER_SIZE_X = 90;
const int PLAYER_SIZE_Y = 160;
const int ATTACK_SIZE = 30;
const int ATTACK_COUNT_POS_X = PLAYER_SIZE_X - ATTACK_SIZE;
const int ATTACK_COUNT_POS_Y = PLAYER_SIZE_Y * 2 - ATTACK_SIZE;
const int CARD_SIZE_X = 90;
const int CARD_SIZE_Y = 125;
const int INTERCARD_X = (SIZE_X - (CARD_SIZE_X*MAX_HAND)) / 6;
const int INTERCARD_Y = (PLAYER_SIZE_Y + 15);
const int INTER_BUTTONS = 10;
const int PASSIVE_POS = PLAYER_SIZE_X + (SIZE_X - PLAYER_SIZE_X - CARD_SIZE_X) / 2;
const int BORDER = 50;
const int SELECTED_POS_X = BORDER;
const int SELECTED_POS_Y = SIZE_Y / 2 - CARD_SIZE_Y / 2;
const int CHANGE_HAND_COST = 2;
const int CARD_BORDER = 3;
const int WEAPON_POS_X = PLAYER_SIZE_X - 5 - 82;
const int WEAPON_POS_Y = 73;
const int BAR_SIZE_X = 141;
const int LIFE_BAR_Y = 12;
const int ENERGY_BAR_Y = 28;
const int ANIM_PLAYER_TILE_X = 5;
const int ANIM_PLAYER_TILE_Y = 2;
const int INTER_BAR = 18;
const int ANIM_SPEED = 50;

const int BACK_SIZE = 20;
const int BACK_POS_X = 20;
const int BACK_POS_Y = 20;

const int END_SIZE_X = 40;
const int END_SIZE_Y = 20;
const int END_POS_X = (SIZE_X / 2) - END_SIZE_X - INTER_BUTTONS;
const int END_POS_Y = PLAYER_SIZE_Y - END_SIZE_Y - INTER_BUTTONS;

const int ATTACK_SIZE_X = 60;
const int ATTACK_SIZE_Y = 20;
const int ATTACK_POS_X = END_POS_X - ATTACK_SIZE_X - INTER_BUTTONS;
const int ATTACK_POS_Y = SIZE_Y / 2 - ATTACK_SIZE_Y - INTER_BUTTONS;

const int START_POS_X = 50;
const int START_POS_Y = 50;
const int START_SIZE_X = 240;
const int START_SIZE_Y = 80;

const int BREAK_SIZE_X = 60;
const int BREAK_SIZE_Y = 20;
const int BREAK_POS_X = SIZE_X - BREAK_SIZE_X - INTER_BUTTONS/2;
const int BREAK_POS_Y = SIZE_Y - BREAK_SIZE_Y - INTER_BUTTONS/2;

const int CONTINUE_POS_X = 100;
const int CONTINUE_POS_Y = 100;
const int CONTINUE_SIZE_X = 60;
const int CONTINUE_SIZE_Y = 20;

const int RESTART_POS_X = 100;
const int RESTART_POS_Y = 150;
const int RESTART_SIZE_X = 60;
const int RESTART_SIZE_Y = 20;

const int DECK_LIST_POS_X = 50;
const int DECK_LIST_POS_Y = 130;
const int DECK_LIST_SIZE_X = 240;
const int DECK_LIST_SIZE_Y = 80;


const int CHANGE_SIZE_X = 120;
const int CHANGE_SIZE_Y = 80;
const int CHANGE_POS_X = PLAYER_SIZE_X;
const int CHANGE_POS_Y = 45;




#endif