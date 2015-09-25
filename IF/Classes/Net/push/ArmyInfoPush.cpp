//
//  ArmyInfoPush.cpp
//  IF
//
//  Created by 李锐奇 on 14-6-25.
//
//

#include "ArmyInfoPush.h"
#include "ArmyController.h"

void ArmyInfoPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* params = _dict(dict->objectForKey("params"));
    //army初始化
    if(params->objectForKey("soldiers")){
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("soldiers"));
        if(arr){
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj){
                ArmyController::getInstance()->refreshArmyFreeNum(_dict(obj));
            }
        }
    }
    //fort初始化
    if(params->objectForKey("fort")){
        CCArray *fort = dynamic_cast<CCArray*>(params->objectForKey("fort"));
        if(fort){
            CCObject *obj;
            CCARRAY_FOREACH(fort, obj){
                ArmyController::getInstance()->refreshFortFreeNum(_dict(obj));
            }
        }
    }

    return;
}