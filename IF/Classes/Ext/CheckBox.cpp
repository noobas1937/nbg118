//
//  CheckBox.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-10.
//
//

#include "CheckBox.h"
#include "CCSafeNotificationCenter.h"

void CheckBox::onEnter(){
    CCNode::onEnter();
}

void CheckBox::onExit(){
    CCNode::onExit();
}

CheckBox* CheckBox::create(bool state,string bgStr){
    CheckBox* ret = new CheckBox(state,bgStr);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void CheckBox::setBtnTouchPriority(int p){
    m_Btn->setTouchPriority(p);
}
bool CheckBox::init(){
    m_Btn = CCControlButton::create(CCLoadSprite::createScale9Sprite(m_bgStr.c_str()));
    m_Btn->addTargetWithActionForControlEvents(this, cccontrol_selector(CheckBox::onBtnClick), CCControlEventTouchUpInside);
    m_Btn->setAdjustBackgroundImage(false);
    m_selectIcon = CCLoadSprite::createSprite("green_yes.png");
    this->addChild(m_Btn);
    this->addChild(m_selectIcon);
    m_selectIcon->setVisible(m_select);
    return true;
}

void CheckBox::onBtnClick(cocos2d::CCObject *pSender, CCControlEvent evt){
    if(!m_canSelect && !m_select){
        return;
    }
    setSelect(!m_select);
}

void CheckBox::setCanSelect(bool b){
    this->m_canSelect = b;
}

void CheckBox::setSelect(bool b){
    m_select = b;
    m_selectIcon->setVisible(m_select);
    if(m_select){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHECK_BOX_SELECT, this);
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHECK_BOX_UNSELECT, this);
    }
}

bool CheckBox::isSelected(){
    return m_select;
}