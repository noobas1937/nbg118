//
//  ArmyCancelCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-25.
//
//

#include "ArmyCancelCommand.h"
#include "QueueController.h"

bool ArmyCancelCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(ARMY_CANCEL_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        parseResult(params);
    }
    return true;
}

void ArmyCancelCommand::parseResult(CCDictionary *dict){
    GlobalData::shared()->resourceInfo.setResourceData(dict);
}