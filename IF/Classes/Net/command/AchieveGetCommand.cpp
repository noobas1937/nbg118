//
//  AchieveGetCommand.cpp
//  IF
//
//  Created by ganxiaohua on 13-12-24.
//
//

#include "AchieveGetCommand.h"
#include "AchievementInfo.h"
#include "AchievementController.h"
#include "RewardController.h"
#include "GCMRewardController.h"

bool AchieveGetCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(ACHIEVE_GET) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if(params==NULL) return false;
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        AchievementController::getInstance()->isDataBack = true;
        CCArray* achieves = dynamic_cast<CCArray*>(params->objectForKey("list"));
        AchievementController::getInstance()->updateAchievement(achieves);
        if(AchievementController::getInstance()->isNeedPostCompleteAchievementToGoogle){
            AchievementController::getInstance()->postCompleteAchievementToGoogle();
        }
    }
    return true;
}

bool AchieveGetRewardCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(ACHIEVE_GET_REWARD) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if(params==NULL) return false;
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        if(params->objectForKey("reward")){
            auto arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
            GCMRewardController::getInstance()->flyToolReward(GCMRewardController::getInstance()->getFlyArr(arr));
            std::string rewardStr = RewardController::getInstance()->retReward(arr);
            CCCommonUtils::flyHint("", "", rewardStr);
        }
        std::string key = AchievementController::getInstance()->getKeyByItemId(m_itemId);
        AchievementController::getInstance()->m_infos[key].state = ACHIEVEMENT_REWARD_GET;
        AchievementController::getInstance()->allCompelete[m_itemId] = 1;

        if(params->objectForKey("next")){
            AchievementController::getInstance()->updateAchievement(_dict(params->objectForKey("next")), false);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_STATE_CHAGE);
        AchievementController::getInstance()->updateMyMedalInfo();
    }
    if (params->objectForKey("medal"))
    {
        GlobalData::shared()->medalid = params->valueForKey("medal")->getCString();
    }
    return true;
}

bool MedalInitCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MEDAL_INIT) != 0)
    {
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if(params==NULL) return false;
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        CCArray* medalArr = dynamic_cast<CCArray*>(params->objectForKey("medal"));
        if (medalArr)
        {
            AchievementController::getInstance()->initMedalInfo(medalArr);
        }
    }
    
    return true;
}

bool MedalSaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MEDAL_SAVE) != 0)
    {
        return false;
    }
    return true;
}

bool AchieveFindCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACHIEVE_FIND) != 0)
    {
        return false;
    }
    return true;
}