//
//  PushKingdomThroneOwne.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-15.
//
//

#include "PushKingdomThroneOwne.h"

void PushKingdomThroneOwne::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string name = params->valueForKey("name")->getCString();
    string allianceName = params->valueForKey("allianceName")->getCString();

    
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    double curTime = GlobalData::shared()->getWorldTime();
    auto m_countDown = GlobalData::shared()->changeTime(time/1000);
    auto m_time = (long)floor( m_countDown - curTime );
    string delayTime = CC_ITOA(m_time/60);
    string tip = "";
    if(allianceName!=""){
        tip = _lang_1("110005", allianceName.c_str());
    }else{
        tip = _lang_1("110006", name.c_str());
    }
    CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
}