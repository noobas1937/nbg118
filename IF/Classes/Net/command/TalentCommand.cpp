//
//  TalentCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-20.
//
//

#include "TalentCommand.h"
#include "UIComponent.h"

bool TalentSaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TALENT_SAVE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        
    }
    return true;
}

bool TalentClearCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TALENT_CLEAR) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        if (params->objectForKey("gold")) {
            int tmpInt = params->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
    }
    return true;
}