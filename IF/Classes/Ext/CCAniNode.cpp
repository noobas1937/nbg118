//
//  CCAniNode.cpp
//  IF
//
//  Created by fubin on 14-6-6.
//
//

#include "CCAniNode.h"

NS_CC_EXT_BEGIN

cocosbuilder::CCBAnimationManager* CCAniNode::getAnimationManager()
{
    return mActionManager;
}

void CCAniNode::setAnimationManager(cocosbuilder::CCBAnimationManager *pAnimationManager)
{
    mActionManager = pAnimationManager;
}

NS_CC_EXT_END