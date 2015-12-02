#ifndef LUA_H_INCLUDED
#define LUA_H_INCLUDED

#include "Lua.h"
#include "flib.h"

class TLuaFunctions
{
public:
	lua_State * L;

	void Initialize();

	bool Animation(TGfxSprite * pCardSprite) const;
	
	TLuaFunctions();
	~TLuaFunctions();
};

#endif

