//
//  LQFlyNumber.cpp
//  IF
//
//  Created by zhaohf on 13-10-09.
//

#include "CCFloatingText.h"

CCFloatingText* CCFloatingText::create(int num, bool isSign)
{
    std::string str;
    const char* sign = "";
    if(isSign){
        sign = (num>0) ? "+" : "-";
    }
    
    CCFloatingText* numsprite = CCFloatingText::create();
    numsprite->stringf(str, "%s%d", sign,  num);
    numsprite->initWithString(str.c_str());
    return numsprite;
}

CCFloatingText* CCFloatingText::create(const char* str,int fontSize)
{
    CCFloatingText* numsprite = CCFloatingText::create();
    numsprite->m_fontSize = fontSize;
    numsprite->initWithString(str);
    return numsprite;
}

CCFloatingText::CCFloatingText():
m_label(0), m_numbertype(floating_type_normal),m_movepos(CCPoint(0, 30)),m_fadeout(0.8f),m_delay(0.5f),m_scale(1.8f),m_duration(0.5f),m_fontSize(32)
{
    
}

void CCFloatingText::setColor(const ccColor3B &color3){
    m_label->setColor(color3);
}

CCFloatingText::~CCFloatingText()
{
    //CC_SAFE_RELEASE_NULL(m_label);
}

bool CCFloatingText::initWithString(const char* str)
{
    this->setCurValueStr(str);
    return true;
}

/** sets a new curValue to the CCSprite. */
void CCFloatingText::setCurValueStr(const char* curValue)
{
    m_curValueStr = curValue;
    this->drawString(curValue);
}

void CCFloatingText::drawString(const char* str)
{
    if (m_label==NULL){
        m_label = CCLabelIF::create(m_curValueStr,m_fontSize);
        m_label->setColor(ccRED);
        m_label->setPosition(CCPointZero);
        this->addChild(m_label,1);
    }else
        m_label->setString(m_curValueStr);
}

void CCFloatingText::startAnimation()
{
    //调用changeScene函数
    switch (m_numbertype) {
        case floating_type_scale:{ //缩放
            CCFiniteTimeAction* endcall = CCCallFunc::create(this, callfunc_selector(CCFloatingText::removeObject));
            CCFiniteTimeAction* scaleby1 = CCScaleBy::create(m_duration,m_scale);
            CCFiniteTimeAction* moveby1 = CCMoveBy::create(m_duration,m_movepos);
            CCFiniteTimeAction* delay1 = CCDelayTime::create(m_delay);
            CCFiniteTimeAction* fadeout1 = CCFadeOut::create(m_fadeout);
            CCSequence* sequAct1 = CCSequence::create(delay1, fadeout1, endcall, NULL);
            CCSpawn* spAct1 = CCSpawn::create(sequAct1, moveby1, scaleby1, NULL);//
            this->runAction( spAct1 );
            break;
        }
        case floating_type_ui1:{ //
            m_label->setFntFile("Arial_Bold_Border.fnt");
            CCFiniteTimeAction* endcall = CCCallFunc::create(this, callfunc_selector(CCFloatingText::removeObject));
            CCFiniteTimeAction* moveby1 = CCMoveBy::create(0.6,ccp(0, 30));
            CCFiniteTimeAction* delay1 = CCDelayTime::create(0.3);
            CCFiniteTimeAction* fadeout1 = CCFadeOut::create(0.3);
            CCSequence* sequAct1 = CCSequence::create(delay1, fadeout1, endcall, NULL);
            CCSpawn* spAct1 = CCSpawn::create(sequAct1, moveby1, NULL);//
            this->runAction( spAct1 );
            break;
        }
        case floating_type_ui2:{ //
            CCFiniteTimeAction* endcall = CCCallFunc::create(this, callfunc_selector(CCFloatingText::removeObject));
            CCFiniteTimeAction* moveby1 = CCMoveBy::create(0.6,ccp(0, -30));
            CCFiniteTimeAction* delay1 = CCDelayTime::create(0.3);
            CCFiniteTimeAction* fadeout1 = CCFadeOut::create(0.3);
            CCSequence* sequAct1 = CCSequence::create(delay1, fadeout1, endcall, NULL);
            CCSpawn* spAct1 = CCSpawn::create(sequAct1, moveby1, NULL);//
            this->runAction( spAct1 );
            break;
        }
        case floating_type_out:{
            m_label->setFntFile("Arial_Bold_Border.fnt");
            CCFiniteTimeAction* endcall = CCCallFunc::create(this, callfunc_selector(CCFloatingText::removeObject));
            CCFiniteTimeAction* moveby1 = CCMoveBy::create(0.6,ccp(0, -30));
            CCFiniteTimeAction* delay1 = CCDelayTime::create(0.3);
            CCFiniteTimeAction* fadeout1 = CCFadeOut::create(0.3);
            CCSequence* sequAct1 = CCSequence::create(delay1, fadeout1, endcall, NULL);
            CCSpawn* spAct1 = CCSpawn::create(sequAct1, moveby1, NULL);//
            this->runAction( spAct1 );
            break;
        }
        default:{ //正常
            CCFiniteTimeAction* endcall = CCCallFunc::create(this, callfunc_selector(CCFloatingText::removeObject));
            CCFiniteTimeAction* moveby1 = CCMoveBy::create(m_duration,m_movepos);
            CCFiniteTimeAction* delay1 = CCDelayTime::create(m_delay);
            CCFiniteTimeAction* fadeout1 = CCFadeOut::create(m_fadeout);
            CCSequence* sequAct1 = CCSequence::create(delay1, fadeout1, endcall, NULL);
            CCSpawn* spAct1 = CCSpawn::create(sequAct1, moveby1, NULL);//
            this->runAction( spAct1 );
            break;
        }
    }
}

void CCFloatingText::removeObject()
{
    removeFromParentAndCleanup(true);
}

//=============================================================================
// string result;
// stringf(result, "string length is %d", 0);
// 输出结果：result = string length is 0.
void CCFloatingText::stringf(std::string& stgt, const char *sformat, ... )
{
    int size = 100;
    va_list ap;
    while (1) {
        stgt.resize(size);
        va_start(ap, sformat);
        int n = vsnprintf((char *)stgt.c_str(), size, sformat, ap);
        va_end(ap);
        if (n > -1 && n < size) {
            stgt.resize(n);
            return;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
};
