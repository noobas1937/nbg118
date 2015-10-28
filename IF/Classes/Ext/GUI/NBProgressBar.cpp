//
//  NBProgressBar.cpp
//  clipTest
//
//  Created by fusheng on 15-10-8.
//
//

#include "NBProgressBar.h"
USING_NS_CC;

NBProgressBar* NBProgressBar::create(cocos2d::Node *pProgressBar, cocos2d::Node *pMaskNode, float percent,float offsetX ,float alphaThreshold,float minMNValue ,float maxMNValue)
{
    NBProgressBar* ret = new NBProgressBar();
    if(ret && ret->init(pProgressBar,pMaskNode,percent,offsetX,alphaThreshold,minMNValue,maxMNValue)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NBProgressBar::init(cocos2d::Node *pProgressBar, cocos2d::Node *pMaskNode, float percent,float offsetX ,float alphaThreshold,float minMNValue ,float maxMNValue)
{
    m_pClippingNode = ClippingNode::create();
    
    m_pClippingNode->setAnchorPoint(Point::ZERO);
    
    m_percent = percent;
    
    m_pProgressBar = pProgressBar;
    
    m_pMaskNode = pMaskNode;
    
    m_pProgressBar->retain();
    
    m_pMaskNode->retain();
    
    if (m_pProgressBar->getParent()!= NULL) {
        m_pProgressBar->removeFromParent();
    }
    
    if (m_pMaskNode->getParent()!= NULL) {
        m_pMaskNode->removeFromParent();
    }
    
    
    m_pProgressBar->setAnchorPoint(m_pClippingNode->getAnchorPoint());
    
    m_pMaskNode->setAnchorPoint(m_pClippingNode->getAnchorPoint());
    
    m_pMaskNode->setPosition(Vec2::ZERO);
    
    m_pProgressBar->setPosition(Vec2::ZERO);
    
    m_pClippingNode->setStencil(m_pMaskNode);
    
    m_pClippingNode->setInverted(true);
    
    m_pClippingNode->setAlphaThreshold(alphaThreshold);
    
    m_pClippingNode->addChild(m_pProgressBar);
    
    this->addChild(m_pClippingNode, 1);
    
    m_pProgressBar->release();
    
    m_pMaskNode->release();
    
    m_minMNValue = minMNValue == -10086? m_pProgressBar->getPositionX():minMNValue;
    
    m_maxMNValue = maxMNValue == -10086? m_pProgressBar->getPositionX()+m_pProgressBar->getContentSize().width:maxMNValue;
    
    
    this->offsetX = offsetX;
    
    
    refresh();
    
    return true;
}

void NBProgressBar::setPercent(float percent)
{
    if(percent< 0)
        percent = 0;
    if (percent> 1 ) {
        percent = 1;
    }
    m_percent = percent;

    refresh();
}

float NBProgressBar::getPercent()
{
    return  m_percent;
}
void NBProgressBar::refresh()
{
//    CCLOG("nbpb percent : %f" , m_percent);
    m_pMaskNode->setPositionX(m_minMNValue + (m_maxMNValue-m_minMNValue)* m_percent + offsetX);
}