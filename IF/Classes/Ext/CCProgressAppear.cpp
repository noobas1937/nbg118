//
//  CCProgressAppear.cpp
//  IF
//
//  Created by 邹 程 on 13-10-17.
//
//

#include "CCProgressAppear.h"
//#include "cocoa/CCZone.h"
#include "CCClipNode.h"

NS_CC_BEGIN

CCProgressAppear* CCProgressAppear::create(float fDuration, ClipProgressDirection direction,float part)
{
    CCProgressAppear *pRet = new CCProgressAppear();
    pRet->initWithDirection(fDuration,direction, part);
    pRet->autorelease();
    
    return pRet;
}

bool CCProgressAppear::initWithDirection(float fDuration, ClipProgressDirection direction,float part)
{
    if (CCActionInterval::initWithDuration(fDuration))
    {
        m_direction = direction;
        m_part = part;
        m_originX = 0.0;
        m_originY = 0.0;
        return true;
    }
    
    return false;
}

//CCObject* CCProgressAppear::copyWithZone(CCZone *pZone)
//{
//    CCZone* pNewZone = NULL;
//    CCProgressAppear* pCopy = NULL;
//    if(pZone && pZone->m_pCopyObject)
//    {
//        //in case of being called at sub class
//        pCopy = (CCProgressAppear*)(pZone->m_pCopyObject);
//    }
//    else
//    {
//        pCopy = new CCProgressAppear();
//        pZone = pNewZone = new CCZone(pCopy);
//    }
//    
//    CCActionInterval::copyWithZone(pZone);
//    
//    pCopy->initWithDirection(m_fDuration, m_direction,m_part);
//    
//    CC_SAFE_DELETE(pNewZone);
//    return pCopy;
//}

void CCProgressAppear::startWithTarget(CCNode *pTarget)
{
    CCSize targetSize = pTarget->getContentSize();
    CCAssert(dynamic_cast<extension::CCClipSprite*>(pTarget),"This only for CCClipSprite!");
    CCActionInterval::startWithTarget(pTarget);
    ((extension::CCClipSprite*)_target)->setProgressDirection(m_direction);
    if (m_part < 1.0) {
        m_originX = ((extension::CCClipSprite*)_target)->getProgressX();
        m_originY = ((extension::CCClipSprite*)_target)->getProgressY();
    }
}

void CCProgressAppear::update(float time)
{
    // time 0 to 1
    if (_target)
    {
        if (abs(m_direction) == 1) {
            ((extension::CCClipSprite*)_target)->setProgressX(time*m_part + m_originX);
        } else {
            ((extension::CCClipSprite*)_target)->setProgressY(time*m_part + m_originY);
        }
    }
}

CCActionInterval* CCProgressAppear::reverse(void)
{
    return CCProgressAppear::create(_duration, (ClipProgressDirection)(-(int)m_direction));
}

NS_CC_END