//
//  CCTouchNode.cpp
//  IF
//
//  Created by 邹 程 on 13-9-12.
//
//

#include "CCTouchNode.h"

NS_CC_EXT_BEGIN

bool CCTouchNode::init(float xAxis, float yAxis) {
    m_xAxis = xAxis;
    m_yAxis = yAxis;
    return true;
};

bool CCTouchNode::touchedInside(cocos2d::CCTouch *pTouch) {
    if (!this->isVisible() || !this->getParent()) {
        return false;
    }
    CCPoint touchP = ccpSub(this->getPosition(),this->getParent()->convertToNodeSpace(pTouch->getLocation())) ;
    float px = fabsf(touchP.x);
    float py = fabsf(touchP.y);
    float semiXAxis = (m_xAxis/2)*this->getScaleX();
    float semiYAxis = (m_yAxis/2)*this->getScaleY();
    if (px > semiXAxis || py > semiYAxis || py > ((semiYAxis/semiXAxis)*(semiXAxis - px))) {
        return false;
    }
    return true;
};

NS_CC_EXT_END