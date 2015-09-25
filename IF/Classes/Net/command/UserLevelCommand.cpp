//
//  UserLevelCommand.cpp
//  IF
//
//  Created by wangzhenlei on 14-1-2.
//
//

#include "UserLevelCommand.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "QueueController.h"
#include "WorldController.h"
#include "QuestController.h"
#include "RewardController.h"
#include "SocialController.h"

bool UserLevelCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(User_LV_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        //army初始化
        CCArray *armyArr = dynamic_cast<CCArray*>(params->objectForKey("army"));
        if (armyArr) {
            CCObject *armyObject;
            CCARRAY_FOREACH(armyArr, armyObject){
                CCDictionary *armyDic = _dict(armyObject);
                std::string itemId = armyDic->valueForKey("id")->getCString();
                GlobalData::shared()->armyList[itemId].setArmyInfo(armyDic);
            }
        }
        
        //fort 初始化
        CCArray *fortArr = dynamic_cast<CCArray*>(params->objectForKey("fort"));
        CCObject *fortObject;
        CCARRAY_FOREACH(fortArr, fortObject){
            CCDictionary *forDic = _dict(fortObject);
            std::string itemId = forDic->valueForKey("id")->getCString();
            GlobalData::shared()->fortList[itemId].setArmyInfo(forDic);
        }
        
        //建造队列信息初始化
        QueueController::getInstance()->initQueueData(params);
        
        //资源初始化
        CCDictionary* cityInfo = dynamic_cast<CCDictionary*>(params->objectForKey("resource"));
        if (cityInfo) {
            auto &resourceInfo = GlobalData::shared()->resourceInfo;
            resourceInfo.setResourceData(cityInfo);
        }
        
        //将军信息初始化
        CCArray *generalArr = NULL;
        generalArr = dynamic_cast<CCArray*>(params->objectForKey("userGenerals"));
        if (generalArr) {
            int length = generalArr->count();
            CCDictionary *generalItem = NULL;
            for (int i=0; i<length; i++) {
                generalItem = _dict(generalArr->objectAtIndex(i));
                string uuid = generalItem->valueForKey("uuid")->getCString();
                GlobalData::shared()->generals[uuid].updateGeneralData(generalItem);
            }
        }
        
        //quest
        CCArray* tasks = dynamic_cast<CCArray*>(params->objectForKey("task"));
        QuestController::getInstance()->initQuest(tasks);
        
        //world
        CCArray *world = dynamic_cast<CCArray*>(params->objectForKey("worldMarch"));
        if(world){
            WorldController::getInstance()->doWhenBackToGame(world);
        }
        
        if (params->objectForKey("gold")) {
            long _gold = params->valueForKey("gold")->intValue();
            GlobalData::shared()->playerInfo.gold = _gold;
        }
        CCDictionary* mine = _dict(params->objectForKey("mine"));
        SocialController::getInstance()->parse(mine);
    }
    
    return true;
}

bool UserLevelRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(User_LV_REWARD_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        GlobalData::shared()->lordInfo.levelUp+=1;
        int userLv = GlobalData::shared()->lordInfo.levelUp;
        if(userLv<GlobalData::shared()->playerInfo.level){
            int key = 100100 + userLv;
            auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(key));
            string item =  dict->valueForKey("level_item")->getCString();
            if(item!=""){
                CCDictionary* tmp = CCDictionary::create();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(User_LV_REWARD_DELAY);
            }
   
         // CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAYER_LEVELUP);
        }
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
        std::string str = RewardController::getInstance()->retReward(arr);

    }
    
    return true;
}