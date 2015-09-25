//
//  BaseSkill.h
//  IF
//
//  Created by 邹 程 on 13-9-23.
//
//

#ifndef __IF__BaseSkill__
#define __IF__BaseSkill__

#include "CommonInclude.h"
#include "ParticleController.h"
#include "SceneController.h"

class BaseSkill : public CCNode {
public:
    virtual void remove();
    void setCallback(CCObject *target,SEL_CallFunc func);
protected:
    BaseSkill():
    m_callbackTarget(NULL)
    ,m_callbackFunc(NULL)
    {}
    
    void battleCallback();
    
    CCObject *m_callbackTarget;
    SEL_CallFunc m_callbackFunc;
};


#endif /* defined(__IF__BaseSkill__) */
