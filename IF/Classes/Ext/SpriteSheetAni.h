//
//  SpriteSheetAni.h
//  IF
//
//  Created by ganxiaohua on 13-8-29.
//
//

#ifndef __IF__SpriteSheetAni__
#define __IF__SpriteSheetAni__

#include "CommonInclude.h"

class SpriteSheetAni: public cocos2d::CCSprite
{
public:
    SpriteSheetAni(const char* aniName,int loop,bool endRemove,int type,float delayPerUnit,int totalFrame):
    mAniName(aniName),
    mendRemove(endRemove),
    mtype(type),
    mdelayPerUnit(delayPerUnit),
    mLoop(loop),
    manimationEnd(NULL),
    marrayAF(NULL),
    manimation(NULL),
    m_callbackTarget(NULL),
    m_callbackFunc(NULL),
    mtotalFrame(totalFrame){};
    ~SpriteSheetAni(){};
    bool init();
    void play(float delayTime=0);
    CC_SYNTHESIZE(bool, mendRemove, EndRemove);//结束是否移除
    CC_SYNTHESIZE(int, mLoop, Loop);//循环次数
    CC_SYNTHESIZE(const char* , mAniName, AniName);//动画资源的名字，如（troop_1.png,troop_2.png,troop_3.png,写成troop_即可）
    CC_SYNTHESIZE(int , mtype, Type);//资源类型
    CC_SYNTHESIZE(float , mdelayPerUnit, DelayPerUnit);//设置每帧的时间
    CC_SYNTHESIZE(int , mtotalFrame, TotalFrame);//总帧数
    CC_SYNTHESIZE(cocos2d::CCFiniteTimeAction* , manimationEnd, AnimationEnd);
    CC_SYNTHESIZE(cocos2d::CCAnimation * , manimation, Animation);
    CC_SYNTHESIZE(cocos2d::CCArray* , marrayAF, ArrayAF);
    
    static SpriteSheetAni* create(const char* aniName,int loop=1,bool endRemove=true,int type=0,float delayPerUnit=0.1,int totalFrame=99);
   // void setAF(cocos2d::CCArray* arrayAF);
    void setCallback(CCObject *target,SEL_CallFunc func);
    void callback();
    virtual void onExit();
protected:
    void onAnimationEnd(cocos2d::CCNode* node);
    void createAnimation();
    
    CCObject *m_callbackTarget;
    SEL_CallFunc m_callbackFunc;
private:
    
};



#endif /* defined(__IF__SpriteSheetAni__) */
