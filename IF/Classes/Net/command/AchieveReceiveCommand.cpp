//
//  AchieveReceiveCommand.cpp
//  IF
//
//  Created by ganxiaohua on 13-12-23.
//
//

#include "AchieveReceiveCommand.h"

AchieveReceiveCommand::AchieveReceiveCommand(std::string id)
: CommandBase(ACHIEVE_RECEIVE),m_id(id)
{
    putParam("id", CCString::create(m_id.c_str()));
}

bool AchieveReceiveCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(ACHIEVE_RECEIVE) != 0)
        return false;
    CCDictionary* result = _dict(dict->objectForKey("params"));
    if(result==NULL) return false;
    const CCString *pStr = result->valueForKey("errorCode");
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
//        callSuccess(NetResult::create());
//        map<string,AchievementInfo* >::iterator iter = GlobalData::shared()->achievementList.find(m_id);
//        if(iter!=GlobalData::shared()->achievementList.end()){
//            iter->second->setProgress(result->valueForKey("progress")->intValue());
//            iter->second->setTimes(result->valueForKey("times")->intValue());
//        }
//        GlobalData::shared()->playerInfo.gold = result->valueForKey("gold")->intValue();
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_DATA_CHANGE);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    return true;
}