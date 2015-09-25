//
//  ArrowAni.h
//  IF
//
//  Created by ganxiaohua on 13-11-8.
//
//

#ifndef __IF__ArrowAni__
#define __IF__ArrowAni__
#include "CommonInclude.h"

class ArrowAni: public CCNode
{
public:
    ArrowAni(float delayPerUnit=0.1):m_pos(CCPoint()),m_direction(1),delayPerUnit(delayPerUnit){};
    ~ArrowAni(){};
    bool init();
    void play(CCPoint pos, int w, int h, int d = 1);
    void stop();
    static ArrowAni* create(float delayPerUnit=0.1);
protected:
    CCPoint m_pos;
    int  m_direction;
    float delayPerUnit;
    CCSafeObject<CCSprite> m_arrow;
    CCSafeObject<CCScale9Sprite> m_rect;
private:
//    virtual void keyBackClicked(){}
    virtual bool isClickSwallowed(){return true;}
    virtual int getPriority(){return 0;}
};
#endif /* defined(__IF__ArrowAni__) */


