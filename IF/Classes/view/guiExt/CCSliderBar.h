//
//  CCSliderBar.h
//  IF
//
//  Created by ganxiaohua on 14-6-23.
//
//

#ifndef __IF__CCSliderBar__
#define __IF__CCSliderBar__

#include "CommonInclude.h"

class CCSliderBar : public ControlSlider
{
public:
    CCSliderBar(void){};
    ~CCSliderBar(void){};
    static CCSliderBar* createSlider(CCScale9Sprite* backgroundSprite, CCSprite* pogressSprite, CCSprite* thumbSprite);
    virtual void needsLayout();
    bool onTouchBegan(CCTouch* touch, CCEvent* pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void setProgressScaleX(float scaleX);
    
    bool m_touchEnd;
protected:
    virtual float valueForLocation(Vec2 location);
    float m_maxScaleX;
    float m_bgWidth;
};
#endif /* defined(__IF__CCSliderBar__) */
