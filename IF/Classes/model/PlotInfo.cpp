//
//  PlotInfo.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-6.
//
//

#include "PlotInfo.h"
#include "LocalController.h"
#include "CCCommonUtils.h"

void PlotInfo::parse(cocos2d::CCDictionary* dic){
    if(dic==NULL){
        CCLOG("param dic");
        return ;
    }
    m_id = dic->valueForKey("id")->getCString();
    m_order = dic->valueForKey("order")->intValue();
    m_cover = dic->valueForKey("cover")->intValue();
    
    std::string seat = dic->valueForKey("seat")->getCString();
    CCCommonUtils::splitString(seat,"|",m_seat);
    
    std::string figure = dic->valueForKey("figure")->getCString();
    CCCommonUtils::splitString(figure,"|",m_figure);
    
    std::string content = dic->valueForKey("content")->getCString();
    CCCommonUtils::splitString(content,"|",m_content);

    std::string name = dic->valueForKey("name")->getCString();
    CCCommonUtils::splitString(name,"|",m_names);
}