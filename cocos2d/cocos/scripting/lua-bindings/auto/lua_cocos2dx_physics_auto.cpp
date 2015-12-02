#include "lua_cocos2dx_physics_auto.hpp"
#if CC_USE_PHYSICS
#include "cocos2d.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

TOLUA_API int register_all_cocos2dx_physics(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"cc",0);
	tolua_beginmodule(tolua_S,"cc");


	tolua_endmodule(tolua_S);
	return 1;
}

#endif
