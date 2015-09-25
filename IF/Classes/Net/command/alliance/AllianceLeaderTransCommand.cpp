//
//  AllianceLeaderTransCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-6.
//
//

#include "AllianceLeaderTransCommand.h"

bool AllianceLeaderTransCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_LEADER_TRANS) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        CCDictionary *alliance=_dict(params->objectForKey("alliance"));
        GlobalData::shared()->playerInfo.allianceInfo.updateAllianceInfo(alliance);
        std::string playerId = params->valueForKey("oldLeaderId")->getCString();
        int oldRank = params->valueForKey("oldLeaderRank")->intValue();
        auto dic = CCDictionary::create();
        dic->setObject(CCString::create(CC_ITOA(oldRank)), "rank");
        dic->setObject(CCString::create(playerId.c_str()), "playerId");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_LEADER_CHANGE,dic);
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool AllianceOrderCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_ORDER_CMD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
    }
    return true;
}