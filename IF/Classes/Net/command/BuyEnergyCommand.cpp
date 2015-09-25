//
//  BuyEnergyCommand.cpp
//  IF
//
//  Created by fubin on 13-12-5.
//
//

#include "BuyEnergyCommand.h"

bool BuyEnergyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(BUY_ENERGY_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if (params1) {
            if(params1->objectForKey("errorCode"))
            {
                CCCommonUtils::flyText(_lang("E100046").c_str());
                return true;
            }
//            etObj.putInt("costGold", costGold);
//            retObj.putLong("gold", userProfile.getGold());
//            retObj.putInt("energy", lord.getEnergy());
//            retObj.putInt("buyEnergyTimesDaily", lord.getBuyEnergyTimesDaily());
            GlobalData::shared()->playerInfo.gold = params1->valueForKey("gold")->intValue();
            GlobalData::shared()->lordInfo.energy = params1->valueForKey("energy")->intValue();
            GlobalData::shared()->lordInfo.buyEnergyTimesDaily = params1->valueForKey("buyEnergyTimesDaily")->intValue();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_ENERGY_UPDATE,CCBoolean::create(true));
        }
        
        return true;
    }
    return false;
}