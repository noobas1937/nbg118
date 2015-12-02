#include "lua_nbg_auto.hpp"
#include "LuaCCBNode.h"
#include "NBGNetManager.h"
#include "NBG.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"


int lua_nbg_LuaCCBNode_beforeLoadNodeResource(lua_State* tolua_S)
{
    int argc = 0;
    LuaCCBNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCCBNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCCBNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_LuaCCBNode_beforeLoadNodeResource'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        const BANodeResourceInfo* arg0;

        ok &= luaval_to_object<const BANodeResourceInfo>(tolua_S, 2, "BANodeResourceInfo",&arg0, "LuaCCBNode:beforeLoadNodeResource");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_LuaCCBNode_beforeLoadNodeResource'", nullptr);
            return 0;
        }
        cobj->beforeLoadNodeResource(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCCBNode:beforeLoadNodeResource",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_LuaCCBNode_beforeLoadNodeResource'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_LuaCCBNode_onEnter(lua_State* tolua_S)
{
    int argc = 0;
    LuaCCBNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCCBNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCCBNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_LuaCCBNode_onEnter'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_LuaCCBNode_onEnter'", nullptr);
            return 0;
        }
        cobj->onEnter();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCCBNode:onEnter",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_LuaCCBNode_onEnter'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_LuaCCBNode_onExit(lua_State* tolua_S)
{
    int argc = 0;
    LuaCCBNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCCBNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCCBNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_LuaCCBNode_onExit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_LuaCCBNode_onExit'", nullptr);
            return 0;
        }
        cobj->onExit();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCCBNode:onExit",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_LuaCCBNode_onExit'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_LuaCCBNode_setLuaHandlerOnExit(lua_State* tolua_S)
{
    int argc = 0;
    LuaCCBNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCCBNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCCBNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnExit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        CXLUAFUNC arg0;

        arg0 = (  toluafix_ref_function(tolua_S,2,0));
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnExit'", nullptr);
            return 0;
        }
        cobj->setLuaHandlerOnExit(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCCBNode:setLuaHandlerOnExit",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnExit'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_LuaCCBNode_setLuaHandlerAnimationCallback(lua_State* tolua_S)
{
    int argc = 0;
    LuaCCBNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCCBNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCCBNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_LuaCCBNode_setLuaHandlerAnimationCallback'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        CXLUAFUNC arg0;

        arg0 = (  toluafix_ref_function(tolua_S,2,0));
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_LuaCCBNode_setLuaHandlerAnimationCallback'", nullptr);
            return 0;
        }
        cobj->setLuaHandlerAnimationCallback(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCCBNode:setLuaHandlerAnimationCallback",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_LuaCCBNode_setLuaHandlerAnimationCallback'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_LuaCCBNode_setLuaHandlerOnTouchClick(lua_State* tolua_S)
{
    int argc = 0;
    LuaCCBNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCCBNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCCBNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnTouchClick'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        CXLUAFUNC arg0;

        arg0 = (  toluafix_ref_function(tolua_S,2,0));
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnTouchClick'", nullptr);
            return 0;
        }
        cobj->setLuaHandlerOnTouchClick(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCCBNode:setLuaHandlerOnTouchClick",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnTouchClick'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_LuaCCBNode_setLuaHandlerOnEnter(lua_State* tolua_S)
{
    int argc = 0;
    LuaCCBNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCCBNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCCBNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnEnter'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        CXLUAFUNC arg0;

        arg0 = (  toluafix_ref_function(tolua_S,2,0));
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnEnter'", nullptr);
            return 0;
        }
        cobj->setLuaHandlerOnEnter(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCCBNode:setLuaHandlerOnEnter",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_LuaCCBNode_setLuaHandlerOnEnter'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_LuaCCBNode_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaCCBNode",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        const char* arg0;
        CXLUAFUNC arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "LuaCCBNode:create"); arg0 = arg0_tmp.c_str();
        arg1 = (  toluafix_ref_function(tolua_S,3,0));
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_LuaCCBNode_create'", nullptr);
            return 0;
        }
        LuaCCBNode* ret = LuaCCBNode::create(arg0, arg1);
        object_to_luaval<LuaCCBNode>(tolua_S, "LuaCCBNode",(LuaCCBNode*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "LuaCCBNode:create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_LuaCCBNode_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_nbg_LuaCCBNode_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (LuaCCBNode)");
    return 0;
}

int lua_register_nbg_LuaCCBNode(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LuaCCBNode");
    tolua_cclass(tolua_S,"LuaCCBNode","LuaCCBNode","NBGNode",nullptr);

    tolua_beginmodule(tolua_S,"LuaCCBNode");
        tolua_function(tolua_S,"beforeLoadNodeResource",lua_nbg_LuaCCBNode_beforeLoadNodeResource);
        tolua_function(tolua_S,"onEnter",lua_nbg_LuaCCBNode_onEnter);
        tolua_function(tolua_S,"onExit",lua_nbg_LuaCCBNode_onExit);
        tolua_function(tolua_S,"setLuaHandlerOnExit",lua_nbg_LuaCCBNode_setLuaHandlerOnExit);
        tolua_function(tolua_S,"setLuaHandlerAnimationCallback",lua_nbg_LuaCCBNode_setLuaHandlerAnimationCallback);
        tolua_function(tolua_S,"setLuaHandlerOnTouchClick",lua_nbg_LuaCCBNode_setLuaHandlerOnTouchClick);
        tolua_function(tolua_S,"setLuaHandlerOnEnter",lua_nbg_LuaCCBNode_setLuaHandlerOnEnter);
        tolua_function(tolua_S,"create", lua_nbg_LuaCCBNode_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaCCBNode).name();
    g_luaType[typeName] = "LuaCCBNode";
    g_typeCast["LuaCCBNode"] = "LuaCCBNode";
    return 1;
}

int lua_nbg_NBGNetManager_setIsDebugMode(lua_State* tolua_S)
{
    int argc = 0;
    NBGNetManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NBGNetManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NBGNetManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_NBGNetManager_setIsDebugMode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "NBGNetManager:setIsDebugMode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBGNetManager_setIsDebugMode'", nullptr);
            return 0;
        }
        cobj->setIsDebugMode(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "NBGNetManager:setIsDebugMode",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBGNetManager_setIsDebugMode'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_NBGNetManager_getIsDebugMode(lua_State* tolua_S)
{
    int argc = 0;
    NBGNetManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NBGNetManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NBGNetManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_NBGNetManager_getIsDebugMode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBGNetManager_getIsDebugMode'", nullptr);
            return 0;
        }
        bool ret = cobj->getIsDebugMode();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "NBGNetManager:getIsDebugMode",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBGNetManager_getIsDebugMode'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_NBGNetManager_send(lua_State* tolua_S)
{
    int argc = 0;
    NBGNetManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NBGNetManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NBGNetManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_NBGNetManager_send'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        cocos2d::ValueMap arg1;
        CXLUAFUNC arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "NBGNetManager:send");

        ok &= luaval_to_ccvaluemap(tolua_S, 3, &arg1, "NBGNetManager:send");

        arg2 = (  toluafix_ref_function(tolua_S,4,0));
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBGNetManager_send'", nullptr);
            return 0;
        }
        cobj->send(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "NBGNetManager:send",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBGNetManager_send'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_NBGNetManager_connect(lua_State* tolua_S)
{
    int argc = 0;
    NBGNetManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NBGNetManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NBGNetManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_NBGNetManager_connect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        int arg1;
        CXLUAFUNC arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "NBGNetManager:connect");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "NBGNetManager:connect");

        arg2 = (  toluafix_ref_function(tolua_S,4,0));
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBGNetManager_connect'", nullptr);
            return 0;
        }
        cobj->connect(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "NBGNetManager:connect",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBGNetManager_connect'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_NBGNetManager_login(lua_State* tolua_S)
{
    int argc = 0;
    NBGNetManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NBGNetManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NBGNetManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_nbg_NBGNetManager_login'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::ValueMap arg0;
        CXLUAFUNC arg1;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "NBGNetManager:login");

        arg1 = (  toluafix_ref_function(tolua_S,3,0));
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBGNetManager_login'", nullptr);
            return 0;
        }
        cobj->login(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "NBGNetManager:login",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBGNetManager_login'.",&tolua_err);
#endif

    return 0;
}
int lua_nbg_NBGNetManager_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBGNetManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBGNetManager_getInstance'", nullptr);
            return 0;
        }
        NBGNetManager* ret = NBGNetManager::getInstance();
        object_to_luaval<NBGNetManager>(tolua_S, "NBGNetManager",(NBGNetManager*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBGNetManager:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBGNetManager_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_nbg_NBGNetManager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (NBGNetManager)");
    return 0;
}

int lua_register_nbg_NBGNetManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NBGNetManager");
    tolua_cclass(tolua_S,"NBGNetManager","NBGNetManager","",nullptr);

    tolua_beginmodule(tolua_S,"NBGNetManager");
        tolua_function(tolua_S,"setIsDebugMode",lua_nbg_NBGNetManager_setIsDebugMode);
        tolua_function(tolua_S,"getIsDebugMode",lua_nbg_NBGNetManager_getIsDebugMode);
        tolua_function(tolua_S,"send",lua_nbg_NBGNetManager_send);
        tolua_function(tolua_S,"connect",lua_nbg_NBGNetManager_connect);
        tolua_function(tolua_S,"login",lua_nbg_NBGNetManager_login);
        tolua_function(tolua_S,"getInstance", lua_nbg_NBGNetManager_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NBGNetManager).name();
    g_luaType[typeName] = "NBGNetManager";
    g_typeCast["NBGNetManager"] = "NBGNetManager";
    return 1;
}

int lua_nbg_NBG_getOpenUDID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getOpenUDID'", nullptr);
            return 0;
        }
        std::string ret = NBG::getOpenUDID();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    if (argc == 1)
    {
        bool arg0;
        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "NBG:getOpenUDID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getOpenUDID'", nullptr);
            return 0;
        }
        std::string ret = NBG::getOpenUDID(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getOpenUDID",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getOpenUDID'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getLanguage(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getLanguage'", nullptr);
            return 0;
        }
        std::string ret = NBG::getLanguage();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getLanguage",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getLanguage'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getCurrentSecond(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getCurrentSecond'", nullptr);
            return 0;
        }
        long ret = NBG::getCurrentSecond();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getCurrentSecond",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getCurrentSecond'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getCurrentMillisecond(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getCurrentMillisecond'", nullptr);
            return 0;
        }
        long long ret = NBG::getCurrentMillisecond();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getCurrentMillisecond",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getCurrentMillisecond'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getPublishRegion(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getPublishRegion'", nullptr);
            return 0;
        }
        std::string ret = NBG::getPublishRegion();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getPublishRegion",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getPublishRegion'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getUDID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getUDID'", nullptr);
            return 0;
        }
        std::string ret = NBG::getUDID();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getUDID",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getUDID'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_get_COCOS2D_DEBUG(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_get_COCOS2D_DEBUG'", nullptr);
            return 0;
        }
        int ret = NBG::get_COCOS2D_DEBUG();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:get_COCOS2D_DEBUG",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_get_COCOS2D_DEBUG'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_md5(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        const char* arg0;
        std::string arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "NBG:md5"); arg0 = arg0_tmp.c_str();
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "NBG:md5");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_md5'", nullptr);
            return 0;
        }
        std::string ret = NBG::md5(arg0, arg1);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:md5",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_md5'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getVersionCode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getVersionCode'", nullptr);
            return 0;
        }
        std::string ret = NBG::getVersionCode();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getVersionCode",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getVersionCode'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getCountryCode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getCountryCode'", nullptr);
            return 0;
        }
        std::string ret = NBG::getCountryCode();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getCountryCode",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getCountryCode'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getPlatformToken(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getPlatformToken'", nullptr);
            return 0;
        }
        std::string ret = NBG::getPlatformToken();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getPlatformToken",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getPlatformToken'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getHandSetInfo(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getHandSetInfo'", nullptr);
            return 0;
        }
        std::string ret = NBG::getHandSetInfo();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getHandSetInfo",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getHandSetInfo'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getVersionName(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getVersionName'", nullptr);
            return 0;
        }
        std::string ret = NBG::getVersionName();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getVersionName",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getVersionName'.",&tolua_err);
#endif
    return 0;
}
int lua_nbg_NBG_getPlatformUID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NBG",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_nbg_NBG_getPlatformUID'", nullptr);
            return 0;
        }
        std::string ret = NBG::getPlatformUID();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "NBG:getPlatformUID",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_nbg_NBG_getPlatformUID'.",&tolua_err);
#endif
    return 0;
}
static int lua_nbg_NBG_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (NBG)");
    return 0;
}

int lua_register_nbg_NBG(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NBG");
    tolua_cclass(tolua_S,"NBG","NBG","",nullptr);

    tolua_beginmodule(tolua_S,"NBG");
        tolua_function(tolua_S,"getOpenUDID", lua_nbg_NBG_getOpenUDID);
        tolua_function(tolua_S,"getLanguage", lua_nbg_NBG_getLanguage);
        tolua_function(tolua_S,"getCurrentSecond", lua_nbg_NBG_getCurrentSecond);
        tolua_function(tolua_S,"getCurrentMillisecond", lua_nbg_NBG_getCurrentMillisecond);
        tolua_function(tolua_S,"getPublishRegion", lua_nbg_NBG_getPublishRegion);
        tolua_function(tolua_S,"getUDID", lua_nbg_NBG_getUDID);
        tolua_function(tolua_S,"get_COCOS2D_DEBUG", lua_nbg_NBG_get_COCOS2D_DEBUG);
        tolua_function(tolua_S,"md5", lua_nbg_NBG_md5);
        tolua_function(tolua_S,"getVersionCode", lua_nbg_NBG_getVersionCode);
        tolua_function(tolua_S,"getCountryCode", lua_nbg_NBG_getCountryCode);
        tolua_function(tolua_S,"getPlatformToken", lua_nbg_NBG_getPlatformToken);
        tolua_function(tolua_S,"getHandSetInfo", lua_nbg_NBG_getHandSetInfo);
        tolua_function(tolua_S,"getVersionName", lua_nbg_NBG_getVersionName);
        tolua_function(tolua_S,"getPlatformUID", lua_nbg_NBG_getPlatformUID);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NBG).name();
    g_luaType[typeName] = "NBG";
    g_typeCast["NBG"] = "NBG";
    return 1;
}
TOLUA_API int register_all_nbg(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"nbg",0);
	tolua_beginmodule(tolua_S,"nbg");

	lua_register_nbg_LuaCCBNode(tolua_S);
	lua_register_nbg_NBG(tolua_S);
	lua_register_nbg_NBGNetManager(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

