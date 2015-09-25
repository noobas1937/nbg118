//
//  RGBTest.h
//  IF
//
//  Created by 李锐奇 on 15/2/9.
//
//

#ifndef __IF__RGBTest__
#define __IF__RGBTest__

#include "CommonInclude.h"
#include "CCSliderBar.h"

enum RGBTESTTAG{
    COLOR_H,
    COLOR_S,
    COLOR_B,
    ADD_B,
    ADD_C,
    ADD_S,
};

inline const char* CC_ITOA_2(double v){return CCString::createWithFormat("%.2f", v)->getCString();}

class RGBTest : public CCNode{
public:
    static RGBTest *create(std::string pic1, std::string pic2);
private:
    RGBTest(std::string pic1, std::string pic2) :
    m_pic1(pic1)
    , m_pic2(pic2)
    , m_h(0)
    , m_s(1.0)
    , m_b(1.0)
    , a_b(0.0)
    , a_c(1.0)
    , a_s(1.0){};
    
    virtual bool init();
    void refreshView();
    void valueChange(CCObject * pSender, Control::EventType pCCControlEvent);
    float getValueByType(CCSliderBar *slider);
    float getVaue(float value, float type);
    
    float m_h;
    float m_s;
    float m_b;
    float a_b;
    float a_c;
    float a_s;
    std::string m_pic1;
    std::string m_pic2;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_sliderNode;
};

#endif /* defined(__IF__RGBTest__) */
