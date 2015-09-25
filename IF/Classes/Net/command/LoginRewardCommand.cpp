//
//  LoginRewardCommand.cpp
//  IF
//
//  Created by 邹 程 on 13-12-2.
//
//

#include "LoginRewardCommand.h"
#include "UIComponent.h"
#include "GeneralManager.h"

bool LoginRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(LOGIN_REWARD_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        //parse errorCode
        auto ret = NetResult::createWithFail(params);
        callFail(ret);
        return true;
    }
    
    auto &loginReward = GlobalData::shared()->loginReward;
    
    CCDictElement *pDict;
    CCDICT_FOREACH(params, pDict) {
        int slot = atoi(pDict->getStrKey()) - 1;
        auto info = _dict(pDict->getObject());
        int color = info->valueForKey("type")->intValue() - 1;
        loginReward.lotteryResult[slot] = color;
        
        auto values = dynamic_cast<CCArray*>(info->objectForKey("values"));
        CCObject* obj;
        int j = 0;
        CCARRAY_FOREACH(values, obj) {
            loginReward.lotteryValue[slot][j++] = atoi(dynamic_cast<CCString*>(obj)->getCString());
        }
    }
    
    // add gold
    int goldCount = _dict(params->objectForKey("1"))->valueForKey("total")->intValue();
    GlobalData::shared()->playerInfo.gold = goldCount;
    // add coin
    int coinCount = _dict(params->objectForKey("2"))->valueForKey("total")->intValue();
    GlobalData::shared()->resourceInfo.lMoney = coinCount;
    
    // do not update UI here , since the lottery animation is not finished!
    
    // add general cards
    auto generals = dynamic_cast<CCArray*>(_dict(params->objectForKey("3"))->objectForKey("gens"));
    CCObject* pObj;
    CCARRAY_FOREACH(generals, pObj) {
        auto generalInfo = _dict(pObj);
        GeneralManager::getInstance()->addGeneral(generalInfo);
    }
    
    callSuccess(NetResult::create());
    return true;
}
