//
//  MailGetRewardCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-1-8.
//
//

#include "MailGetRewardCommand.h"
#include "GCMRewardController.h"
#include "ToolController.h"
#include "GlobalData.h"
#include "EquipmentController.h"
#include "ChatServiceCocos2dx.h"
#include "MailController.h"

bool MailGetRewardCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_GET_REWARD) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (MailController::getInstance()->getIsNewMailListEnable())
        ChatServiceCocos2dx::setMailRewardStatus(m_uid);
#endif
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return true;
    }
    
    if(params->objectForKey("honor")){
        int honor = params->valueForKey("honor")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.accPoint = honor;
    }
    if(params->objectForKey("alliancepoint")){
        int alliancepoint = params->valueForKey("alliancepoint")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.point = alliancepoint;
    }

    map<string, MailInfo*>::iterator it = GlobalData::shared()->mailList.find(m_uid);
    if(it != GlobalData::shared()->mailList.end()){
        it->second->rewardStatus = 1;
    }else{
        return true;
    }
    if(params->objectForKey("goods")){
//        if(it->second->killReward == NULL){
//            it->second->killReward = CCArray::create();
//        }

        auto arr = dynamic_cast<CCArray*>(params->objectForKey("goods"));
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj) {
            std::string itemId = _dict(obj)->valueForKey("itemId")->getCString();
            auto &item = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
            int num = _dict(obj)->valueForKey("count")->intValue() - item.getCNT();
            if(num <= 0){
                continue;
            }
            auto d = CCDictionary::create();
            CCDictionary *valueDict = CCDictionary::create();
            valueDict->setObject(CCString::create(itemId), "itemId");
            valueDict->setObject(CCString::create(CC_ITOA(num)), "count");

            d->setObject(CCString::create(CC_ITOA(R_GOODS)), "type");
            d->setObject(valueDict, "value");
//            it->second->killReward->addObject(d);
        }
//        if(it->second->killReward){
//            it->second->killReward->retain();
//        }
        
        GCMRewardController::getInstance()->flyToolReward(arr);
    }
    if(params->objectForKey("equip")){
        CCArray *equipArray = dynamic_cast<CCArray*>(params->objectForKey("equip"));
        if(equipArray){
            EquipmentController::getInstance()->addEquip(equipArray);
        }
    }
    CCArray* arr = GCMRewardController::getInstance()->retReward(params);
    GCMRewardController::getInstance()->flyReward(arr);
    

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MIAL_GET_REWARD_REFRESH, CCString::create(m_uid));
    
    return true;
}