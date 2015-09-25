//
//  ClearAllianceGiftCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-10.
//
//

#include "ClearAllianceGiftCommand.h"
#include "NetController.h"
#include "AllianceManager.h"

ClearAllianceGiftCommand::ClearAllianceGiftCommand(std::string giftId)
: CommandBase(CLEAR_GIFT),m_giftId(giftId)
{
    putParam("giftId", CCString::create(m_giftId.c_str()));
}

bool ClearAllianceGiftCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CLEAR_GIFT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        AllianceManager::getInstance()->giftsList.erase(m_giftId);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ALLIANCE_GIFT_DATA,NULL);
        //callSuccess(NetResult::create());
    }
    return true;
}