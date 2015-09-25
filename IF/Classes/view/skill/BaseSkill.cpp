//
//  BaseSkill.cpp
//  IF
//
//  Created by 邹 程 on 13-9-23.
//
//

#include "BaseSkill.h"

void BaseSkill::remove() {
    this->removeFromParentAndCleanup(true);
    this->battleCallback();
}

void BaseSkill::battleCallback() {
    if (m_callbackTarget && m_callbackFunc) {
        (m_callbackTarget->*m_callbackFunc)();
    }
}

void BaseSkill::setCallback(cocos2d::CCObject *target, SEL_CallFunc func) {
    m_callbackTarget = target;
    m_callbackFunc = func;
}