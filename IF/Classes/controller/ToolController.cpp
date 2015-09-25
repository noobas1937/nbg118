//
//  ToolController.cpp
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#include "ToolController.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "YesNoDialog.h"
#include "QueueController.h"
#include "GeneralManager.h"
#include "WorldController.h"
#include "RewardController.h"
#include "ChangeNickNameCommand.h"
#include "ChangePicCommand.h"
#include "FunBuildController.h"
#include "VipDetailView.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "GlobalData.h"
#include "Utf8Utils.h"
#include "ChestUseView.h"
#include "FBUtilies.h"
#include "EquipmentController.h"
#include "DynamicResourceController.h"
#include "ShowStatusItemCommand.h"
#include "ImperialScene.h"

static ToolController *_instance = NULL;

ToolController::ToolController():m_merchantRefreshCost(0),m_merchantRefreshTime(0),/*m_merchante_time_s1(0),m_merchante_time_e1(0),*/m_merchante_state(0),m_merchante_animation(false),m_merchante_costTip(0),m_maincitySkin(""),m_bMaincityResource(false)
{
    m_callBackFunc = NULL;
    m_willUseItem = 0;
    
    auto statusDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("status");
    if(statusDictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(statusDictInfo, element)
        {
            string strId = element->getStrKey();
            CCDictionary* dictInfo = _dict(element->getObject());
            if(dictInfo->objectForKey("md5") && dictInfo->objectForKey("type2") && dictInfo->valueForKey("type2")->intValue() == 100 && dictInfo->objectForKey("effectart")){
                ToolCustomSkin skin = {dictInfo->valueForKey("md5")->getCString(),dictInfo->valueForKey("effectart")->getCString()};
                m_customSkinGroup[dictInfo->valueForKey("id")->intValue()]=skin;
            }
            int groupId = dictInfo->valueForKey("group")->intValue();
            groupId = groupId/100;
            m_statusGroup[groupId].push_back(atoi(strId.c_str()));
        }
    }
    
    auto allDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("goods");
    if(allDictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(allDictInfo, element)
        {
            string strId = element->getStrKey();
            int id = atoi(strId.c_str());
            CCDictionary* dictInfo = _dict(element->getObject());
            ToolInfo info = ToolInfo(dictInfo);
            m_toolInfos[id] = info;
            
            vector<int>::iterator it;
            bool tbOK = true;
            for (it = m_typeTools[info.type].begin(); it != m_typeTools[info.type].end(); it++) {
                int tmpId = *it;
                if (m_toolInfos[tmpId].orderNum > info.orderNum) {
                    m_typeTools[info.type].insert(it, info.itemId);
                    tbOK = false;
                    break;
                }
            }
            if (tbOK) {
                m_typeTools[info.type].push_back(info.itemId);
            }
            if(info.price_all>0){
                bool bOK = true;
                for (it = m_allianceTools.begin(); it != m_allianceTools.end(); it++) {
                    int tmpId = *it;
                    if (m_toolInfos[tmpId].alliance_order > info.alliance_order) {
                        m_allianceTools.insert(it, id);
                        bOK = false;
                        break;
                    }
                }
                if (bOK) {
                    m_allianceTools.push_back(id);
                }
            }
            if(info.pagehot>0){
                bool bOK = true;
                for (it = m_hotList.begin(); it != m_hotList.end(); it++) {
                    int tmpId = *it;
                    if (m_toolInfos[tmpId].pagehot > info.pagehot) {
                        m_hotList.insert(it, id);
                        bOK = false;
                        break;
                    }
                }
                if (bOK) {
                    m_hotList.push_back(id);
                }
            }
            if (info.page == 1) {
                bool bOK = true;
                for (it = m_warTools.begin(); it != m_warTools.end(); it++) {
                    int tmpId = *it;
                    if (m_toolInfos[tmpId].orderNum > info.orderNum) {
                        m_warTools.insert(it, id);
                        bOK = false;
                        break;
                    }
                }
                if (bOK) {
                    m_warTools.push_back(id);
                }
            }
            else if (info.page == 2) {
                bool bOK = true;
                for (it = m_spdTools.begin(); it != m_spdTools.end(); it++) {
                    int tmpId = *it;
                    if (m_toolInfos[tmpId].orderNum > info.orderNum) {
                        m_spdTools.insert(it, id);
                        bOK = false;
                        break;
                    }
                }
                if (bOK) {
                    m_spdTools.push_back(id);
                }
            }
            else if (info.page == 3) {
                bool bOK = true;
                for (it = m_resTools.begin(); it != m_resTools.end(); it++) {
                    int tmpId = *it;
                    if (m_toolInfos[tmpId].orderNum > info.orderNum) {
                        m_resTools.insert(it, id);
                        bOK = false;
                        break;
                    }
                }
                if (bOK) {
                    m_resTools.push_back(id);
                }
            }
            else if (info.page == 4) {
                bool bOK = true;
                for (it = m_othTools.begin(); it != m_othTools.end(); it++) {
                    int tmpId = *it;
                    if (m_toolInfos[tmpId].orderNum > info.orderNum) {
                        m_othTools.insert(it, id);
                        bOK = false;
                        break;
                    }
                }
                if (bOK) {
                    m_othTools.push_back(id);
                }
            }
            
            if (info.crt_order > 0) {
                m_makeTools.push_back(id);
            }
            
            
            switch (info.pages) {
                case 1:{
                    bool bOK = true;
                    for (it = m_warList.begin(); it != m_warList.end(); it++) {
                        int tmpId = *it;
                        if (m_toolInfos[tmpId].store_order > info.store_order) {
                            m_warList.insert(it, id);
                            bOK = false;
                            break;
                        }
                    }
                    if (bOK) {
                        m_warList.push_back(id);
                    }
                    break;
                }
                case 2:{
                    bool bOK = true;
                    for (it = m_buffList.begin(); it != m_buffList.end(); it++) {
                        int tmpId = *it;
                        if (m_toolInfos[tmpId].store_order > info.store_order) {
                            m_buffList.insert(it, id);
                            bOK = false;
                            break;
                        }
                    }
                    if (bOK) {
                        m_buffList.push_back(id);
                    }
                    break;
                }
                case 3:{
                    bool bOK = true;
                    for (it = m_resList.begin(); it != m_resList.end(); it++) {
                        int tmpId = *it;
                        if (m_toolInfos[tmpId].store_order > info.store_order) {
                            m_resList.insert(it, id);
                            bOK = false;
                            break;
                        }
                    }
                    if (bOK) {
                        m_resList.push_back(id);
                    }
                    break;
                }
                default:{
                    bool bOK = true;
                    for (it = m_otherList.begin(); it != m_otherList.end(); it++) {
                        int tmpId = *it;
                        if (m_toolInfos[tmpId].store_order > info.store_order) {
                            m_otherList.insert(it, id);
                            bOK = false;
                            break;
                        }
                    }
                    if (bOK) {
                        m_otherList.push_back(id);
                    }
                    break;
                }
            }
            
        }
    }
    
    int num = m_warTools.size();
    for (int i=0;i<num;i++) {
        m_allTools.push_back(m_warTools[i]);
    }
    num = m_spdTools.size();
    for (int i=0;i<num;i++) {
        m_allTools.push_back(m_spdTools[i]);
    }
    num = m_resTools.size();
    for (int i=0;i<num;i++) {
        m_allTools.push_back(m_resTools[i]);
    }
    num = m_othTools.size();
    for (int i=0;i<num;i++) {
        m_allTools.push_back(m_othTools[i]);
    }
    
    m_mateTools = m_typeTools[7];
    CCLOG("test");
}

ToolController::~ToolController() {}

ToolController* ToolController::getInstance() {
    if (!_instance) {
        _instance = new ToolController();
    }
    return _instance;
}

void ToolController::purgeData() {
    delete _instance;
    _instance = NULL;
}

bool sortMakeTool(int item1, int item2)
{
    auto& tool1 = ToolController::getInstance()->getToolInfoById(item1);
    auto& tool2 = ToolController::getInstance()->getToolInfoById(item2);
    if (tool1.crt_order > tool2.crt_order) {
        return true;
    }else {
        return false;
    }
}

void ToolController::initToolData(CCDictionary* dict)
{
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("items"));
    CCDictionary* item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        int id = item->valueForKey("itemId")->intValue();
        if (m_toolInfos.find(id) != m_toolInfos.end()) {
            m_toolInfos[id].SetInfoFromServer(item,true);
        }
    }
    
    sort(m_makeTools.begin(), m_makeTools.end(), sortMakeTool);
}

void ToolController::initToolReward(CCDictionary* dict)
{
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("store"));
    CCDictionary* item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        int id = item->valueForKey("id")->intValue();
        CCArray* rewards = dynamic_cast<CCArray*>(item->objectForKey("reward"));
        if (rewards) {
            for (int j=0; j<rewards->count(); j++) {
                auto redItem = _dict(rewards->objectAtIndex(j));
                ToolRewardInfo info = ToolRewardInfo(redItem);
                m_toolReward[id].push_back(info);
            }
        }
    }
}

void ToolController::pushAddTool(CCDictionary* dict)
{
    int id = dict->valueForKey("itemId")->intValue();
    if (m_toolInfos.find(id) != m_toolInfos.end()) {
        m_toolInfos[id].SetInfoFromServer(dict);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_CHANGE);
    }
}

void ToolController::pushDelTool(CCDictionary* dict)
{
    int id = dict->valueForKey("itemId")->intValue();
    if (m_toolInfos.find(id) != m_toolInfos.end()) {
        m_toolInfos[id].SetInfoFromServer(dict);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_CHANGE);
    }
}

bool ToolController::buyLongJingEquip(int itemId, int num, CCCallFunc* callBackFunc, float price,bool buyAndUse,bool batchbuy,string fromView){
//    ToolInfo& info = getToolInfoById(itemId);
//    float cntPrice = num*info.price;
//    if (price>0) {
//        cntPrice = price;
//    }
//    if (cntPrice > GlobalData::shared()->playerInfo.gold) {
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_CHANGE);
//        YesNoDialog::gotoPayTips();
//        return false;
//    }
    
    m_callBackFunc = callBackFunc;
    if(batchbuy){
        ToolBuyCommand* cmd = new ToolBuyCommand(LONG_BUY_COMMAND, CC_ITOA(itemId), num,buyAndUse,1);
        cmd->sendAndRelease();
    }else{
        ToolBuyCommand* cmd = new ToolBuyCommand(LONG_BUY_COMMAND, CC_ITOA(itemId), num,buyAndUse);
        cmd->sendAndRelease();
    }
    return true;

}
void ToolController::retBuyLongJingEquip(CCDictionary* dict,bool buyAndUse,int buyCount)
{
    if (dict->objectForKey("errorCode")) {
        return;
    }
    else {
        CCDictionary* equip = _dict(dict->objectForKey("dragonShard"));
        int itemId = equip->valueForKey("itemId")->intValue();
        string uuid = equip->valueForKey("uuid")->getCString();
        EquipmentController::getInstance()->m_MyEquipmentMap[uuid] = itemId;
        EquipmentController::getInstance()->LongEquipmentInfoMap[uuid]=EquipmentController::getInstance()->EquipmentInfoMap[itemId];
        EquipmentInfo &equipInfo = EquipmentController::getInstance()->LongEquipmentInfoMap[uuid];
        string extraProperty = equip->valueForKey("extraProperty")->getCString();
        equipInfo.setLongProperty(extraProperty);
        
        if (dict->valueForKey("remainCrystal")->intValue()) {
            int longJing = dict->valueForKey("remainCrystal")->intValue();
            UIComponent::getInstance()->updateLongJing(longJing);
        }
        
        if (m_callBackFunc) {
            m_callBackFunc->execute();
            m_callBackFunc = NULL;
        }
        if(buyAndUse==false){
            std::string showTip = _lang(equipInfo.name);
//            if(buyCount>1){
//                showTip.append(" x ");
//                showTip.append(CC_ITOA(buyCount));
//            }
            CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), "", _lang_1("104918",showTip.c_str() ));
        }
    }
}

bool ToolController::buyTool(int itemId, int num, CCCallFunc* callBackFunc, float price,bool buyAndUse,bool batchbuy,string fromView)
{
    ToolInfo& info = getToolInfoById(itemId);
    float cntPrice = num*info.price;
    if (price>0) {
        cntPrice = price;
    }
    if (cntPrice > GlobalData::shared()->playerInfo.gold) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_CHANGE);
        YesNoDialog::gotoPayTips();
        return false;
    }
    
    m_callBackFunc = callBackFunc;
    if(batchbuy){
        ToolBuyCommand* cmd = new ToolBuyCommand(TOOL_BUY_COMMAND, CC_ITOA(itemId), num,buyAndUse,1);
        cmd->sendAndRelease();
    }else{
        ToolBuyCommand* cmd = new ToolBuyCommand(TOOL_BUY_COMMAND, CC_ITOA(itemId), num,buyAndUse);
        cmd->sendAndRelease();
    }
    if(GlobalData::shared()->analyticID!="beta"){
          FBUtilies::appEventPurchaseItem(info.page, GlobalData::shared()->playerInfo.level, FunBuildController::getInstance()->getMainCityLv(), info.itemId, CC_ITOA(info.itemId),(int) info.price, num, (int) (info.price*num), fromView);
    }
    return true;
}
bool ToolController::buyHotTool(int itemId, int num, CCCallFunc* callBackFunc, float price,string fromView)
{
    ToolInfo& info = getToolInfoById(itemId);
    float cntPrice = num*info.hotprice;
    if (price>0) {
        cntPrice = price;
    }
    if (cntPrice > GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        return false;
    }
    
    m_callBackFunc = callBackFunc;
    ToolHotBuyCommand* cmd = new ToolHotBuyCommand(TOOL_HOT_BUY_COMMAND, CC_ITOA(itemId), num,info.hotprice);
    cmd->sendAndRelease();
    if(GlobalData::shared()->analyticID!="beta"){
        FBUtilies::appEventPurchaseItem(info.page, GlobalData::shared()->playerInfo.level, FunBuildController::getInstance()->getMainCityLv(), info.itemId, CC_ITOA(info.itemId),(int) info.price, num, (int) (info.price*num), fromView);
    }
    return true;
}
bool ToolController::useItemChnageName(int itemId,std::string nickName){
    ToolInfo& info = getToolInfoById(itemId);
    ChangeNickNameCommand* cmd = new ChangeNickNameCommand(nickName, info.uuid);
    cmd->sendAndRelease();
    if(info.getCNT() > 0){
        useTool(ITEM_RENAME, 1, false);
    }
    return true;
}

bool ToolController::useItemChnagePic(int itemId,std::string pic){
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(ITEM_CHANGE_PIC);
    ChangePicCommand* cmd = new ChangePicCommand(pic);
    cmd->sendAndRelease();
    ToolController::getInstance()->useTool(ITEM_CHANGE_PIC, 1, false);
    return true;
}
void ToolController::retMerchantItems(CCDictionary* dict){
    m_merchantCurrIndex = 0;
    GameController::getInstance()->removeWaitInterface();
    m_toolMerchantInfos.clear();
    if (dict->objectForKey("errorCode")) {
        return;
    }
    CCString *str = dynamic_cast<CCString*>(dict->objectForKey("refreshTime"));
    if(str && str->getCString()){
        m_merchantRefreshTime = str->doubleValue();
    }
    str = dynamic_cast<CCString*>(dict->objectForKey("goldCost"));
    if(str && str->getCString()){
        m_merchantRefreshCost = str->intValue();
    }
    CCDictionary *bestItem = dynamic_cast<CCDictionary*>(dict->objectForKey("bestItem"));
    if(bestItem){
        vector<std::string> tmp;
        std::string goods = bestItem->valueForKey("goodsId")->getCString();
        CCCommonUtils::splitString(goods, ";", tmp);
        if(tmp.size()>1){
            merchantBestTool.itemId = atoi(tmp.at(0).c_str());
            merchantBestTool.itemNum = atoi(tmp.at(1).c_str());
        }else{
            merchantBestTool.itemId = 0;
            merchantBestTool.itemNum = 0;
        }
        
        if(bestItem->objectForKey("type")){
            merchantBestTool.type =  (MerchantToolType)(bestItem->valueForKey("type")->intValue());
        }else{
            merchantBestTool.type = MerchantTool_GOODS;
        }
        if(bestItem->objectForKey("priceType")){
            merchantBestTool.priceType = bestItem->valueForKey("priceType")->intValue();
        }else{
            merchantBestTool.priceType = 0;
        }
        if(bestItem->objectForKey("price")){
            merchantBestTool.price =bestItem->valueForKey("price")->intValue();
        }else{
            merchantBestTool.price = 0;
        }
        merchantBestTool.color = bestItem->valueForKey("color")->intValue();
        if(bestItem->objectForKey("price_hot")){
            merchantBestTool.priceHot = bestItem->valueForKey("price_hot")->intValue();
        }else{
            merchantBestTool.priceHot = 0;
        }
    }
    std::vector<MerchantToolInfo> tmpTools;
    CCArray* arrHot = dynamic_cast<CCArray*>(dict->objectForKey("hotItem"));
    CCObject* obj = NULL;
    CCARRAY_FOREACH(arrHot, obj){
        CCDictionary* itemDict = dynamic_cast<CCDictionary*>(obj);
        MerchantToolInfo info = MerchantToolInfo(itemDict);
        if(info.itemId<WorldResource_Max){
            switch (info.itemId) {
                case Wood:{
                    info.orderNum = 400;
                    break;
                }
                case Food:{
                    info.orderNum = 401;
                    break;
                }
                case Iron:{
                    info.orderNum = 402;
                    break;
                }
                case Stone:{
                    info.orderNum = 403;
                    break;
                }
                default:{
                    info.orderNum = 404;
                    break;
                }
            }
            info.type = MerchantTool_RESOURCE;
        }else{
            ToolInfo &tool = getToolInfoById(info.itemId);
            info.orderNum = tool.orderNum;
        }
        tmpTools.push_back(info);
    }
    std::string saveString = CCUserDefault::sharedUserDefault()->getStringForKey(TOOL_MERCHANTE_SAVE_INDEX_KEY);
    if(saveString.empty()){
        sort(tmpTools.begin(),tmpTools.end(),sortMerchantItemsByOder);
        m_toolMerchantInfos=tmpTools;
        saveMerchanteIndex();
    }else{
        bool isFind = true;
        std::vector<std::string> splitStr;
        CCCommonUtils::splitString(saveString, ",", splitStr);
        if(splitStr.size() != tmpTools.size()){
            isFind=false;
        }else{
            auto iter = tmpTools.begin();
            while (iter!=tmpTools.end()) {
                std::string findStr = CC_ITOA((*iter).itemId) + std::string(":")+CC_ITOA((*iter).itemNum);
                if(find(splitStr.begin(), splitStr.end(), findStr)==splitStr.end()){
                    isFind=false;
                    break;
                }
                ++iter;
            }
        }
        if(isFind==false){
            sort(tmpTools.begin(),tmpTools.end(),sortMerchantItemsByOder);
            m_toolMerchantInfos=tmpTools;
            saveMerchanteIndex();
        }else{
            std::vector<std::string>::iterator iter1 = splitStr.begin();
            while (iter1!=splitStr.end()) {
                std::vector<std::string> splitStr1;
                CCCommonUtils::splitString((*iter1), ":", splitStr1);
                if(splitStr1.size()>1){
                    int itemid = atoi(splitStr1[0].c_str());
                    int itemnum = atoi(splitStr1[1].c_str());
                    auto iter = tmpTools.begin();
                    while (iter!=tmpTools.end()) {
                        if((*iter).itemId == itemid && (*iter).itemNum == itemnum){
                            m_toolMerchantInfos.push_back((*iter));
                            tmpTools.erase(iter);
                            break;
                        }
                        ++iter;
                    }
                }
                ++iter1;
            }
        }
    }
}
void ToolController::saveMerchanteIndex(){
    std::string saveString = "";
    std::vector<MerchantToolInfo>::iterator iter = m_toolMerchantInfos.begin();
    while (iter!=m_toolMerchantInfos.end()) {
        std::string tmp = CC_ITOA((*iter).itemId) +std::string(":")+ CC_ITOA((*iter).itemNum);
        saveString = saveString.empty()? tmp:saveString+","+tmp;
        ++iter;
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(TOOL_MERCHANTE_SAVE_INDEX_KEY,saveString);
    CCUserDefault::sharedUserDefault()->flush();
}
bool ToolController::sortMerchantItemsByOder(const MerchantToolInfo& info1,const MerchantToolInfo& info2){
    return (info1.orderNum<info2.orderNum);
}
void ToolController::buyMerchantTool(const string itemId,const int itemNum,const int price,const int priceType,CCCallFunc* callBackFunc,const int num){
    if(m_merchantCurrIndex>0){
        if(priceType < WorldResource_Max){
            if (CCCommonUtils::isEnoughResourceByType(priceType,price)) {
                m_callBackFunc = callBackFunc;
                ToolMerchantBuyCommand* cmd = new ToolMerchantBuyCommand(itemId, itemNum,num,price,priceType);
                cmd->sendAndRelease();
            }else{
                YesNoDialog::gotoPayTips();
            }
        }else{
            m_callBackFunc = callBackFunc;
            ToolMerchantBuyCommand* cmd = new ToolMerchantBuyCommand(itemId, itemNum,num,price,priceType);
            cmd->sendAndRelease();
        }
    }
}
void ToolController::retBuyMerchantTool(CCDictionary* dict){
    if (dict->objectForKey("errorCode")) {
        return;
    }
    //更新买到的物品信息
    if(dict->objectForKey("item")){
        CCDictionary* item = _dict(dict->objectForKey("item"));
        int itemId = item->valueForKey("itemId")->intValue();
        ToolInfo& info = getToolInfoById(itemId);
        info.SetInfoFromServer(item);
    }
    if(dict->objectForKey("equip")){
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("equip"));
        EquipmentController::getInstance()->addEquip(arr);
    }
    
    //更新售卖的物品信息
    if(dict->objectForKey("hotItem") && m_merchantCurrIndex>0){
        int index = m_merchantCurrIndex-1;
        MerchantToolInfo *minfo = &m_toolMerchantInfos.at(index);
        int oldItemid = minfo->itemId;
        std::string showTip = std::string(" x") + CC_ITOA(minfo->itemNum);
//        if(oldItemid<WorldResource_Max){
//            showTip = CCCommonUtils::getResourceNameByType(oldItemid) + showTip;
//            CCCommonUtils::flyHintWithDefault(CCCommonUtils::getResourceIconByType(oldItemid), "", _lang_1("104918", showTip.c_str()));
//        }else{
            if(minfo->type == MerchantTool_RESOURCE){
                showTip = CCCommonUtils::getResourceNameByType(oldItemid) + showTip;
                CCCommonUtils::flyHintWithDefault(CCCommonUtils::getResourceIconByType(oldItemid), "", _lang_1("104918", showTip.c_str()));
            }else if(minfo->type == MerchantTool_GOODS){
                ToolInfo& tool = getToolInfoById(oldItemid);
                showTip = tool.getName() + showTip;
                CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(oldItemid)), "", _lang_1("104918", showTip.c_str()));
            }else if(minfo->type == MerchantTool_EQUIP){
                auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[oldItemid];
                showTip = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level) + showTip;
            }
//        }
        if(minfo->priceType>WorldResource_Max){
            auto &useTool = ToolController::getInstance()->getToolInfoById(minfo->priceType);
            useTool.setCNT(useTool.getCNT() - minfo->price);
        }
        CCDictionary* hotitem = _dict(dict->objectForKey("hotItem"));
        minfo->SetInfoFromServer(hotitem);
        if (m_callBackFunc) {
            m_callBackFunc->execute();
            m_callBackFunc = NULL;
        }
        saveMerchanteIndex();
    }
}
void ToolController::retHotItems(CCDictionary* dict){
    GameController::getInstance()->removeWaitInterface();
    if (dict->objectForKey("errorCode")) {
        return;
    }
    CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("hotItem"));
    CCObject* obj;
    CCARRAY_FOREACH(arr, obj){
        CCDictionary* itemDict = dynamic_cast<CCDictionary*>(obj);
        int itemId = itemDict->valueForKey("itemId")->intValue();
        int num = itemDict->valueForKey("num")->intValue();
        int price = itemDict->valueForKey("price")->intValue();
        ToolInfo& info = getToolInfoById(itemId);
        info.hotnum = num;
        info.hotprice = price;
        info.ishot=true;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_HOT_GET_RETURN);
}
void ToolController::retBuyTool(CCDictionary* dict,bool buyAndUse,int buyCount)
{
    if (dict->objectForKey("errorCode")) {
        return;
    }
    else {
        CCDictionary* item = _dict(dict->objectForKey("item"));
        int itemId = item->valueForKey("itemId")->intValue();
        ToolInfo& info = getToolInfoById(itemId);
        info.SetInfoFromServer(item,buyAndUse);
        
        if(dict->objectForKey("hotItem")){
            CCDictionary* hotitem = _dict(dict->objectForKey("hotItem"));
            int num = hotitem->valueForKey("num")->intValue();
            info.hotnum = num;
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_CHANGE);
        
        int tmpInt = dict->valueForKey("remainGold")->intValue();
        UIComponent::getInstance()->updateGold(tmpInt);
        
        if (dict->valueForKey("remainCrystal")->intValue()) {
            int longJing = dict->valueForKey("remainCrystal")->intValue();
            UIComponent::getInstance()->updateLongJing(longJing);
        }
        
        if (m_callBackFunc) {
            m_callBackFunc->execute();
            m_callBackFunc = NULL;
        }
        if(buyAndUse==false){
            std::string showTip = info.getName();
            if(buyCount>1){
                showTip.append(" x ");
                showTip.append(CC_ITOA(buyCount));
            }
            CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), "", _lang_1("104918",showTip.c_str() ));
        }
    }
}

std::vector<ToolRewardInfo>* ToolController::findToolRewards(int itemId)
{
    if (m_toolReward.find(itemId)!=m_toolReward.end()) {
        return &(m_toolReward[itemId]);
    }
    return NULL;
}

void ToolController::addTool(int itemId, int num, string uuid)
{
    ToolInfo& info = getToolInfoById(itemId);
    info.setCNT(num);
    if (uuid!="") {
        info.uuid = uuid;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_CHANGE,CCInteger::create(itemId));
}

bool ToolController::useTool(int itemId, int num, bool isSend,bool isByBuy)
{
    ToolInfo& info = getToolInfoById(itemId);
    if (info.getCNT() < num) {
        return false;
    }
    
    bool flag = QueueController::getInstance()->checkMarchStatus();
    if (info.type==4 && info.type2==1 && flag) {
        CCCommonUtils::flyHint("", "", _lang("101439"));
        return false;
    }
    
    int mlv = FunBuildController::getInstance()->getMainCityLv();
    if (mlv<info.limitUseLv) {
        CCCommonUtils::flyHint("", "", _lang_2("104917", CC_ITOA(info.limitUseLv), info.getName().c_str()));
        return false;
    }
    if(info.itemId == ITEM_ADD_STAMINE){
        if(WorldController::getInstance()->currentStamine >= GlobalData::shared()->worldConfig.stamineMax){
            CCCommonUtils::flyHint("", "", _lang("103746"));
            return false;
        }
    }
    if(info.type == 3) {
        if (info.type2 == Iron) {
            int mlv = FunBuildController::getInstance()->getMainCityLv();
            if (mlv<FunBuildController::getInstance()->building_base_k3) {
                CCCommonUtils::flyHint("", "", _lang_2("104917", CC_ITOA(FunBuildController::getInstance()->building_base_k3), info.getName().c_str()));
                return false;
            }
        }
        if (info.type2 == Stone) {
            int mlv = FunBuildController::getInstance()->getMainCityLv();
            if (mlv<FunBuildController::getInstance()->building_base_k4) {
                CCCommonUtils::flyHint("", "", _lang_2("104917", CC_ITOA(FunBuildController::getInstance()->building_base_k4), info.getName().c_str()));
                return false;
            }
        }
    }
    
    if (info.type == 4 && isSend && !isByBuy) {//状态道具
        m_willUseItem = itemId;
        if ( checkUseStateTool(itemId,CCCallFunc::create(this, callfunc_selector(ToolController::onOkUse))) ){
            return false;
        }
    }
    
    if (isSend)
    {
        ToolUseCommand* cmd = new ToolUseCommand(TOOL_USE_COMMAND, info.uuid, num);
        cmd->sendAndRelease();
    }
    else {
        int tCnt = info.getCNT() - num;
        info.setCNT(tCnt);
        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), _lang("104908"), _lang_1("104909", info.getName().c_str()));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOOL_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA,CCInteger::create(itemId));
    }
    return true;
}

void ToolController::retUseTool(CCDictionary* dict)
{
//    retObj.putUtfString("itemId", itemId);
//    retObj.putInt("count", item.getCount());
//    retObj.putSFSObject("itemEffectObj", itemEffectObj);
    
    if (dict->objectForKey("errorCode")) {
        const CCString *pStr = dict->valueForKey("errorCode");
        if (pStr->compare("104928")==0 || pStr->compare("104929") == 0) {
            CCCommonUtils::flyText(_lang(pStr->getCString()));
        }
        return;
    }
    else {
        int itemId = dict->valueForKey("itemId")->intValue();
        int count = dict->valueForKey("count")->intValue();
        
        ToolInfo& info0 = getToolInfoById(itemId);
        int cnt0 = info0.getCNT();
        
        addTool(itemId, count);
        ToolInfo& info = getToolInfoById(itemId);
        
        int cnt1 = count;
        string name = info.getName();
        int dcnt = cnt0 - cnt1;
        if (dcnt > 1) {
            string str = CC_ITOA(dcnt);
            name += " * ";
            name += str;
        }
        
        if(info.type == 3){
            CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), "", _lang_1("104913", name.c_str()));
        }else if(info.type != 5&&itemId!=ITEM_FIREWORK1&&itemId!=ITEM_FIREWORK2&&itemId!=0 && itemId!=ITEM_CITY_DEF_FULL){
            CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), _lang("104908"), _lang_1("104909", name.c_str()));
        }
        if(info.type == 5){
            CCLog("【优化】宝箱获得物品展示弹窗 ");
            PopupViewController::getInstance()->addPopupView(ChestUseView::create(dict),false,false);
        }


        if(itemId >= ITEM_QUEUE_OPEN_BEGIN && itemId <= ITEM_QUEUE_OPEN_END)
        {
            CCDictionary* queueDict = _dict(dict->objectForKey("itemEffectObj"));
            QueueController::getInstance()->openQueue(queueDict);
        }
//        else if(itemId == atoi(GlobalData::shared()->tavernConfig.freeItemId.c_str())){
//            GeneralGambleController::getInstance()->setTavernFreeTime(GeneralGambleController::getInstance()->getTavernFreeTime() - 1);
//        }
        
        if(dict && dict->objectForKey("itemEffectObj"))
        {
            auto effectObj = _dict(dict->objectForKey("itemEffectObj"));
            
            if (effectObj->objectForKey("oldStatus")) {//删除该状态的作用
                int reStatusId = effectObj->valueForKey("oldStatus")->intValue();
                if (GlobalData::shared()->statusMap.find(reStatusId) != GlobalData::shared()->statusMap.end()) {
                    GlobalData::shared()->statusMap[reStatusId] = 0;
                }
            }
            
            if (effectObj->objectForKey("effectState")) {
                auto stateDict = _dict(effectObj->objectForKey("effectState"));
                CCDictElement* element;
                CCDICT_FOREACH(stateDict, element)
                {
                    string key = element->getStrKey();
                    int effectId = atoi(key.c_str());
                    if(effectId>=PLAYER_PROTECTED_TIME1 && effectId<=PLAYER_PROTECTED_TIME5){
                        GlobalData::shared()->playerInfo.protectTimeStamp = stateDict->valueForKey(key)->doubleValue();
                        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                            WorldMapView::instance()->m_map->updateDynamicMap(WorldController::getInstance()->selfPoint);
                        }
                    }
                    double time = stateDict->valueForKey(key)->doubleValue()/1000;
                    if (time>0) {
                        time = GlobalData::shared()->changeTime(time);
                    }
                    if (key!="startTime") {
                        GlobalData::shared()->statusMap[atoi(key.c_str())] = time;
                    }
                    map<int, CCDictionary* >::iterator it = m_statusItems.find(info.type2);
                    CCObject* obj = element->getObject();
                    CCString* str = (CCString*)obj;
                    if(info.type==4){
                        if(it!=m_statusItems.end()){
                            auto dic = it->second;
                            if(!dic->objectForKey("startTime")){
                                dic->setObject(CCString::create(CC_ITOA(WorldController::getInstance()->getTime())), "startTime");
                            }else if(key!="" && key!="startTime"){
                                dic->setObject(CCString::create(str->getCString()), "endTime");
                            }else if(key=="startTime"){
                                dic->setObject(CCString::create(str->getCString()), "startTime");
                            }
                            m_statusItems[info.type2] = dic;
                        }else{
                            auto infoDic = CCDictionary::create();
                            infoDic->retain();
                            if(key!="" && key!="startTime"){
                                infoDic->setObject(CCString::create(str->getCString()), "endTime");
                            }else if(key=="startTime"){
                                infoDic->setObject(CCString::create(str->getCString()), "startTime");
                            }
                            m_statusItems[info.type2] = infoDic;
                        }
                    }
                    if(info.type2 == 100){
                        checkMaincityCustom();
                    }
                }
            }
            if(info.type==4){
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ITME_STATUS_TIME_CHANGE,CCInteger::create(info.type2));
            }
            if (effectObj->objectForKey("status")) {
                auto arr = dynamic_cast<CCArray*>(effectObj->objectForKey("status"));
                CCDictionary *item = NULL;
                for (int i=0; i<arr->count(); i++) {
                    item = _dict(arr->objectAtIndex(i));
                    auto effState = stateEffect();
                    effState.effectId = item->valueForKey("effNum")->intValue();
                    effState.value = item->valueForKey("effVal")->intValue();
                    effState.stateId = item->valueForKey("stateId")->intValue();
                    
                    if (true) {//去除重复的stateId
                        vector<stateEffect>::iterator it = GlobalData::shared()->effectStateMap[effState.effectId].begin();
                        for (; it != GlobalData::shared()->effectStateMap[effState.effectId].end(); it++) {
                            if (effState.stateId == it->stateId) {
                                GlobalData::shared()->effectStateMap[effState.effectId].erase(it);
                                break;
                            }
                        }
                    }
                    
                    GlobalData::shared()->effectStateMap[effState.effectId].push_back(effState);
                }
            }
            
//            if (effectObj->objectForKey("reward")) {
//                auto arr = dynamic_cast<CCArray*>(effectObj->objectForKey("reward"));
//                std::string rewardStr = RewardController::getInstance()->retReward(arr);
//                if(info.type == 5){
//                    int totalLen = Utf8Utils::strlen(rewardStr.c_str());
//                    if(totalLen>128){
//                        rewardStr = CCCommonUtils::subStrByUtf8(rewardStr,0,50);
//                        rewardStr.append("...");
//                    }
//                    CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), "", rewardStr);
//                }
//            }
        
            if(effectObj->objectForKey("stamina")){
                WorldController::getInstance()->currentStamine = effectObj->valueForKey("stamina")->intValue();
                WorldController::getInstance()->lastStamineTime = effectObj->valueForKey("lastStaminaTime")->doubleValue();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
            }
            
            if (effectObj->objectForKey("remainGold")) { //"delta" 是金币差值
                int tmpGlod = effectObj->valueForKey("remainGold")->intValue();
                UIComponent::getInstance()->updateGold(tmpGlod);
            }
            if (effectObj->objectForKey("remainCrystal")) { //"delta" 是差值
                int tmpCrystal = effectObj->valueForKey("remainCrystal")->intValue();
                UIComponent::getInstance()->updateLongJing(tmpCrystal);
            }
            if(effectObj->objectForKey("vip")){
                CCDictionary* vipInfo = dynamic_cast<CCDictionary*>(effectObj->objectForKey("vip"));
                auto &playerInfo = GlobalData::shared()->playerInfo;
                if (vipInfo)
                {
                    playerInfo.vipPoints = vipInfo->valueForKey("score")->intValue();
                    playerInfo.vipEndTime = vipInfo->valueForKey("vipEndTime")->doubleValue()/1000;
                    if (playerInfo.vipEndTime>0) {
                        playerInfo.vipEndTime = GlobalData::shared()->changeTime(playerInfo.vipEndTime);
                    }
                    playerInfo.nextDayLoginVipReward = vipInfo->valueForKey("nextDayScore")->intValue();
                    playerInfo.consecutiveLoginDays = vipInfo->valueForKey("loginDays")->intValue();
                    playerInfo.updateVipEffect();
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(VIP_REFRESH);

                }
            }
            if(effectObj->objectForKey("freeFromItem")){
                CCString *prayC = dynamic_cast<CCString*>(effectObj->objectForKey("freeFromItem"));
                if(prayC && prayC->getCString()){
                    int prayCount = atoi(prayC->getCString());
                    GlobalData::shared()->sacrificeInfo.setPrayCountFromItem( prayCount);
                }
            }
            if(effectObj->objectForKey("armyId")){
                CCString *armyStr= dynamic_cast<CCString*>(effectObj->objectForKey("armyId"));
                CCString *freeStr = dynamic_cast<CCString*>(effectObj->objectForKey("free"));
                if(armyStr && armyStr->getCString()  && freeStr && freeStr->getCString()){
                    int free = atoi(freeStr->getCString());
                    map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(armyStr->getCString());
                    if(it!=GlobalData::shared()->armyList.end()){
                        (*it).second.free = free;
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
                    }
                }
            }
            
            if(effectObj->objectForKey("cityDefValue") &&  effectObj->objectForKey("lastCityDefTime")){
                GlobalData::shared()->cityDefenseVal = effectObj->valueForKey("cityDefValue")->intValue();
                GlobalData::shared()->cityLastUpdateDefenseStamp = effectObj->valueForKey("lastCityDefTime")->doubleValue();
                GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
                CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(itemId)), "", _lang("104965"));
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA,CCInteger::create(itemId));
    }
}

ToolInfo& ToolController::getToolInfoById(int itemId)
{
    ToolInfo& ret = m_toolInfos[itemId];
    return ret;
}
int ToolController::getTotalCountInBag(){
    int count = 0;
    std::vector<int> m_dataList = (ToolController::getInstance()->m_allTools);
    for (int i=0; i<m_dataList.size(); i++) {
        int tmpToolId = (m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if (info.getCNT() > 0) {
            count++;
        }
        
    }
    return count;
}
bool ToolController::checkUseStateTool(int itemId,CCCallFunc* callFunc)
{
    auto& info = getToolInfoById(itemId);
//    int statusId = atoi(info.paras[1].c_str());
    
    auto statusDict = LocalController::shared()->DBXMLManager()->getObjectByKey(info.paras[1]);
    int overlap = statusDict->valueForKey("overlap")->intValue();
    int group = statusDict->valueForKey("group")->intValue();
    int gID = group/100;
    
    int findSid = 0;
    if (overlap==0) {
        if (m_statusGroup.find(gID) != m_statusGroup.end()) {
            int curTime = GlobalData::shared()->getWorldTime();
            for (int i=0; i<m_statusGroup[gID].size(); i++) {
                int tmpSid = m_statusGroup[gID][i];
                if (GlobalData::shared()->statusMap.find(tmpSid) != GlobalData::shared()->statusMap.end()) {
                    if (GlobalData::shared()->statusMap[tmpSid] > curTime) {
                        findSid = tmpSid;
                        break;
                    }
                }
            }
        }
    }
    
    int curStatusGid = 0;
    if (findSid>0) {
        string findGroup = CCCommonUtils::getPropById(CC_ITOA(findSid), "group");
        curStatusGid = atoi(findGroup.c_str());
    }
    
    if (curStatusGid>0) {
        if (group>=curStatusGid) {
            //可以覆盖
            YesNoDialog::show( _lang("101432").c_str() , callFunc);
        }
        else {
            YesNoDialog::showYesDialog(_lang("101433").c_str());
        }
        return true;
    }
    else {
        return false;
    }
}

void ToolController::onOkUse()
{
    ToolInfo& info = getToolInfoById(m_willUseItem);
    ToolUseCommand* cmd = new ToolUseCommand(TOOL_USE_COMMAND, info.uuid, 1);
    cmd->sendAndRelease();
}
int ToolController::getSPDItem(int speedType){
    std::map<int,ToolInfo>::iterator iter = m_toolInfos.begin();
    while (iter!=m_toolInfos.end()) {
        ToolInfo& info = (*iter).second;
        if(info.type == ITEM_TYPE_SPD && (info.type2 == speedType || info.type2 == ItemSpdMenu_ALL)){
            if(info.getCNT()>0)
                return info.itemId;
        }
        ++iter;
    }
    return -1;
}

int ToolController::getMateCntByType(int mateType)
{
    int ret = 0;
    for (int i=0; i<10; i++) {
        int tmpToolId = mateType+i;
        if (m_toolInfos.find(tmpToolId) != m_toolInfos.end() ) {
            ret += m_toolInfos[tmpToolId].getCNT();
        }
        else {
            break;
        }
    }
    return ret;
}

void ToolController::initMerchanteInfo(CCDictionary* dict){
//    if (dict->objectForKey("startTime")) {
//        CCString *s = dynamic_cast<CCString*>(dict->objectForKey("startTime"));
//        if(s && s->getCString()){
//            m_merchante_time_s1 = atof(s->getCString());
//        }
//    }
//    if (dict->objectForKey("endTime")) {
//        CCString *s = dynamic_cast<CCString*>(dict->objectForKey("endTime"));
//        if(s && s->getCString()){
//            m_merchante_time_e1 = atof(s->getCString());
//        }
//    }
    if(dict->objectForKey("isOpen")){
        if(dict->valueForKey("isOpen")->intValue() == 1){
            m_merchante_state = 2;
        }
    }
    if(dict->objectForKey("k8")){
         m_merchante_costTip = dict->valueForKey("k8")->intValue();
    }
}

void ToolController::addToNewGoodsList(int toolid){
    if(find(m_newGoodsList.begin(),m_newGoodsList.end(),toolid)==m_newGoodsList.end()){
        m_newGoodsList.push_back(toolid);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("showEquipTips");
    }
}
void ToolController::removeFromNewGoodsList(int itemid){
    auto iter = find(m_newGoodsList.begin(),m_newGoodsList.end(),itemid);
    if(iter!=m_newGoodsList.end()){
        m_newGoodsList.erase(iter);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_HAS_NEW_GOOD);
    }
}
bool ToolController::isNewVersionGoods(std::string checkV){
    if(checkV.empty())
        return false;
    std::vector<string> vector1;
    std::vector<string> vector2;
    CCCommonUtils::splitString(checkV, ".", vector1); //要求的版本
    CCCommonUtils::splitString(GlobalData::shared()->version, ".", vector2); //当前的版本
    if(vector1.size() == vector2.size()){
        int index = 0;
        while (index<vector1.size()) {
            int tmpV1 = atoi(vector1.at(index).c_str());
            int tmpV2 = atoi(vector2.at(index).c_str());
            if(tmpV1<tmpV2){
                return false;
            }else if(tmpV1>tmpV2){
                YesNoDialog::showYesDialog(_lang("104956").c_str());
                return true;
            }
            ++index;
        }
        return false;
    }
    YesNoDialog::showYesDialog(_lang("104956").c_str());
    return true;
}

//添加建造材料空位
bool ToolController::addMakeToolQueue(int num)
{
    ToolQueueAddCommand* cmd = new ToolQueueAddCommand(num);
    cmd->sendAndRelease();
    return true;
}
void ToolController::endAddMakeToolQueue(CCDictionary* dict){
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("updateLockCell",dict);
    }
}


//取消建造材料
bool ToolController::cancelMakeTool(int index)
{
    ToolQueueCancelCommand* cmd = new ToolQueueCancelCommand(index);
    cmd->sendAndRelease();
    return true;
}

void ToolController::endCancelMakeTool(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        string key = QueueController::getInstance()->addQueueInfo(dict);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MATETOOL_CREATE_END);
    }
}

//制作道具
bool ToolController::startMakeTool(int itemId)
{
    ToolCreateCommand* cmd = new ToolCreateCommand(itemId);
    cmd->sendAndRelease();
    return true;
}
void ToolController::retMakeTool(CCDictionary* dict)
{
    CCLOG("start create tool!");
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (dict->objectForKey("queue")) {
            auto queue = _dict(dict->objectForKey("queue"));
            string key = QueueController::getInstance()->addQueueInfo(queue);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MATETOOL_CREATE_END);
    }
}
bool ToolController::getMakeTool(string quuid)
{
    ToolHavestCommand* cmd = new ToolHavestCommand(quuid);
    cmd->sendAndRelease();
    return true;
}
void ToolController::retGetMakeTool(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (dict->objectForKey("add")) {
            CCDictionary* addDict = dynamic_cast<CCDictionary*>(dict->objectForKey("add"));
            int id = addDict->valueForKey("itemId")->intValue();
            pushAddTool(addDict);
            flyToolReward(id);
            CCCommonUtils::flyHint("", "", _lang_1("102381", CCCommonUtils::getNameById(CC_ITOA(id)).c_str()), 3, 0, false, 0.2 );
            
            if (dict->objectForKey("queue")) {
                auto queue = _dict(dict->objectForKey("queue"));
                if (queue->valueForKey("updateTime")->doubleValue()>0) {
                    string key = QueueController::getInstance()->addQueueInfo(queue);
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("addFunBuildState");
                }
                
            }
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MATETOOL_CREATE_END);
    }
}

void ToolController::flyToolReward(int itemId)
{
    if (m_toolInfos.find(itemId) == m_toolInfos.end()) {
        return;
    }
    
    int dyCnt = 1;
    
    auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
    int centerX = CCDirector::sharedDirector()->getWinSize().width / 2;
    int centerY = CCDirector::sharedDirector()->getWinSize().height / 2;
    
    auto modelLayer = CCModelLayerColor::create();
    modelLayer->setOpacity(125);
    modelLayer->setColor(ccBLACK);
    modelLayer->setContentSize(CCDirector::sharedDirector()->getWinSize());
    modelLayer->setAnchorPoint(ccp(0.5, 0.5));
    modelLayer->setZOrder(-1);
    topLayer->addChild(modelLayer);
    
    auto node = CCNode::create();
    node->setZOrder(1);
    if (true) {
        auto particle1 = ParticleController::createParticle("MallBag_1");
        auto particle2 = ParticleController::createParticle("MallBag_2");
        auto particle3 = ParticleController::createParticle("MallBag_3");
        particle1->setPosition(0, 65);
        particle2->setPosition(0, 65);
        particle3->setPosition(0, 65);
        node->addChild(particle1);
        node->addChild(particle2);
        node->addChild(particle3);
    }
    
    auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(itemId)).c_str(),true,CCLoadSpriteType_GOODS);
    sprite->setAnchorPoint(ccp(0.5, 0));
    CCCommonUtils::setSpriteMaxSize(sprite, 130, true);
    node->addChild(sprite);
    
    auto label = CCLabelIF::create();
    label->setColor(ccGREEN);
    std::string valueStr = "";
    valueStr.append("+ 1");
    label->setString(valueStr.c_str());
    label->setFontSize(24);
    label->setAnchorPoint(ccp(0.5, 0));
    node->addChild(label);
    
    int endY = -80;//node->getPositionY() + 150;
    int endX = centerX;//node->getPositionX();
    
    CCActionInterval * scaleto = CCScaleTo ::create(0.5, 1);
    CCActionInterval * easeBackOut1 = CCEaseBackOut ::create(scaleto);
    CCActionInterval * moveto = CCMoveTo ::create(0.5, ccp(centerX, dyCnt * 90 + centerY));
    CCActionInterval * easeBackOut2 = CCEaseBackOut ::create(moveto);
    CCFiniteTimeAction * spawn1 =CCSpawn::create(easeBackOut1,easeBackOut2,NULL);
    
    CCActionInterval * delayT = CCDelayTime ::create(1.25);
    
    CCActionInterval * scaleto2 = CCScaleTo ::create(0.5, 0);
    CCActionInterval * CCEaseBackIn1 = CCEaseBackIn ::create(scaleto2);
    CCActionInterval * moveto2 = CCMoveTo ::create(0.5, ccp(endX, endY));
    CCActionInterval * CCEaseBackIn2 = CCEaseBackIn ::create(moveto2);
    CCFiniteTimeAction * spawn2 =CCSpawn::create(CCEaseBackIn1,CCEaseBackIn2,NULL);
    
    node->setScale(0);
    node->setPosition(centerX, centerY);
    topLayer->addChild(node);
//    SoundController::sharedSound()->playEffects(Music_Sfx_UI_loteryrwd);
    node->runAction(CCSequence::create(spawn1,delayT,spawn2,CCCallFuncO::create(this, callfuncO_selector(ToolController::aniComplete),node), CCCallFuncO::create(this, callfuncO_selector(ToolController::removeLayerColor),modelLayer), NULL));
}
void ToolController::aniComplete(CCObject* p){
    auto node = dynamic_cast<CCNode*>(p);
    if(node->getParent()){
        node->removeFromParent();
    }
}
void ToolController::removeLayerColor(CCObject* p){
    auto layerColor = dynamic_cast<CCModelLayerColor*>(p);
    if(layerColor->getParent()){
        layerColor->removeFromParent();
    }
}
void ToolController::checkMaincityCustom(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ToolController::resetMaincityCustom), this);
    m_bMaincityResource=false;
    auto iter = GlobalData::shared()->statusMap.begin();
    while (iter!=GlobalData::shared()->statusMap.end()) {
        auto iter1 = ToolController::getInstance()->m_customSkinGroup.find((*iter).first);
        if(iter1!=ToolController::getInstance()->m_customSkinGroup.end()){
            double nowT = GlobalData::shared()->getWorldTime();
            float leftT = nowT - (*iter).second;
            if(leftT < 0.0001){
                CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ToolController::resetMaincityCustom), this, 0, 0, -leftT,false);
                if(DynamicResourceController::getInstance()->checkMainCityResource((*iter1).second.skin, (*iter1).second.md5)==true){
                    m_maincitySkin = (*iter1).second.skin;
                    DynamicResourceController::getInstance()->initMainCityConfig(m_maincitySkin, maincityConfig);
                    m_bMaincityResource=true;
                    SceneController::getInstance()->setMaincityCustom(true);
                    return;
                }else{
                    m_bMaincityResource=false;
                    m_maincitySkin = (*iter1).second.skin;
                    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ToolController::setMaincityCustom), MSG_DYNAMIC_DOWNLOAD_FINISH, NULL);
                }
                break;
            }
        }
        ++iter;
    }
    SceneController::getInstance()->setMaincityCustom(false);
}
void ToolController::setMaincityCustom(CCObject *ccObj){
    if(ccObj){
        CCString *str = dynamic_cast<CCString*>(ccObj);
        if(str && str->getCString() && !m_maincitySkin.empty() && m_maincitySkin.compare(str->getCString()) ==0 ){
            DynamicResourceController::getInstance()->initMainCityConfig(m_maincitySkin, maincityConfig);
            m_bMaincityResource=true;
            CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_DYNAMIC_DOWNLOAD_FINISH);
            SceneController::getInstance()->setMaincityCustom(true);
        }
    }
}
void ToolController::resetMaincityCustom(float fupdate){
    m_maincitySkin = "";
    m_bMaincityResource=false;
    maincityConfig.clear();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ToolController::resetMaincityCustom), this);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
        SceneController::getInstance()->setMaincityCustom(true);
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if(layer){
            layer->removeCustomBatchLayer();
        }
    }
}
void ToolController::cancelCustomSkin(){
    auto iter = GlobalData::shared()->statusMap.begin();
    while (iter!=GlobalData::shared()->statusMap.end()) {
        auto iter1 = ToolController::getInstance()->m_customSkinGroup.find((*iter).first);
        if(iter1!=ToolController::getInstance()->m_customSkinGroup.end()){
            double nowT = GlobalData::shared()->getWorldTime();
            float leftT = nowT - (*iter).second;
            if(leftT < 0.0001){
                m_maincitySkin = "";
                m_bMaincityResource=false;
                maincityConfig.clear();
                CancelStatusItemCommand* cmd = new CancelStatusItemCommand((*iter1).first);
                cmd->sendAndRelease();
                break;
            }
        }
        ++iter;
    }
}
bool ToolController::canChangeSkin(){
    if(!m_maincitySkin.empty() && m_bMaincityResource==true){
        return true;
    }
    return false;
}