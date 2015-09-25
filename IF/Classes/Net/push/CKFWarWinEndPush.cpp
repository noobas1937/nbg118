//
//  CKFWarWinEndPush.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/14.
//
//

#include "CKFWarWinEndPush.h"
#include "WorldController.h"

void CKFWarWinEndPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    string srcUid = params->valueForKey("srcUid")->getCString();
    string srcName = params->valueForKey("srcName")->getCString();
    string srcAbbr = params->valueForKey("srcAbbr")->getCString();
    string srcServerId = params->valueForKey("srcServerId")->getCString();

    string dstUid = params->valueForKey("dstUid")->getCString();
    string dstName = params->valueForKey("dstName")->getCString();
    string dstAbbr = params->valueForKey("dstAbbr")->getCString();
    string dstServerId = params->valueForKey("dstServerId")->getCString();
    
    string streakId = params->valueForKey("streakId")->getCString();
    string dialog = CCCommonUtils::getPropById(streakId, "shut_down_desc");
    string temp = "";
    if (srcAbbr!="") {
        temp.append("(");
        temp.append(srcAbbr);
        temp.append(")");
    }
    temp.append(srcName);
    temp.append("#");
    temp.append(srcServerId);
    
    string temp2 = "";
    if (dstAbbr!="") {
        temp2.append("(");
        temp2.append(dstAbbr);
        temp2.append(")");
    }
    temp2.append(dstName);
    temp2.append("#");
    temp2.append(dstServerId);
    
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    string tip = _lang_2(dialog.c_str(), temp.c_str(), temp2.c_str());
    if (tip!="") {
        CCCommonUtils::flySystemUpdateHint(time, true,tip.c_str(),FLY_HINT_SYSTEM);
    }
}