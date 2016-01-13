//
//  GetAllianceRankCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-8.
//
//

#include "GetAllianceRankCommand.h"

#include "NetController.h"
#include "AllianceInfo.h"
#include "AllianceManager.h"
#include "PlayerInfo.h"

GetAllianceRankCommand::GetAllianceRankCommand(std::string allianceId)
: CommandBase(RANK_LIST),m_allianceId(allianceId)
{
    putParam("allianceId", CCString::create(m_allianceId.c_str()));
}

bool GetAllianceRankCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(RANK_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        map<std::string, PlayerInfo*>::iterator it;
        for(it = AllianceManager::getInstance()->rankPlayerList.begin(); it != AllianceManager::getInstance()->rankPlayerList.end(); it++){
            it->second->release();
        }
        AllianceManager::getInstance()->rankPlayerList.clear();
        CCArray* arr =  (CCArray*)params->objectForKey("list");
        int num = arr->count();
        for (int i=0; i<num; i++) {
            CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(i);
            PlayerInfo* info = new PlayerInfo();
            info->updateInfo(dic);
            AllianceManager::getInstance()->rankPlayerList[info->uid] = info;
        }
        callSuccess(NetResult::create());
    }
    return true;
}