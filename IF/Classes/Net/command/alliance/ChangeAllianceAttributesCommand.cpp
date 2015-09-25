//
//  ChangeAllianceAttributesCommand.cpp
//  IF
//
//  Created by chenliang on 14-4-15.
//
//

#include "ChangeAllianceAttributesCommand.h"

#include "NetController.h"

static string COMMAND_NAME = "al.attr";

ChangeAllianceAttributesCommand::ChangeAllianceAttributesCommand()
: CommandBase(COMMAND_NAME)
{
    
}

bool ChangeAllianceAttributesCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_NAME.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        if(params->objectForKey("gold")){
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        callSuccess(NetResult::create(Error_OK, (CCObject*)params));
    }
    return true;
}
