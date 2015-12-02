#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

enum EState
{
	State_Player1 = 1,
	State_Player2,
	State_MainMenu,
	State_DeckMenu,
	State_BreakMenu,
	State_Dead
};


class TGameState
{
public:
	TGameState();
	~TGameState();


	void Initialize();
	void Update();
	void Render() const;
};

#endif

