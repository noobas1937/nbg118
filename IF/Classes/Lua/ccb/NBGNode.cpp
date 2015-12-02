//
//  NBGNode.cpp
//  nbg
//
//  Created by guojiang on 4/29/15.
//
//

#include "NBGNode.h"

#pragma mark -

NBGNode::NBGNode()
: m_delegate(nullptr)
, m_pNBGCCBReader(nullptr)
, m_pNode(nullptr)
{
    m_labelTxtMap.clear();
}

NBGNode::~NBGNode()
{
    m_labelTxtMap.clear();
    
    setAnimationDelegate(nullptr);
    if (m_pNBGCCBReader) m_pNBGCCBReader->releaseMembers();
    CC_SAFE_RELEASE_NULL(m_pNBGCCBReader);
    CC_SAFE_RELEASE_NULL(m_pNode);
}

void NBGNode::loadNodeResource(const char* resFilePath, const BANodeResourceInfo* nri, BANodeResourceType nrt)
{
    if (nrt == BANodeResourceType_CCBI)
    {
        loadCCB(resFilePath, nri);
    }
    else if (nrt == BANodeResourceType_JSON)
    {
        beforeLoadNodeResource(nri);
        loadJSON(nri);
        afterLoadNodeResource();
    }
    else if (nrt == BANodeResourceType_MANUAL)
    {
        beforeLoadNodeResource(nri);
        createChildrenManual();
        afterLoadNodeResource();
    }
}

void NBGNode::setAnimationDelegate(IBA2CCBAnimationDelegate *pDelegate)
{
    CC_SAFE_RELEASE(dynamic_cast<Ref*>(m_delegate));
    m_delegate = pDelegate;
    CC_SAFE_RETAIN(dynamic_cast<Ref*>(m_delegate));
}

void NBGNode::playAnimation(const char* animationName)
{
    if (m_pNBGCCBReader && m_pNBGCCBReader->getAnimationManager())
    {
        m_pNBGCCBReader->getAnimationManager()->runAnimationsForSequenceNamed(animationName);
    }
}

void NBGNode::playAnimation(const char* animationName, float fTweenDuration)
{
    if (m_pNBGCCBReader && m_pNBGCCBReader->getAnimationManager())
    {
        m_pNBGCCBReader->getAnimationManager()->runAnimationsForSequenceNamedTweenDuration(animationName, fTweenDuration);
    }
}

void NBGNode::loadCCB(const char* resFilePath, const BANodeResourceInfo* nri)
{
    if (m_pNBGCCBReader == nullptr)
    {
        m_pNBGCCBReader = new NBGCCBReader();
        m_pNBGCCBReader->animCompletedCallback = [this](const char *animationSequenceName){
            if (m_delegate) m_delegate->onAnimationCompleted(this, animationSequenceName);
        };
    }
    if (nri != nullptr)
    {
        beforeLoadNodeResource(nri);
    }
    
    m_pNBGCCBReader->readCCB(resFilePath);
    if (m_pNode && m_pNode->getParent()) m_pNode->removeFromParent();
    CC_SAFE_RELEASE_NULL(m_pNode);
    m_pNode = m_pNBGCCBReader->getNode();
    CC_SAFE_RETAIN(m_pNode);
    if (m_pNode)
    {
        setAnchorPoint(m_pNode->getAnchorPoint());
        setContentSize(m_pNode->getContentSize());
        addChild(m_pNode);
    }
    
    afterLoadNodeResource();
}
