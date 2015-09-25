
#include "MailBatchCommand.h"
#include "ToolController.h"
#include "GCMRewardController.h"
#include "MailController.h"

bool MailBatchDelCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DELETE_BATCH_COMMAND) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return true;
    }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(!MailController::getInstance()->getIsNewMailListEnable())
        MailController::getInstance()->endRemoveAllOpMails(_dict(params->objectForKey("RetObj")));
#else
    MailController::getInstance()->endRemoveAllOpMails(_dict(params->objectForKey("RetObj")));
#endif
    return true;
}

bool MailBatchRewardCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_REWARD_BATCH_COMMAND) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
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
    if(params->objectForKey("goods")){
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
        }
        
        GCMRewardController::getInstance()->flyToolReward(arr);
    }
    CCArray* arr = GCMRewardController::getInstance()->retReward(params);
    GCMRewardController::getInstance()->flyReward(arr);
    
    MailController::getInstance()->endRewardAllOpMails();
    
    return true;
}