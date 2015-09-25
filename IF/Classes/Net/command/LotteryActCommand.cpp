//
//  LotteryActCommand.cpp
//  IF
//
//  Created by chenfubi on 15/1/23.
//
//

#include "LotteryActCommand.h"
#include "GCMRewardController.h"
#include "ToolController.h"

bool LotteryActCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(LOTTERY_START_COMMAND) != 0) {
        return false;
    }
    LotteryController::shared()->setSendCMD(false);
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
//        return true;
    }
    
    if (params->objectForKey("times1"))
    {
        LotteryController::shared()->lotteryInfo.lotteryCnt = params->valueForKey("times1")->intValue();
    }
    
    if (params->objectForKey("times2"))
    {
        LotteryController::shared()->lotteryInfo.lotteryCnt2 = params->valueForKey("times2")->intValue();
    }
    
    if (m_type == 1)
    {
        handleRecieve1(params);
    }
    else if (m_type == 2)
    {
        handleRecieve2(params);
    }
    
    return true;
}

bool LotteryActCommand::handleRecieve1(cocos2d::CCDictionary *dict)
{
    CCDictionary* resourceDic = _dict(dict->objectForKey("resource"));
    
    if (resourceDic->objectForKey("chip"))
    {
        GlobalData::shared()->resourceInfo.lChip = resourceDic->valueForKey("chip")->intValue();
    }
    if (resourceDic->objectForKey("gold"))
    {
        GlobalData::shared()->playerInfo.gold = resourceDic->valueForKey("gold")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    
    LotteryController::shared()->lotteryActCmdBack(dict);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEWCMD, NetResult::createWithSuccess(dict));
    
    return true;
}

bool LotteryActCommand::handleRecieve2(cocos2d::CCDictionary *dict)
{
    CCDictionary* resourceDic = _dict(dict->objectForKey("resource"));
    if (resourceDic->objectForKey("diamond"))
    {
        GlobalData::shared()->resourceInfo.lDiamond = resourceDic->valueForKey("diamond")->intValue();
    }
    if (resourceDic->objectForKey("gold"))
    {
        GlobalData::shared()->playerInfo.gold = resourceDic->valueForKey("gold")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    dict->setObject(CCString::createWithFormat("%d", m_position), "position");
    LotteryController::shared()->lotteryAct2CmdBack(dict);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEWCMD2, NetResult::createWithSuccess(dict));
    
    return true;
}
bool LotteryCanCelCmd::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(LOTTERY_CANCEL_COMMAND) != 0) {
        return false;
    }
    return true;
}
bool LotteryBuyCmd::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(LOTTERY_BUY_COMMAND) != 0) {
        return false;
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
//        return true;
    }
    
    if (params->objectForKey("chip"))
    {
        GlobalData::shared()->resourceInfo.lChip = params->valueForKey("chip")->intValue();
    }
    
    if (params->objectForKey("gold"))
    {
        GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("BUYCHIPBACK", params);
    
    return true;
}
