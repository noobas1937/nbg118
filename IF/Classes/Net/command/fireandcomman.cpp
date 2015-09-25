//
//  fireandcomman.cpp
//  IF
//
//  Created by jiaohongye on 14-2-24.
//
//

#include "fireandcomman.h"
#include "TrainSoldierCommand.h"
#include "ChatLockInfoCommand.h"
#include "ShieldUnlockPopUpView.h"

bool fireandcomman::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(FIRE) != 0&&dict->valueForKey("cmd")->compare(FORT_FIRE) !=0&&dict->valueForKey("cmd")->compare(HOSPITAL_FIRE) !=0){
        return false;
    }
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        std::string id = params->valueForKey("id")->getCString();
        int value = params->valueForKey("free")->intValue();
        if(dict->valueForKey("cmd")->compare(FIRE) == 0){
            GlobalData::shared()->armyList[id].free = value;
        }
        if(dict->valueForKey("cmd")->compare(FORT_FIRE) == 0){
            GlobalData::shared()->fortList[id].free = value;
        }
        if(dict->valueForKey("cmd")->compare(HOSPITAL_FIRE) == 0){
            auto arr = dynamic_cast<CCArray*>(params->objectForKey("hospital"));
            for (int i=0; i<arr->count(); i++) {
                CCDictionary *dict=dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
                GlobalData::shared()->treatList[dict->valueForKey("id")->getCString()].dead=dict->valueForKey("dead")->intValue();
            }
//            GlobalData::shared()->treatList[id].dead=value;
        }

        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ARMY_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
    }
    return true;
}





