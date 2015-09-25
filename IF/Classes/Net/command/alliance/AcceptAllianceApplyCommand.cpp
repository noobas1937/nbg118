//
//  AcceptAllianceApplyCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-2.
//
//

#include "AcceptAllianceApplyCommand.h"
#include "NetController.h"
#include "AllianceManager.h"

AcceptAllianceApplyCommand::AcceptAllianceApplyCommand(std::string Id,std::string mailId)
: CommandBase(ACCEPT_APPLY),playerId(Id),m_mailId(mailId)
{
    putParam("playerId", CCString::create(playerId.c_str()));
    if(m_mailId!=""){
        putParam("mailId", CCString::create(m_mailId.c_str()));
    }
}

bool AcceptAllianceApplyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACCEPT_APPLY) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        if(m_mailId!=""){
            CCLOG("accept form mail error =%s",_lang(pStr->getCString()).c_str());
        }else{
            CCCommonUtils::flyText(_lang(pStr->getCString()));
        }
    }else{
        callSuccess(NetResult::create());
    }
    return true;
}