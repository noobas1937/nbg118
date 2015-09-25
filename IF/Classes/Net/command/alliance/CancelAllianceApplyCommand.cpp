//
//  CancelAllianceApplyCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-2.
//
//

#include "CancelAllianceApplyCommand.h"

#include "NetController.h"
#include "AllianceManager.h"

CancelAllianceApplyCommand::CancelAllianceApplyCommand(std::string allianceId)
: CommandBase(CANCEL_APPLY),m_allianceId(allianceId)
{
    putParam("allianceId", CCString::create(m_allianceId.c_str()));
}

bool CancelAllianceApplyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CANCEL_APPLY) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        //AllianceManager::getInstance()->applyAllianceList.erase(m_allianceId);
        //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INVITES_DATA,NULL);
//        map<std::string, AllianceInfo*>::iterator it = AllianceManager::getInstance()->applyAllianceList.find(m_allianceId);
//        it->second->release();
        callSuccess(NetResult::create());
    }
    return true;
}