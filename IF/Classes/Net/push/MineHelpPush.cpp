//
//  MineHelpPush.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-21.
//
//

#include "MineHelpPush.h"
#include "SocialController.h"

void MineHelpPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string name = params->valueForKey("name")->getCString();
    auto time = params->valueForKey("time")->doubleValue();
    if(time!=0){
        double mineTime = SocialController::getInstance()->mineTime - GlobalData::shared()->getWorldTime();
        if(SocialController::getInstance()->mineTime==0 || mineTime<=0){
            SocialController::getInstance()->miner = SocialController::getInstance()->miner+1;
        }
    }
    if(name!=""){
        CCCommonUtils::flyHint("", "", _lang_1("107075", name.c_str()));
    }
}