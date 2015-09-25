//
//  RefuseInviteCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#include "RefuseInviteCommand.h"
#include "NetController.h"
#include "AllianceManager.h"

RefuseInviteCommand::RefuseInviteCommand(std::string Id)
: CommandBase(REFUSE_INVITE),m_Id(Id)
{
    putParam("mailId", CCString::create(m_Id.c_str()));
}

bool RefuseInviteCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(REFUSE_INVITE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
       // AllianceManager::getInstance()->invitesAllianceList.erase(m_Id);
      //  CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INVITES_DATA,NULL);
        //callSuccess(NetResult::create());
    }
    return true;
}