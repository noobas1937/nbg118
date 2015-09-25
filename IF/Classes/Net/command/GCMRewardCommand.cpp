//
//  GCMRewardCommand.cpp
//  IF
//
//  Created by fubin on 13-12-25.
//
//

#include "GCMRewardCommand.h"
#include "GCMRewardController.h"

bool GCMRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GCM_REWARD_COMMAND) == 0)
    {
        CCDictionary *params=_dict(dict->objectForKey("params"));
        GCMRewardController::getInstance()->retReward(params);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(FEED_REWARD_COMMAND) == 0)
    {
        CCDictionary *params=_dict(dict->objectForKey("params"));
        int addValue = params->valueForKey("MONEY")->intValue() - GlobalData::shared()->resourceInfo.lMoney;
        GlobalData::shared()->resourceInfo.lMoney = params->valueForKey("MONEY")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        
        string tmp = "+ ";
        tmp = tmp+CC_ITOA(addValue);
        CCSafeObject<CCString> temp=CCString::create(tmp);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FLY_TEXT,temp);
        return true;
    }
    return false;
}