//
//  PushKingdomRes.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-5.
//
//

#include "PushKingdomRes.h"

void PushKingdomRes::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    int resType = params->valueForKey("resType")->intValue();
    string name = params->valueForKey("kingName")->getCString();
    string  resName = CCCommonUtils::getResourceNameByType(resType);
    string tip = _lang_3("110114",name.c_str(),resName.c_str(),resName.c_str());
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
}