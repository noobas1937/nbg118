//
//  QuestRewardCommand.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-4.
//
//

#include "QuestRewardCommand.h"
#include "QuestController.h"
#include "RewardController.h"
#include "PopupViewController.h"
#include "GCMRewardController.h"

bool QuestRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEST_REWARD_GET) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    CCLOG("%s", m_id.c_str());
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
        string type = CCCommonUtils::getPropById(m_id, "type");
        if (type=="3") {
            GCMRewardController::getInstance()->retReward2(arr,true);
        }else{
            std::string str = RewardController::getInstance()->retReward(arr);
            CCCommonUtils::flyHint("quest_icon_1.png", _lang("107519"), str, 3, 0, m_isMain);
        }

        //QuestController::getInstance()->removeQuest(m_id);
        CCDictionary *task=_dict(params->objectForKey("task"));
        if(task){
            CCArray* arr = CCArray::create();
            arr->addObject(task);
            QuestController::getInstance()->updateTask(arr);
        }
        if (params->objectForKey("ckfTask")) {
            CCDictionary *ckfTask=_dict(params->objectForKey("ckfTask"));
            CCArray* arr = CCArray::create();
            arr->addObject(ckfTask);
            QuestController::getInstance()->updateTask(arr);
        }
        QuestController::getInstance()->getRewardSuccessHandle(m_id);
    }
    QuestController::getInstance()->resetGettingRewardFlag(0);
    return true;
}

bool FSTaskCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEST_FS_TASK) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}