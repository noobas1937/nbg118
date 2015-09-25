//
//  CancelInviteAllianceCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-4.
//
//

#include "CancelInviteAllianceCommand.h"
#include "NetController.h"
#include "AllianceManager.h"

CancelInviteAllianceCommand::CancelInviteAllianceCommand(std::string Id)
: CommandBase(CANCEL_INVITE),m_playerId(Id)
{
    putParam("playerId", CCString::create(m_playerId.c_str()));
}

bool CancelInviteAllianceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CANCEL_INVITE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        AllianceManager::getInstance()->sentInvitesList.erase(m_playerId);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_INVITES_DATA,NULL);
    }
    return true;
}