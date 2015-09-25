//
//  InvitesAllianceCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-2.
//
//

#include "InvitesAllianceCommand.h"
#include "NetController.h"
#include "AllianceManager.h"

InvitesAllianceCommand::InvitesAllianceCommand(std::string playerId)
: CommandBase(INVITES_PLAYER),m_playerId(playerId)
{
    putParam("playerId", CCString::create(m_playerId.c_str()));
}

bool InvitesAllianceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(INVITES_PLAYER) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        callSuccess(NetResult::create(Error_OK, (CCObject*)params));
    }
    GameController::getInstance()->removeWaitInterface();
    return true;
}