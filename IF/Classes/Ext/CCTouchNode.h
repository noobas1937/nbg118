//
//  CCTouchNode.h
//  IF
//
//  Created by 邹 程 on 13-9-12.
//
//

#ifndef __IF__CCTouchNode__
#define __IF__CCTouchNode__

#include "cocos2d.h"
#include "cocos-ext.h"

NS_CC_EXT_BEGIN

class CCTouchNode : public cocos2d::CCNode {
public:
    bool init(float xAxis = 0.0,float yAxis = 0.0);
    
    virtual ~CCTouchNode(){}
    bool touchedInside(cocos2d::CCTouch *pTouch);
protected:
    CC_SYNTHESIZE(float, m_xAxis, XAxis);
    CC_SYNTHESIZE(float, m_yAxis, YAxis);
};
NS_CC_EXT_END
#endif /* defined(__IF__CCTouchNode__) */
