//
//  PushKingdomWinner.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-24.
//
//

#include "PushKingdomWinner.h"
#include "WorldController.h"

void PushKingdomWinner::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    double relinquishEndTime = params->valueForKey("relinquishEndTime")->doubleValue();
    string allianceId = params->valueForKey("allianceId")->getCString();
    if(GlobalData::shared()->playerInfo.isInAlliance() && allianceId==GlobalData::shared()->playerInfo.allianceInfo.uid){
         GlobalData::shared()->playerInfo.relinquishEndTime = relinquishEndTime;
    }
    string name = params->valueForKey("name")->getCString();
    string allianceName = params->valueForKey("allianceName")->getCString();
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    if (allianceName!="") {
        CCCommonUtils::flySystemUpdateHint(time, true,_lang_1("110096",allianceName.c_str()),FLY_HINT_KING);
    }else if(name!=""){
        CCCommonUtils::flySystemUpdateHint(time, true,_lang_1("110097",name.c_str()),FLY_HINT_KING);
    }
}