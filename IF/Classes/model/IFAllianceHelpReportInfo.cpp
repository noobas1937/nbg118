//
//  IFAllianceHelpReportInfo.cpp
//  IF
//
//  Created by wangdianzhen on 15/10/21.
//
//

#include "IFAllianceHelpReportInfo.hpp"

void IFAllianceHelpReportInfo::parseData(cocos2d::CCDictionary * data){
    if (data->valueForKey("name")) {
        m_name = data->valueForKey("name")->getCString();
    }
    if (data->valueForKey("soldier")) {
        m_count = data->valueForKey("soldier")->intValue();
    }
    
    if (data->valueForKey("timestamp")) {
        m_time = data->valueForKey("timestamp")->doubleValue() / 1000;
    }
    
    if (data->valueForKey("pic")) {
        m_pic = data->valueForKey("pic")->getCString();
    }
    
    if (data->valueForKey("uid")) {
        m_uid = data->valueForKey("uid")->getCString();
    }
    
    if (data->valueForKey("picVer")) {
        m_picVer = data->valueForKey("picVer")->intValue();
    } 
}