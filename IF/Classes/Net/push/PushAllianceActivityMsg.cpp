//
//  PushAllianceActivityMsg.cpp
//  IF
//
//  Created by ganxiaohua on 15/3/4.
//
//

#include "PushAllianceActivityMsg.h"

void PushAllianceActivityMsg::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    int type = params->valueForKey("type")->intValue();
    string name = params->valueForKey("kingName")->getCString();
    string tip = "";
    switch (type) {
        case 0:
            tip = _lang_1("133044", "");
            break;
        case 1:
            tip = _lang_1("133045", "");
            break;
        case 2:
            tip = _lang_2("133046", "","");
            break;
        case 3:
            tip = _lang_1("133047", "");
            break;
        case 4:
            tip = _lang_1("133048", "");
            break;
        default:
            break;
    }
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
}