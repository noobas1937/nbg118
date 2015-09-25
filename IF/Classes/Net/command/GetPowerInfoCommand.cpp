//
//  GetPowerInfoCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-20.
//
//

#include "GetPowerInfoCommand.h"
#include "NetController.h"
#include "ToolController.h"


GetPowerInfoCommand::GetPowerInfoCommand()
: CommandBase(GET_POWER_INFO)
{
}

bool GetPowerInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_POWER_INFO) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        GlobalData::shared()->playerInfo.updateInfo(params);
        callSuccess(NetResult::create(Error_OK, (CCObject*)(params)));
    }
    return true;
}