//
//  QuestRewardInfoGetCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-25.
//
//

#include "QuestRewardInfoGetCommand.h"
#include "QuestController.h"

bool QuestRewardInfoGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEST_REWRD_INFO_GET_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        CCDictElement *ele;
        bool flag = false;
        CCDICT_FOREACH(params, ele) {
            std::string itemId = ele->getStrKey();
            CCArray *arr = dynamic_cast<CCArray*>(ele->getObject());
            if(arr){
                QuestController::getInstance()->setQuestReward(itemId, arr);
            }
            flag = true;
        }
        if(flag){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(QUEST_REWARD_DATA_BACK);
        }
    }
    return true;
}