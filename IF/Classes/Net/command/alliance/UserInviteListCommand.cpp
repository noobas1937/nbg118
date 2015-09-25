//
//  UserInviteListCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#include "UserInviteListCommand.h"
#include "NetController.h"
#include "AllianceInfo.h"
#include "AllianceManager.h"

UserInviteListCommand::UserInviteListCommand(int page)
: CommandBase(USER_INVITES_LIST),m_page(page)
{
    putParam("page", CCInteger::create(page));
}

bool UserInviteListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(USER_INVITES_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        map<std::string, AllianceInfo*>::iterator it;
        for(it = AllianceManager::getInstance()->invitesAllianceList.begin(); it != AllianceManager::getInstance()->invitesAllianceList.end(); it++){
            it->second->release();
        }
        AllianceManager::getInstance()->invitesAllianceList.clear();
        CCArray* arr =  (CCArray*)params->objectForKey("list");
        int total = params->valueForKey("total")->intValue();
        AllianceManager::getInstance()->maxAlliancePage = total/10+(total%10==0?0:1);
        int num = arr->count();
        for (int i=0; i<num; i++) {
            CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);
            AllianceInfo* alliance = new AllianceInfo();
            alliance->updateAllianceInfo(dicAlliance);
            AllianceManager::getInstance()->invitesAllianceList[alliance->uid] = alliance;
        }
        callSuccess(NetResult::create());
    }
    return true;
}