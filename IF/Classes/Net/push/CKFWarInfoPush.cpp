//
//  CKFWarInfoPush.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/3.
//
//

#include "CKFWarInfoPush.h"
#include "WorldController.h"

void CKFWarInfoPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    string uid = params->valueForKey("uid")->getCString();
    string name = params->valueForKey("name")->getCString();
    string abbr = params->valueForKey("abbr")->getCString();
    string serverId = params->valueForKey("serverId")->getCString();
    string positionId = params->valueForKey("positionId")->getCString();
    int pointId = params->valueForKey("pointId")->intValue();
    int streakNum = params->valueForKey("streakNum")->intValue();
    CCPoint pos = WorldController::getInstance()->getPointByIndex(pointId);
    string oName = CCCommonUtils::getNameById(positionId);
    
    string temp = "";
    if (abbr!="") {
        temp.append("(");
        temp.append(abbr);
        temp.append(")");
    }
    temp.append(name);
    temp.append("#");
    temp.append(serverId);
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    string tip = "";
    string posStr = "(";
    posStr.append(CC_ITOA(pos.x));
    posStr.append(",");
    posStr.append(CC_ITOA(pos.y));
    posStr.append(")");
    if (oName!="") {
        temp = "";
        if (abbr!="") {
            temp.append("(");
            temp.append(abbr);
            temp.append(")");
        }
        temp.append(name);
        
        string temp2 = "#";
        temp2.append(serverId);
        
        tip = _lang_5("138129", temp2.c_str(),oName.c_str(),temp.c_str(),CC_ITOA(streakNum), posStr.c_str());
    }else{
        tip = _lang_3("138127", temp.c_str(), CC_ITOA(streakNum), posStr.c_str());
    }

    CCCommonUtils::flySystemUpdateHint(time, true,tip.c_str(),FLY_HINT_SYSTEM);

}