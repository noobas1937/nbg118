//
//  CCProgressAppear.h
//  IF
//
//  Created by 邹 程 on 13-10-17.
//
//

#ifndef __IF__CCProgressAppear__
#define __IF__CCProgressAppear__

#include "cocos2d.h"

enum ClipProgressDirection{
    NONE_DIRECTION = 0
    ,WEST_TO_EAST = 1
    ,EAST_TO_WEST = -1
    ,SOUTH_TO_NORTH = 2
    ,NORTH_TO_SOUTH = -2
};

NS_CC_BEGIN

class CC_DLL CCProgressAppear : public CCActionInterval
{
public:
    
    static CCProgressAppear* create(float fDuration, ClipProgressDirection direction,float part = 1.0);
    
    bool initWithDirection(float fDuration,ClipProgressDirection direction,float part);
//    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual CCActionInterval* reverse(void);
    
protected:
    ClipProgressDirection m_direction;
    float m_part;
    float m_originX;
    float m_originY;
};

NS_CC_END

#endif /* defined(__IF__CCProgressAppear__) */
