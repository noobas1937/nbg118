//
//  MonsterAttackPush.cpp
//  IF
//
//  Created by ganxiaohua on 15/4/24.
//
//

#include "MonsterAttackPush.h"
#include "ActivityController.h"

void MonsterAttackPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params){
        string mId = params->valueForKey("id")->getCString();
        int type = params->valueForKey("type")->intValue();
        string name = CCCommonUtils::getNameById(mId);
        string allianceName = "";
        if (GlobalData::shared()->playerInfo.isInAlliance()) {
            allianceName = GlobalData::shared()->playerInfo.allianceInfo.name;
        }
        string tip = "";
        switch (type) {
            case 1:
                tip = _lang_1("133044", allianceName.c_str());
                if(params->objectForKey("siegeST")){
                    double siegeST = params->valueForKey("siegeST")->doubleValue()/1000;
                    siegeST = GlobalData::shared()->changeTime(siegeST);
                    ActivityController::getInstance()->aActivityInfo.siegeST = siegeST;
                }
                if(params->objectForKey("siegeET")){
                    double siegeET = params->valueForKey("siegeET")->doubleValue()/1000;
                    siegeET = GlobalData::shared()->changeTime(siegeET);
                    ActivityController::getInstance()->aActivityInfo.siegeET = siegeET;
                }
                break;
            case 2:
                tip = _lang_1("133047", name.c_str());
                break;
            case 3:
                tip = _lang_1("133048", allianceName.c_str());
                break;
            default:
                break;
        }
        double worldTime = GlobalData::shared()->getWorldTime();
        worldTime *= 1000;
        worldTime = GlobalData::shared()->renewTime(worldTime);
        auto time = worldTime+1000*59; 
        if (tip!="") {
            CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_SYSTEM);
        }
        
    }
}