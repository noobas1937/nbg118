//
//  LuaCCBNode.cpp
//  nbg
//
//  Created by guojiang on 8/31/15.
//
//

#include "LuaCCBNode.h"
#include "CCLuaEngine.h"

LuaCCBNode* LuaCCBNode::create(const char* ccbi, CXLUAFUNC memberVarSetter)
{
    LuaCCBNode *pRet = new LuaCCBNode();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        LuaNodeResourceInfo rimvn;
        rimvn.luaHandler_memberVarSetter = memberVarSetter;
        pRet->loadNodeResource(ccbi, &rimvn);
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

LuaCCBNode::LuaCCBNode()
: luaHandler_onEnter(0)
, luaHandler_onExit(0)
{
    
}

void LuaCCBNode::onEnter()
{
    NBGNode::onEnter();
    if (luaHandler_onEnter > 0)
    {
        auto engine = LuaEngine::getInstance();
        LuaStack* stack = engine->getLuaStack();
        stack->executeFunctionByHandler(luaHandler_onEnter, 0);
        stack->clean();
    }
}

void LuaCCBNode::onExit()
{
    NBGNode::onExit();
    if (luaHandler_onExit > 0)
    {
        auto engine = LuaEngine::getInstance();
        LuaStack* stack = engine->getLuaStack();
        stack->executeFunctionByHandler(luaHandler_onExit, 0);
        stack->clean();
    }
}

void LuaCCBNode::beforeLoadNodeResource(const BANodeResourceInfo* nri)
{
    if (nri == nullptr) return;
    const LuaNodeResourceInfo* lnri = dynamic_cast<const LuaNodeResourceInfo*>(nri);
    if (lnri == nullptr) return;
    
    m_pNBGCCBReader->luaHandler_memberVarSetter = lnri->luaHandler_memberVarSetter;
}

void LuaCCBNode::setLuaHandlerAnimationCallback(CXLUAFUNC f)
{
    if (m_pNBGCCBReader) m_pNBGCCBReader->luaHandler_animationCallback = f;
}