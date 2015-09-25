//
//  AllianceShopShowCommand.cpp
//  IF
//
//  Created by lifangkai on 14-9-1.
//
//

#include "AllianceShopShowCommand.h"
#include "ToolController.h"
#include "StoreBuyConfirmDialog.h"
#include "ActivityController.h"
#include "AllianceManager.h"

bool AllianceShopShowCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_SHOP_SHOW) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    GameController::getInstance()->removeWaitInterface();
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
       
    }else{
       // CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_FUNCTION_EFFECT,CCInteger::create(m_buildingKey));
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("goods"));
        CCObject *goodObject;
        CCARRAY_FOREACH(arr, goodObject){
            CCDictionary *goodsDic = _dict(goodObject);
            std::string itemId = goodsDic->valueForKey("goodsId")->getCString();
            int count =goodsDic->valueForKey("count")->intValue();
            auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
            toolInfo.allianceCnt = count;
           // GlobalData::shared()->armyList[itemId] = ArmyInfo(armyDic);
        }
         int alliancePoint = params->valueForKey("alliancePoint")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.alliancepoint = alliancePoint;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_SHOP_SHOW);
    }
    return true;
}

bool AllianceShopLeaderBuyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_SHOP_BUY_LEADER) != 0)
        return false;
    GameController::getInstance()->removeWaitInterface();
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_BUY_FAIL);
    }else{
        string goodsId = params->valueForKey("goodsId")->getCString();
        int count = params->valueForKey("num")->intValue();
        int alliancePoint = params->valueForKey("alliancePoint")->intValue();
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(goodsId.c_str()));
        toolInfo.allianceCnt = count;
        GlobalData::shared()->playerInfo.allianceInfo.alliancepoint = alliancePoint;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_SHOP_BUY_LEADER,CCInteger::create(atoi(m_goodId.c_str())));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK);
        string name = toolInfo.getName();
        if (m_num > 1) {
            name += " × ";
            name += CC_ITOA(m_num);
        }
        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(goodsId), "", _lang_1("115829", name.c_str()));
    }
    return true;
}

bool AllianceShopUserBuyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_SHOP_BUY_USER) != 0)
        return false;
    GameController::getInstance()->removeWaitInterface();
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        
    }else{
        string goodsId = params->valueForKey("goodsId")->getCString();
        int count = params->valueForKey("num")->intValue();
        int point = params->valueForKey("point")->intValue();

        GlobalData::shared()->playerInfo.allianceInfo.accPoint = point;
        
        CCDictionary* item = _dict(params->objectForKey("goods"));
        string itemIdstr = item->valueForKey("itemId")->getCString();
        int itemId = atoi(itemIdstr.c_str());
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
        if (count >= 0)
        {
            info.allianceCnt = count;
        }
        info.SetInfoFromServer(item);
        
       // CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_SHOP_BUY_USER,CCInteger::create(atoi(m_goodId.c_str())));
        string name = info.getName();
        if (m_num > 1) {
            name += " × ";
            name += CC_ITOA(m_num);
        }
        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), "", _lang_1("104918", name.c_str()));
        
        CCArray* arr = CCArray::create();
        CCDictionary* arrDic = CCDictionary::create();
        CCDictionary* value = CCDictionary::create();
        value->setObject(CCString::create(itemIdstr), "itemId");
        value->setObject(CCString::create(CC_ITOA(m_num)), "rewardAdd");
        arrDic->setObject(value, "value");
        arr->addObject(arrDic);
        PortActController::getInstance()->flyToolReward(arr, true);
    }
    return true;
}