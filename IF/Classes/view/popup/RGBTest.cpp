//
//  RGBTest.cpp
//  IF
//
//  Created by 李锐奇 on 15/2/9.
//
//

#include "RGBTest.h"
#include "CCSliderBar.h"

RGBTest *RGBTest::create(std::string pic1, std::string pic2){
    RGBTest *ret = new RGBTest(pic1, pic2);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool RGBTest::init(){
    auto size = CCDirector::sharedDirector()->getWinSize();
    m_picNode = CCNode::create();
    this->addChild(m_picNode);
    m_picNode->setPosition(ccp(320, size.height - 200));
    
    m_picNode1 = CCNode::create();
    this->addChild(m_picNode1);
    m_picNode1->setPosition(ccp(320, size.height - 360));

    m_sliderNode = CCNode::create();
    this->addChild(m_sliderNode);
    m_sliderNode->setPosition(ccp(320, size.height - 460));
    
    auto addSliderBar = [](CCNode *m_parent, CCNode *node, int tag, float startValue, float showValue, float &posY, std::string des){
        auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
        m_sliderBg->setInsetBottom(5);
        m_sliderBg->setInsetLeft(5);
        m_sliderBg->setInsetRight(5);
        m_sliderBg->setInsetTop(5);
        m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
        m_sliderBg->setPosition(ccp(252/2, 25));
        m_sliderBg->setContentSize(CCSize(252,18));
        
        auto bgSp = CCLoadSprite::createSprite("huadongtiao4.png");
        bgSp->setVisible(false);
        auto proSp = CCLoadSprite::createSprite("huadongtiao4.png");
        auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
        
        auto slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
        slider->setMinimumValue(0.0f);
        slider->setMaximumValue(1.0f);
        slider->setProgressScaleX(248/proSp->getContentSize().width);
        slider->setTag(tag);
        slider->setLimitMoveValue(25);
        slider->setTouchPriority(Touch_Default);
        slider->addTargetWithActionForControlEvents(node, cccontrol_selector(RGBTest::valueChange), CCControlEventValueChanged);
        auto label = CCLabelIF::create();
        label->setTag(10 + tag);
        
        std::string str = CC_ITOA_2(showValue);
        label->setString(str);
        auto desLabel = CCLabelIF::create(des.c_str());

        m_parent->addChild(slider);
        m_parent->addChild(label);
        m_parent->addChild(desLabel);
        
        slider->setPosition(ccp(0, posY));
        label->setPosition(ccp(290, posY));
        desLabel->setPosition(ccp(0, posY));

        label->setAnchorPoint(ccp(1, 0.5));
        desLabel->setAnchorPoint(ccp(0, 0.5));
        
        slider->setValue(startValue);
        posY -= 60;
    };
    float posY = 0;
    addSliderBar(m_sliderNode, this, COLOR_H, 0.0, getVaue(0.0, COLOR_H), posY, "H");
    addSliderBar(m_sliderNode, this, COLOR_S, 1.0, getVaue(1.0, COLOR_S), posY, "S");
    addSliderBar(m_sliderNode, this, COLOR_B, 1.0, getVaue(1.0, COLOR_B), posY, "B");
    addSliderBar(m_sliderNode, this, ADD_B, 0.5, getVaue(0.5, ADD_B), posY, "亮度");
    addSliderBar(m_sliderNode, this, ADD_C, 0.1, getVaue(0.1, ADD_C), posY, "对比度");
    addSliderBar(m_sliderNode, this, ADD_S, 0.1, getVaue(0.1, ADD_S), posY, "饱和度");

    auto sprite = CCLoadSprite::createSprite(m_pic2.c_str());
    m_picNode1->addChild(sprite);
    
    refreshView();
    return true;
}

void RGBTest::refreshView(){
    m_picNode->removeAllChildren();

    auto color = CCCommonUtils::covertHSBToRGB(m_h, m_s, m_b);
    auto sp = CCCommonUtils::addFilterSprite(m_pic1, color, a_b, a_c, a_s);
    m_picNode->addChild(sp);
}

void RGBTest::valueChange(CCObject * pSender, Control::EventType pCCControlEvent)
{
    CCSliderBar * slider = dynamic_cast<CCSliderBar*>(pSender);
    int tag = slider->getTag();
    float value = getValueByType(slider);
    auto label = dynamic_cast<CCLabelIF*>(m_sliderNode->getChildByTag(10 + tag));
    if(label){
        label->setString(CC_ITOA_2(value));
    }

    if(tag == COLOR_H){
        m_h = value;
    }else if(tag == COLOR_S){
        m_s = value;
    }else if(tag == COLOR_B){
        m_b = value;
    }else if(tag == ADD_B){
        a_b = value;
    }else if(tag == ADD_C){
        a_c = value;
    }else if(tag == ADD_S){
        a_s = value;
    }

    refreshView();
}

float RGBTest::getValueByType(CCSliderBar *slider){
    float result = 0.0f;
    if(slider){
        result = slider->getValue();
        int tag = slider->getTag();
        result = getVaue(result, tag);
//        if(tag == COLOR_H){
//            result = floor(result * 360);
//        }
//        
//        if(tag == ADD_B){
//            result = 2.0 * result - 1.0;
//        }
//        
//        if(tag == ADD_C){
//            result = 1.0 + result;
//        }
//        
//        if(tag == ADD_S){
//            result = 1.0 + result;
//        }
    }
    return result;
}

float RGBTest::getVaue(float value, float type){
    float result = value;
    if(type == COLOR_H){
        result = floor(result * 360);
    }
    if(type == COLOR_B){
        result = result;
    }
    
    if(type == COLOR_S){
        result = result;
    }

    if(type == ADD_B){
        result = 2.0 * result - 1.0;
    }
    
    if(type == ADD_C){
        result = result * 10;
    }
    
    if(type == ADD_S){
        result = result * 10;
    }
    return result;
}
