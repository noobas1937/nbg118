//
//  ParticleFireAni.h
//  IF
//
//  Created by ganxiaohua on 14-8-15.
//
//

#ifndef __IF__ParticleFireAni__
#define __IF__ParticleFireAni__

#include "CommonInclude.h"


class ParticleFireAni : public cocos2d::Layer
 
{
public:
    ParticleFireAni(){};
    virtual ~ParticleFireAni(){}
    
    static ParticleFireAni* create();
    
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_partileNode;
    CCSafeObject<CCNode> m_clickParNode;
    CCSafeObject<CCScale9Sprite> m_clickNode;
    unsigned long m_touchTime;
    bool m_first;
};
#endif /* defined(__IF__ParticleFireAni__) */
