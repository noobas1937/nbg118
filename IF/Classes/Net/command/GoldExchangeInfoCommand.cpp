//
//  GoldExchangeInfoCommand.cpp
//  IF
//
//  Created by lifangkai on 14-9-10.
//
//

#include "GoldExchangeInfoCommand.h"
#include "RewardController.h"
bool GoldExchangeInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GOLDEXCHANGE_SHOW_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
            GoldExchangeItem* item = it->second;
            delete item;
        }
        GlobalData::shared()->goldExchangeList.clear();
        CCArray *goldExchangeArr = dynamic_cast<CCArray*>(params->objectForKey("exchange"));
        if(GlobalData::shared()->goldExchangeList.empty()){
            CCObject *obj;
            CCARRAY_FOREACH(goldExchangeArr, obj){
                CCDictionary* dict = _dict(obj);
                GoldExchangeItem* goldExchangeItem = new GoldExchangeItem(dict);
                GlobalData::shared()->goldExchangeList[goldExchangeItem->id] = goldExchangeItem;
            }
        }
        GlobalData::shared()->goldExchageRefreshStamp = GlobalData::shared()->getWorldTime();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_SHOW_COMMAND);
    }
    
    return true;
}

bool MonthCardRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MONTH_CARD_REWARD_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        string ardInfo = RewardController::getInstance()->retReward(dynamic_cast<CCArray*>(params->objectForKey("reward")));
        CCCommonUtils::flyHint("", "", ardInfo);
        std::string cardid = getData()->valueForKey("itemId")->getCString();
        if (GlobalData::shared()->monthCardInfoList.find(cardid) != GlobalData::shared()->monthCardInfoList.end()) {
            auto item = GlobalData::shared()->monthCardInfoList[cardid];
            if(item->getItemId().compare(cardid)==0){
                item->addTime();
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_SHOW_COMMAND,CCString::create(cardid.c_str()));
    }
    
    return true;
}