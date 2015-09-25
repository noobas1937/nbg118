//
//  HospitalReceivePush.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-15.
//
//

#include "HospitalReceivePush.h"
#include "ArmyController.h"

void HospitalReceivePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    CCArray *arr = dynamic_cast<CCArray*>(result->objectForKey("hospital"));
    if(arr){
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            auto dic = dynamic_cast<CCDictionary*>(obj);
            if(dic){
                ArmyController::getInstance()->refreshTreatDeadNum(dic);
            }
        }
    }
    return;
}