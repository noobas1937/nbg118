//
//  CCShake.h
//  IF
//
//  Created by zhaohf on 13-10-10.
//
//

//使用实例
//CCShake *t = CCShake::create(1, 10);
//this->runAction(t);

#ifndef __SHAKE_H__
#define __SHAKE_H__

#include "cocos2d.h"

class CCShake : public cocos2d::CCActionInterval
{
public:
    CCShake();
    
    // Create the action with a time and a strength (same in x and y)
    static CCShake* create(float d, float strength );
    // Create the action with a time and strengths (different in x and y)
    static CCShake* createWithStrength(float d, float strength_x, float strength_y );
    bool initWithDuration(float d, float strength_x, float strength_y );
    
protected:
    
    void startWithTarget(cocos2d::CCNode *pTarget);
    void update(float time);
    void stop(void);
    
    
    // Initial position of the shaked node
    float m_initial_x, m_initial_y;
    // Strength of the action
    float m_strength_x, m_strength_y;
};

#endif //__SHAKE_H__
