//
//  PushKingdomThroneOwnerCountdown.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-15.
//
//

#include "PushKingdomThroneOwnerCountdown.h"

void PushKingdomThroneOwnerCountdown::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string name = params->valueForKey("name")->getCString();
    string allianceName = params->valueForKey("allianceName")->getCString();
    auto countdown = params->valueForKey("countdown")->doubleValue();
    auto m_countDown = countdown/(1000*60);
    if(countdown>0){
        string delayTime = CC_ITOA(m_countDown);
        string tip = "";
        if (allianceName!="") {
            tip = _lang_2("110007", allianceName.c_str(), delayTime.c_str());
        }else{
            tip = _lang_2("110008", name.c_str(),delayTime.c_str());
        }
        double worldTime = GlobalData::shared()->getWorldTime();
        worldTime *= 1000;
        worldTime = GlobalData::shared()->renewTime(worldTime);
        auto time = worldTime+1000*60;
        CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
    }
}