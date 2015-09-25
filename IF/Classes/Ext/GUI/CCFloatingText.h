//
//  CCFloatingText.h
//  IF
//
//  Created by zhaohf on 13-10-09.
//
#ifndef __CCFloatingText_H__
#define __CCFloatingText_H__

#include "cocos2d.h"
#include "CCLabelIF.h"

using namespace cocos2d;

//定义飘字类型
typedef enum{
    floating_type_normal=0,   //正常
    floating_type_scale =1,   //放大字体
    floating_type_ui1 =2,   //向上飘绿字
    floating_type_ui2 =3,   //向下飘红字
    floating_type_out =4,   //向下飘红字
} FloatingTypeEnum;

/*
 示例:
 CCFloatingText* fly = CCFloatingText::create("111");
 fly->setColor(ccGREEN);
 fly->setDelay(0.5f);
 fly->setFadeOut(0.8f);
 fly->setMoveByPos(ccp(0,100));
 fly->setActionScale(2.5);
 fly->setDuration(0.9);
 fly->setFloatingType(FloatingTypeEnum::floating_type_scale);
 fly->setPosition(ccp(500,500));
 addChild(fly);
 fly->startAnimation();
 */
class CCFloatingText : public CCSprite
{
private:
    const char* m_curValueStr; //当前串值
    int m_fontSize; //当前串值
    CCLabelIF *m_label;
public:
    static CCFloatingText* create(int num, bool isSign);  //数字 带符号
    static CCFloatingText* create(const char* str,int fontSize=32);
    CCFloatingText();
    ~CCFloatingText();
    
    bool initWithString(const char* str);
    void startAnimation();
    void removeObject();
    
    void setColor(const ccColor3B &color3);
    void setCurValueStr(const char* curValue);
    void drawString(const char* curValue);
    void stringf(std::string& stgt, const char *sformat, ... );
    CC_SYNTHESIZE(float, m_duration, Duration);
    //缩放
    CC_SYNTHESIZE(float, m_scale, ActionScale);
    //渐隐
    CC_SYNTHESIZE(float, m_fadeout, FadeOut);
    //延迟
    CC_SYNTHESIZE(float, m_delay, Delay);
    //移动点设置
    CC_SYNTHESIZE(CCPoint, m_movepos, MoveByPos);
    //飘字类型
    CC_SYNTHESIZE(FloatingTypeEnum, m_numbertype, FloatingType);
    
    //用于允许 "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CCFloatingText);
};

#endif