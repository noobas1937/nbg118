//
//  GongJian2.h
//  IF
//
//  Created by ganxiaohua on 14-6-26.
//
//

#ifndef __IF__GongJian2__
#define __IF__GongJian2__

#include "BaseSkill.h"
#include "SpriteSheetAni.h"
#include "CCMathUtils.h"

class GongJian2 : public BaseSkill{
public:
    virtual bool init();
    static GongJian2* create(CCNode * batchNode,CCPoint startPoint,CCPoint endPoint,int side, string pic, float scale = 0.6, float speed = 400.0);
    ~GongJian2(){
        
    }
protected:
    GongJian2(CCNode * batchNode,CCPoint startPoint,CCPoint endPoint,int side, string pic, float scale, float speed) : m_batchNode(batchNode), mScale(scale), mSpeed(speed)
    ,m_startPoint(startPoint)
    ,m_endPoint(endPoint)
    ,m_attSide(side)
    ,ani(NULL)
    ,m_num(0)
    ,m_duration(0)
    ,m_pic(pic)
    {
        
    };
    void update(float time);
    void removeJian();
    
    int m_delayTime;
    int m_attSide;
    CCPoint m_startPoint;
    CCPoint m_endPoint;
    int m_num;
    float m_duration;
    string m_pic;
    float subX;
    float subY;
    //begin a by ljf
    float mScale;
    float mSpeed;
    //end a by ljf
    SpriteSheetAni* ani;
    CC_SYNTHESIZE(CCNode*, m_batchNode, BatchNode);
    CC_SYNTHESIZE(CCSprite*, m_jian, Jian);
};

#endif /* defined(__IF__GongJian2__) */
