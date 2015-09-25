//
//  AllianceShowGiftCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-10.
//
//

#include "AllianceShowGiftCommand.h"
#include "NetController.h"
#include "AllianceInfo.h"
#include "AllianceManager.h"
#include "AllianceGiftInfo.h"

AllianceShowGiftCommand::AllianceShowGiftCommand()
: CommandBase(SHOW_GIFT)
{
}

bool AllianceShowGiftCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SHOW_GIFT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        AllianceManager::getInstance()->giftsList.clear();
        CCArray* arr =  (CCArray*)params->objectForKey("gifts");
        if(arr!=NULL){
            int num = arr->count();
            for (int i=0; i<num; i++) {
                CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(i);
                AllianceGiftInfo* info = new AllianceGiftInfo();
                info->updateInfo(dic);
                AllianceManager::getInstance()->giftsList[info->giftId] = info;
            }
        }
        callSuccess(NetResult::create());
    }
    return true;
}