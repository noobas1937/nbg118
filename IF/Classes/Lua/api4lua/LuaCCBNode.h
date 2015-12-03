//
//  LuaCCBNode.h
//  nbg
//
//  Created by guojiang on 8/31/15.
//
//

#ifndef __nbg__LuaCCBNode__
#define __nbg__LuaCCBNode__

#include "NBGNode.h"
#include "api4lua_support.h"

class LuaNodeResourceInfo
: public BANodeResourceInfo {
public:
    LuaNodeResourceInfo()
    : luaHandler_memberVarSetter(0)
    {}
    virtual ~LuaNodeResourceInfo() {}
    
    // lua
    CXLUAFUNC luaHandler_memberVarSetter; // (const char * pMemberVariableName, Node * node)
};

class LuaCCBNode : public NBGNode
{
public:
    static LuaCCBNode* create(const char* ccbi, CXLUAFUNC memberVarSetter);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void beforeLoadNodeResource(const BANodeResourceInfo* nri);
    
    // onEnter
    void setLuaHandlerOnEnter(CXLUAFUNC luaHandler) { luaHandler_onEnter = luaHandler; }
    // onEnter
    void setLuaHandlerOnExit(CXLUAFUNC luaHandler) { luaHandler_onExit = luaHandler; }
    // 设置点击回调函数
    // luaHandler_onTouchClick(Node *target, Touch *touch)
    void setLuaHandlerOnTouchClick(CXLUAFUNC luaHandler) { luaHandler_onTouchClick = luaHandler; }
    // 设置动画完成回调函数
    void setLuaHandlerAnimationCallback(CXLUAFUNC f);
    
protected:
    LuaCCBNode();
    
    CXLUAFUNC luaHandler_onEnter;
    CXLUAFUNC luaHandler_onExit;
};

#endif /* defined(__nbg__LuaCCBNode__) */
