//
//  GuideInfo.h
//  IF
//
//  Created by ganxiaohua on 13-11-6.
//
//

#ifndef __IF__GuideInfo__
#define __IF__GuideInfo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"

class GuideInfo : public cocos2d::CCObject {
public:
    ~GuideInfo(){};
    GuideInfo():m_id(std::string())
    ,m_interface(std::string())
    ,m_order(0)
    ,m_plot(0)
    ,m_arrow(0)
    ,m_area(std::string())
    ,m_operate(0)
    ,m_hide(1)
    ,m_fly("2")
    ,m_flyTo("")
    ,m_AnimationName("")
    ,m_enforce(false)
    ,m_mark("")
    ,m_decide("")
    ,m_step("")
    ,m_transition("")
    ,m_open("")
    {};
    
    void parse(cocos2d::CCDictionary* dic);
    
    CC_SYNTHESIZE(std::string,m_id,Id);
    CC_SYNTHESIZE(std::string,m_interface,Interface);
    CC_SYNTHESIZE(int,m_order,Order);
    CC_SYNTHESIZE(int,m_plot,Plot);
    CC_SYNTHESIZE(int,m_arrow,Arrow);
    CC_SYNTHESIZE(std::string,m_area,Area);
    CC_SYNTHESIZE(int,m_operate,Operate);
    CC_SYNTHESIZE(int, m_hide, Hide);
    CC_SYNTHESIZE(std::string, m_fly, Fly);
    CC_SYNTHESIZE(std::string, m_flyTo, FlyTo);
    CC_SYNTHESIZE(std::string, m_AnimationName, AnimationName);
    CC_SYNTHESIZE(bool, m_enforce, Enforce);
    CC_SYNTHESIZE(std::string, m_mark, Mark);
    CC_SYNTHESIZE(std::string, m_decide, Decide);
    CC_SYNTHESIZE(std::string, m_step, Step);
    CC_SYNTHESIZE(std::string, m_transition, Transition);
    CC_SYNTHESIZE(std::string, m_open, Open);
};
#endif /* defined(__IF__GuideInfo__) */
