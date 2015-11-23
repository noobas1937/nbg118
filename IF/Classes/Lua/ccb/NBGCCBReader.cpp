//
//  NBGCCBReader.cpp
//  nbg
//
//  Created by guojiang on 4/29/15.
//
//

#include "NBGCCBReader.h"
#include "NBGNode.h"
#include "CCLuaEngine.h"

#define CCBRESOURCES_BASE_PATH "2d/"
using namespace cocosbuilder;

NBGCCBReader::NBGCCBReader()
: m_pCCBNode(nullptr)
, m_pAnimationManager(nullptr)
, assignMemVarCallback(nullptr)
, animCompletedCallback(nullptr)
, luaHandler_memberVarSetter(0)
, luaHandler_animationCallback(0)
{
    
}

NBGCCBReader::~NBGCCBReader()
{
    releaseMembers();

    assignMemVarCallback = nullptr;
    animCompletedCallback = nullptr;
}

void NBGCCBReader::readCCB(const char * ccbiName)
{
    releaseMembers();
    
    /* Create an autorelease NodeLoaderLibrary. */
    NodeLoaderLibrary* ccNodeLoaderLibrary = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    
    /* Create an autorelease CCBReader. */
    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->setCCBRootPath(CCBRESOURCES_BASE_PATH);
    ccbReader->autorelease();
    
    /* Read a ccbi file. */
    m_pCCBNode = ccbReader->readNodeGraphFromFile(ccbiName, this);
    if (m_pCCBNode != nullptr)
    {
        m_pAnimationManager = ccbReader->getAnimationManager();
        m_pAnimationManager->setDelegate(this);
    }
    CC_SAFE_RETAIN(m_pCCBNode);
    CC_SAFE_RETAIN(m_pAnimationManager);
}

bool NBGCCBReader::onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * node)
{
    if (pTarget == this)
    {
        if (luaHandler_memberVarSetter > 0)
        {
            auto engine = LuaEngine::getInstance();
            LuaStack* stack = engine->getLuaStack();
            stack->pushString(pMemberVariableName);
            stack->pushObject(node, "cc.Node");
            stack->executeFunctionByHandler(luaHandler_memberVarSetter, 2);
            stack->clean();
        }
        
        if (assignMemVarCallback != nullptr)
        {
            return assignMemVarCallback(pTarget, pMemberVariableName, node);
        }
    }
    return false;
}

void NBGCCBReader::completedAnimationSequenceNamed(const char *name)
{
    if (luaHandler_animationCallback > 0)
    {
        auto engine = LuaEngine::getInstance();
        LuaStack* stack = engine->getLuaStack();
        stack->pushString(name);
        stack->executeFunctionByHandler(luaHandler_animationCallback, 1);
        stack->clean();
    }
    
    if (animCompletedCallback != nullptr)
    {
        animCompletedCallback(name);
    }
}

void NBGCCBReader::releaseMembers()
{
    CC_SAFE_RELEASE_NULL(m_pCCBNode);
    if (m_pAnimationManager) m_pAnimationManager->setDelegate(nullptr);
    CC_SAFE_RELEASE_NULL(m_pAnimationManager);
}
