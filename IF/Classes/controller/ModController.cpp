//
//  ModController.cpp
//  IF
//
//  Created by 李锐奇 on 14/11/20.
//
//

#include "ModController.h"

static ModController *_instance = NULL;
ModController *ModController::getInstance(){
    if(_instance == NULL){
        _instance = new ModController();
    }
    return _instance;
}

void ModController::purgeData(){
    nextRewardTime = 0;
    rewardGold = 0;
    if(_instance){
        delete _instance;
    }
    _instance = NULL;
}

void ModController::parseData(CCDictionary *dict){
    
}
