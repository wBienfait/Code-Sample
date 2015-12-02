#include "flib.h"
#include "flib_vec2.h"

#include "Card.h"
#include"Constantes.h"
#include"Player.h"
#include "GameState.h"
#include "Sprites.h"

TGameState g_tGameState;


void Initialize()
{
	g_tGameState.Initialize();
}

void Update()
{
	g_tGameState.Update();
}

void Render()
{
	GfxClear(EGfxColor_Black);
	g_tGameState.Render();
}


void GfxMain(int, char *[])
{
	GfxCallbacks(Initialize, Update, Render);
}
