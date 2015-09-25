//
//  CityDefenceCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-5-26.
//
//

#include "CityDefenceCommand.h"

bool CityDefenceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CITY_DEFENCE_CMD) != 0)
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