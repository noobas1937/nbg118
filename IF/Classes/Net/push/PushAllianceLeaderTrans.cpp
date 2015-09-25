//
//  PushAllianceLeaderTrans.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-6.
//
//

#include "PushAllianceLeaderTrans.h"

void PushAllianceLeaderTrans::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    auto dic = _dict(params->objectForKey("alliance"));
    if(dic->objectForKey("learderName")){
        std::string learderName = dic->valueForKey("learderName")->getCString();
        GlobalData::shared()->playerInfo.allianceInfo.leader = learderName;
    }
    if(dic->objectForKey("learderUid")){
        std::string learderUid = dic->valueForKey("learderUid")->getCString();
        GlobalData::shared()->playerInfo.allianceInfo.leaderUid = learderUid;
        if(learderUid == GlobalData::shared()->playerInfo.uid){
            GlobalData::shared()->playerInfo.allianceInfo.rank = 5;
        }
    }
    if(dic->objectForKey("curMember")){
        int curMember = dic->valueForKey("curMember")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.currentNum = curMember;
    }
}