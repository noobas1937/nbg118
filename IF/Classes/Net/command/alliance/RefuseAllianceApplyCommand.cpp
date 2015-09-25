//
//  RefuseAllianceApplyCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-2.
//
//

#include "RefuseAllianceApplyCommand.h"
#include "NetController.h"
#include "AllianceManager.h"

RefuseAllianceApplyCommand::RefuseAllianceApplyCommand(std::string playerId,std::string mailId)
: CommandBase(REFUSE_APPLY),m_playerId(playerId),m_mailId(mailId)
{
    putParam("playerId", CCString::create(m_playerId.c_str()));
    if(m_mailId!=""){
        putParam("mailId", CCString::create(m_mailId.c_str()));
    }
}

bool RefuseAllianceApplyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(REFUSE_APPLY) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        if(m_mailId!=""){
            CCLOG("refuse form mail error =%s",_lang(pStr->getCString()).c_str());
        }else{
            CCCommonUtils::flyText(_lang(pStr->getCString()));
        }
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INVITES_DATA,NULL);
        //callSuccess(NetResult::create());
    }
    return true;
}