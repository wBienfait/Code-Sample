#include "LuaFunctions.h"



TLuaFunctions::TLuaFunctions()
{
}


TLuaFunctions::~TLuaFunctions()
{
}

void TLuaFunctions::Initialize()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	LuaDoFile(L, "MoveCards.lua");
}

bool TLuaFunctions::Animation(TGfxSprite * pCardSprite) const
{
	const float fStartAngle = GfxSpriteGetAngle(pCardSprite);
	lua_pushnumber(L, fStartAngle);
	const int iReturned = LuaCallGlobal(L, "Move");
	float fAngle = float(luaL_checknumber(L, iReturned));
	lua_pop(L, 1);

	
	GfxSpriteSetAngle(pCardSprite, fAngle);
	lua_pushnumber(L, fAngle);
	const int iReturned2 = LuaCallGlobal(L, "Check");
	const bool bEnd = lua_toboolean(L, iReturned2);
	lua_pop(L, 1);
	return bEnd;
}
