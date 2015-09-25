//
//  OpenAllianceGiftCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-10.
//
//

#include "OpenAllianceGiftCommand.h"
#include "NetController.h"
#include "AllianceManager.h"

OpenAllianceGiftCommand::OpenAllianceGiftCommand(std::string giftId)
: CommandBase(OPEN_GIFT),m_giftId(giftId)
{
    putParam("giftId", CCString::create(m_giftId.c_str()));
}

bool OpenAllianceGiftCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(OPEN_GIFT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        map<std::string, AllianceGiftInfo*>::iterator it = AllianceManager::getInstance()->giftsList.find(m_giftId);
        if(it!=AllianceManager::getInstance()->giftsList.end()){
            it->second->valid = 0;
        }
        callSuccess(NetResult::create());
    }
    return true;
}