//
//  PushKingdomThronePic.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-5.
//
//

#include "PushKingdomThronePic.h"

void PushKingdomThronePic::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string name = params->valueForKey("kingName")->getCString();
    string tip = _lang_1("110103", name.c_str());
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
}