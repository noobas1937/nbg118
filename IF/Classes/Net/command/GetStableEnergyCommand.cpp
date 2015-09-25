//
//  GetStableEnergyCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-27.
//
//

#include "GetStableEnergyCommand.h"
#include "UIComponent.h"

bool GetStableEnergyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_STABLE_ENERGY_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        GlobalData::shared()->lastAcceptEnergyTime = params->valueForKey("lastAcceptEnergyTime")->doubleValue() / 1000;
        GlobalData::shared()->lordInfo.energy = params->valueForKey("energy")->intValue();
//        UIComponent::getInstance()->onCityEnergyUpdate(NULL);
    }
    
    return true;
}