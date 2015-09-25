//
//  JoinAllianceCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-24.
//
//

#include "JoinAllianceCommand.h"
#include "NetController.h"
#include "AllianceManager.h"

JoinAllianceCommand::JoinAllianceCommand(std::string allianceId)
: CommandBase(JOIN_ALLIANCE),m_allianceId(allianceId)
{
    putParam("allianceId", CCString::create(m_allianceId.c_str()));
}

bool JoinAllianceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(JOIN_ALLIANCE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        if(pStr->compare("EE000000")==0){
            //无效处理，不提示给玩家
        }else{
            CCCommonUtils::flyText(pStr->getCString());
        }
    }else{
        map<std::string, AllianceInfo*>::iterator it = AllianceManager::getInstance()->allianceList.find(m_allianceId);
        if(it!=AllianceManager::getInstance()->allianceList.end()){
            
        }
        callSuccess(NetResult::create());
    }
    return true;
}