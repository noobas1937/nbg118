//
//  TreatInfo.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-6.
//
//

#include "TreatInfo.h"
#include "ArmyController.h"

TreatInfo::TreatInfo(cocos2d::CCDictionary *dict){
    dead = dict->valueForKey("dead")->intValue();
    armyId = dict->valueForKey("id")->getCString();
    prepareToTreat = 0;
    const cocos2d::CCString *pStr = dict->valueForKey("heal");
    if(pStr->compare("")!=0){
        heal = pStr->intValue();
    }else{
        heal = 0;
    }
    std::string str = dict->valueForKey("finish")->getCString();
    if(str != ""){
        time_t finishTime = atof(str.c_str()) / 1000;
        ArmyController::getInstance()->setTreatFinishTime(finishTime);
    }
}