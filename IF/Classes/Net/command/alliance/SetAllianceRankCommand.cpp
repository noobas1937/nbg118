//
//  SetAllianceRankCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-5.
//
//

#include "SetAllianceRankCommand.h"

bool SetAllianceRankCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_SET_RANK) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        auto dic = CCDictionary::create();
        dic->setObject(CCString::create(CC_ITOA(m_rank)), "rank");
        dic->setObject(CCString::create(m_playerId.c_str()), "playerId");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_DATA_CHANGE,dic);
        callSuccess(NetResult::create());
    }
    return true;
}