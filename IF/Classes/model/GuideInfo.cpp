//
//  GuideInfo.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-6.
//
//

#include "GuideInfo.h"

void GuideInfo::parse(cocos2d::CCDictionary* dic){
    m_id = dic->valueForKey("id")->getCString();
    m_interface = dic->valueForKey("interface")->getCString();
    m_order = dic->valueForKey("order")->intValue();
    m_plot = dic->valueForKey("plot")->intValue();
    m_arrow = dic->valueForKey("arrow")->intValue();
    m_area = dic->valueForKey("area")->getCString();
    m_operate = dic->valueForKey("operate")->intValue();
    m_hide = dic->valueForKey("hide")->intValue();
    m_fly = dic->valueForKey("fly")->getCString();
    m_flyTo = dic->valueForKey("display")->getCString();
    m_AnimationName = dic->valueForKey("bling")->getCString();
    std::string enforce = dic->valueForKey("enforce")->getCString();
    if(enforce != ""){
        m_enforce = true;
    }else{
        m_enforce = false;
    }
    std::string mark = dic->valueForKey("mark")->getCString();
    if(mark != ""){
        m_mark = mark;
    }else{
        m_mark = "";
    }
    std::string decide = dic->valueForKey("decide")->getCString();
    if(decide != ""){
        m_decide = decide;
    }else{
        m_decide = "";
    }
    std::string step = dic->valueForKey("step")->getCString();
    if(step != ""){
        m_step = step;
    }else{
        m_step = "";
    }
    std::string transition = dic->valueForKey("transition")->getCString();
    if(transition != ""){
        m_transition = transition;
    }else{
        m_transition = "";
    }
    std::string open = dic->valueForKey("open")->getCString();
    if(open != ""){
        m_open = open;
    }else{
        m_open = "";
    }
}