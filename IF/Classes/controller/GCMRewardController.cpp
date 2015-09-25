//
//  GCMRewardController.cpp
//  IF
//
//  Created by fubin on 13-12-25.
//
//

#include "GCMRewardController.h"
#include "GCMRewardCommand.h"
#include "GeneralManager.h"
#include "YesNoDialog.h"
#include "LoadingScene.h"
#include "SceneController.h"
#include "ToolController.h"
#include "EquipmentController.h"

static GCMRewardController *_instance = NULL;

GCMRewardController::GCMRewardController()
{
    isFirstOpen = true;
}

GCMRewardController::~GCMRewardController() {
}

GCMRewardController* GCMRewardController::getInstance() {
    if (!_instance) {
        _instance = new GCMRewardController();
    }
    return _instance;
}

void GCMRewardController::purgeData() {
    delete _instance;
    _instance = NULL;
}

void GCMRewardController::initGCMRewardData(CCDictionary* dict)
{
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("pushActivity"));
    if( arr==NULL ){
        return;
    }
    
    CCDictionary *item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        GCMRewardInfo info = GCMRewardInfo(item);
        m_rewardPool.push_back(info);
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_GCMREWARD_UPDATE);
}

bool GCMRewardController::sendReward(string actId)
{
    std::vector<GCMRewardInfo>::iterator it = m_rewardPool.begin();
    for (; it!=m_rewardPool.end(); it++) {
        m_rewardPool.erase(it);
        break;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_GCMREWARD_UPDATE);
    
    GCMRewardCommand* cmd = new GCMRewardCommand(actId, GCM_REWARD_COMMAND);
    cmd->sendAndRelease();
    return true;
}

CCArray* GCMRewardController::retReward(CCDictionary* dict)
{
    CCArray* ret = CCArray::create();

    if (dict->objectForKey("errorCode"))
    {
        string errorCode = dict->valueForKey("errorCode")->getCString();
        if(errorCode=="E100049")
        {
            //提示更新版本
            YesNoDialog::show(_lang("E100049").c_str(),CCCallFuncO::create(this, callfuncO_selector(GameController::updateVersion), NULL));
        }
        return ret;
    }

    if (dict) {
        bool updateRes = false;
        if (dict->objectForKey("money"))
        {
            int value = dict->valueForKey("money")->intValue() - GlobalData::shared()->resourceInfo.lMoney;
            GlobalData::shared()->resourceInfo.lMoney = dict->valueForKey("money")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("money"));
            ret->addObject(CCInteger::create(value));
        }
        
        if (dict->objectForKey("wood"))
        {
            int value = dict->valueForKey("wood")->intValue() - GlobalData::shared()->resourceInfo.lWood;
            GlobalData::shared()->resourceInfo.lWood = dict->valueForKey("wood")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("wood"));
            ret->addObject(CCInteger::create(value));
        }
        
        if (dict->objectForKey("food"))
        {
            int value = dict->valueForKey("food")->intValue() - GlobalData::shared()->resourceInfo.lFood;
            GlobalData::shared()->resourceInfo.lFood = dict->valueForKey("food")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("food"));
            ret->addObject(CCInteger::create(value));
        }
        
        if (dict->objectForKey("iron"))
        {
            int value = dict->valueForKey("iron")->intValue() - GlobalData::shared()->resourceInfo.lIron;
            GlobalData::shared()->resourceInfo.lIron = dict->valueForKey("iron")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("iron"));
            ret->addObject(CCInteger::create(value));

        }
        if (dict->objectForKey("stone"))
        {
            int value = dict->valueForKey("stone")->intValue() - GlobalData::shared()->resourceInfo.lStone;
            GlobalData::shared()->resourceInfo.lStone = dict->valueForKey("stone")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("stone"));
            ret->addObject(CCInteger::create(value));
            
        }
        
        if (dict->objectForKey("gold"))
        {
            int value = dict->valueForKey("gold")->intValue() - GlobalData::shared()->playerInfo.gold;
            GlobalData::shared()->playerInfo.gold = dict->valueForKey("gold")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("gold"));
            ret->addObject(CCInteger::create(value));
        }
        
        if (dict->objectForKey("crystal"))
        {
            int value = dict->valueForKey("crystal")->intValue() - GlobalData::shared()->playerInfo.longJing;
            GlobalData::shared()->playerInfo.longJing = dict->valueForKey("crystal")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("crystal"));
            ret->addObject(CCInteger::create(value));
        }
        
        if (dict->objectForKey("chip"))
        {
            int value = dict->valueForKey("chip")->intValue() - GlobalData::shared()->resourceInfo.lChip;
            GlobalData::shared()->resourceInfo.lChip = dict->valueForKey("chip")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("chip"));
            ret->addObject(CCInteger::create(value));
        }
        
        if (dict->objectForKey("diamond"))
        {
            int value = dict->valueForKey("diamond")->intValue() - GlobalData::shared()->resourceInfo.lDiamond;
            GlobalData::shared()->resourceInfo.lDiamond = dict->valueForKey("diamond")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("diamond"));
            ret->addObject(CCInteger::create(value));
        }
        
        if (dict->objectForKey("dragonFood"))
        {
            int value = dict->valueForKey("dragonFood")->intValue() - GlobalData::shared()->resourceInfo.lDiamond;
            GlobalData::shared()->resourceInfo.lDragonFood = dict->valueForKey("dragonFood")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("dragonFood"));
            ret->addObject(CCInteger::create(value));
        }
        
        if (dict->objectForKey("dragonGold"))
        {
            int value = dict->valueForKey("dragonGold")->intValue() - GlobalData::shared()->resourceInfo.lDiamond;
            GlobalData::shared()->resourceInfo.lDragonGold = dict->valueForKey("dragonGold")->intValue();
            updateRes = true;
            ret->addObject(CCString::create("dragonGold"));
            ret->addObject(CCInteger::create(value));
        }
        
        if(updateRes)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        }

        if (dict->objectForKey("goods"))
        {
            auto arr = dynamic_cast<CCArray*>(dict->objectForKey("goods"));
            CCDictionary *item = NULL;
            for (int i=0; i<arr->count(); i++) {
                item = _dict(arr->objectAtIndex(i));
                string itemId = item->valueForKey("itemId")->getCString();
                int num = item->valueForKey("count")->intValue();
                string uuid = item->valueForKey("uuid")->getCString();
                ToolController::getInstance()->addTool(atoi(itemId.c_str()), num, uuid);
            }
        }
        if (dict->objectForKey("general"))
        {
            auto arr = dynamic_cast<CCArray*>(dict->objectForKey("general"));
            CCDictionary *item = NULL;
            for (int i=0; i<arr->count(); i++) {
                item = _dict(arr->objectAtIndex(i));
                GeneralManager::getInstance()->addGeneral(item);
                ret->addObject(CCString::create("general"));
                ret->addObject(CCInteger::create(item->valueForKey("generalId")->intValue()));
            }
        }
        if (dict->objectForKey("equip"))
        {
            auto arr = dynamic_cast<CCArray*>(dict->objectForKey("equip"));
            EquipmentController::getInstance()->addEquip(arr);
            CCDictionary *item = NULL;
            for (int i=0; i<arr->count(); i++) {
                item = _dict(arr->objectAtIndex(i));
                ret->addObject(CCString::create("equip"));
                ret->addObject(CCInteger::create(item->valueForKey("itemId")->intValue()));
            }
        }
    }
    return ret;
}

CCArray* GCMRewardController::retReward2(CCArray* arr, bool flyAni/*=false*/)
{
    if(arr == NULL || arr->count() == 0){
        return CCArray::create();
    }
    CCArray *ret = NULL;
    CCArray *retGoods = NULL;
    ret = CCArray::create();
    retGoods = CCArray::create();
    bool flyGoods = false;
    bool flyRes = false;
    
    CCObject *obj;
    CCARRAY_FOREACH(arr, obj){
        auto dict = dynamic_cast<CCDictionary*>(obj);
        int type = dict->valueForKey("type")->intValue();
        switch (type) {
            case R_WOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lWood = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("wood"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_STONE:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lStone = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("stone"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_IRON:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lIron = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("iron"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_FOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lFood = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("food"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_SILVER:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lMoney = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("money"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_GOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.gold = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("gold"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_CHIP:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lChip = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("chip"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_DIAMOND:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lDiamond = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("diamond"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_DRAGONFOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lDragonFood = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("dragonFood"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_DRAGONGOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lDragonGold = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("dragonGold"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_GENERAL:{
                auto arr = dynamic_cast<CCArray*>(dict->objectForKey("general"));
                CCDictionary *item = NULL;
                for (int i=0; i<arr->count(); i++) {
                    item = _dict(arr->objectAtIndex(i));
                    GeneralManager::getInstance()->addGeneral(item);
                    ret->addObject(CCString::create("general"));
                    ret->addObject(CCInteger::create(item->valueForKey("generalId")->intValue()));
                    flyRes = true;
                }
                break;
            }
            case R_GOODS:{
                CCDictionary *goodsDict = _dict(dict->objectForKey("value"));
                int itemId = goodsDict->valueForKey("itemId")->intValue();
                int num = goodsDict->valueForKey("count")->intValue();
                int rewardAdd = 0;
                if(goodsDict->objectForKey("rewardAdd")){
                    rewardAdd = goodsDict->valueForKey("rewardAdd")->intValue();
                }else{
                    auto &info = ToolController::getInstance()->getToolInfoById(itemId);
                    int currentNum = info.getCNT();
                    rewardAdd = num - currentNum;
                }
                string uuid = goodsDict->valueForKey("uuid")->getCString();
                ToolController::getInstance()->addTool(itemId, num, uuid);
                goodsDict->setObject(CCString::createWithFormat("%d",rewardAdd), "count");
                retGoods->addObject(goodsDict);
                flyGoods = true;
                
                break;
            }
            case R_EQUIP:{
                CCDictionary *equipDict = _dict(dict->objectForKey("value"));
                auto equipArr = CCArray::create();
                equipArr->addObject(equipDict);
                EquipmentController::getInstance()->addEquip(equipArr);
                
                int itemId = equipDict->valueForKey("itemId")->intValue();
                
                break;
            }
            case R_EXP:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.exp = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("exp"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            case R_WIN_POINT:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.winPoint = dict->valueForKey("total")->intValue();
                }
                ret->addObject(CCString::create("winPoint"));
                ret->addObject(CCInteger::create(addValue));
                flyRes = true;
                break;
            }
            default:
                break;
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    if(flyAni)
    {
        if (flyGoods)
        {
            flyToolReward(retGoods);
        }
        if (flyRes)
        {
            flyReward(ret);
        }

    }
    return ret;
}

CCArray* GCMRewardController::getFlyArr(cocos2d::CCArray *arr)
{
    if(arr == NULL || arr->count() == 0){
        return CCArray::create();
    }
    CCArray *ret = NULL;
    ret = CCArray::create();
    
    CCObject *obj;
    CCARRAY_FOREACH(arr, obj){
        auto dict = dynamic_cast<CCDictionary*>(obj);
        int type = dict->valueForKey("type")->intValue();
        switch (type) {
            case R_WOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("wood"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_STONE:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("stone"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_IRON:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("iron"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_FOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("food"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_SILVER:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("money"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_GOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("gold"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_CHIP:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("chip"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_DIAMOND:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("diamond"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_DRAGONFOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("dragonFood"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_DRAGONGOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("dragonGold"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            case R_GENERAL:{
                auto arr = dynamic_cast<CCArray*>(dict->objectForKey("general"));
                CCDictionary *item = NULL;
                for (int i=0; i<arr->count(); i++) {
                    item = _dict(arr->objectAtIndex(i));
                    ret->addObject(CCString::create("general"));
                    ret->addObject(CCInteger::create(item->valueForKey("generalId")->intValue()));
                }
                break;
            }
            case R_GOODS:{
                CCDictionary *goodsDict = _dict(dict->objectForKey("value"));
//                int itemId = goodsDict->valueForKey("itemId")->intValue();
                int num = goodsDict->valueForKey("count")->intValue();
                int rewardAdd = goodsDict->valueForKey("rewardAdd")->intValue();
//                string uuid = goodsDict->valueForKey("uuid")->getCString();
                goodsDict->setObject(CCString::createWithFormat("%d",num), "count");
                ret->addObject(goodsDict);
                
                break;
            }
            case R_WIN_POINT:{
                int addValue = dict->valueForKey("value")->intValue();
                ret->addObject(CCString::create("winPoint"));
                ret->addObject(CCInteger::create(addValue));
                break;
            }
            default:
                break;
        }
    }
    return ret;
}

void GCMRewardController::flyReward(CCArray* arr){
    int i = 0;
    while(i < arr->count()){
        float w = 80;
        std::string type = dynamic_cast<CCString*>(arr->objectAtIndex(i))->getCString();
        i++;
        int value = dynamic_cast<CCInteger*>(arr->objectAtIndex(i))->getValue();
        auto node = CCNode::create();
        auto sprite = CCLoadSprite::createSprite(getPicByType(type, value).c_str());
        sprite->setScaleX(w / sprite->getContentSize().width);
        sprite->setScaleY(w / sprite->getContentSize().height);

        node->addChild(sprite);
        auto label = CCLabelIF::create();
        label->setColor(ccGREEN);
        
        std::string valueStr = "";
        if(type == "general"){
            
        }else{
            if (type == "equip") {
                valueStr.append(" + ").append(CC_ITOA(1));
            }else {
                valueStr.append(" + ").append(CC_ITOA(value));
            }
        }
        label->setString(valueStr.c_str());

        label->setAnchorPoint(ccp(0, 0.5));
        label->setPositionX(w / 2 + 20);
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setFontSize(40);
            label->setScale(0.5);
        }
        node->addChild(label);

        auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
        node->setPosition(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2 - 60 * i / 2);
        if (CCCommonUtils::isIosAndroidPad()) {
            node->setPosition(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2 - 60 * 2.0 * i / 2);
        }
        topLayer->addChild(node);
        int endY = node->getPositionY() + 150;
        int endX = node->getPositionX();
        if (CCCommonUtils::isIosAndroidPad())
        {
            endY = node->getPositionY() + 150 * 2.0;
            node->setScale(2.0);
        }
        node->runAction(CCSequence::create(
                                            CCMoveTo::create(1, ccp(endX, endY))
                                            , CCCallFuncO::create(this, callfuncO_selector(GCMRewardController::aniComplete),node)
                                            ,NULL
                                            ));
        i++;
    }
}

void GCMRewardController::aniComplete(CCObject* p){
    auto node = dynamic_cast<CCNode*>(p);
    if(node->getParent()){
        node->removeFromParent();
    }
}

void GCMRewardController::flyToolReward(CCArray* arr)
{
    CCObject *obj;
    auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
    int i = 0;
    int cnt = arr->count();
    int centerX = CCDirector::sharedDirector()->getWinSize().width / 2;
    int centerY = CCDirector::sharedDirector()->getWinSize().height / 2;
    int lineH = 90;
    int cellW = 90;
    if (CCCommonUtils::isIosAndroidPad())
    {
        lineH = lineH * 2.0;
        cellW = cellW * 2.0;
    }
    CCARRAY_FOREACH(arr, obj) {
        std::string itemId = _dict(obj)->valueForKey("itemId")->getCString();
        auto &item = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
        int num = _dict(obj)->valueForKey("count")->intValue() - item.getCNT();
        
        float w = 80;
        auto node = CCNode::create();
        auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getIcon(itemId.c_str()).c_str(),true,CCLoadSpriteType_GOODS);
        sprite->setAnchorPoint(ccp(0.5, 0));
        CCCommonUtils::setSpriteMaxSize(sprite, 80);
        
        node->addChild(sprite);
        auto label = CCLabelIF::create();
        label->setColor(ccGREEN);
        
        std::string valueStr = "";
        valueStr.append("+ ").append(CC_ITOA(num));
        label->setString(valueStr.c_str());
        label->setAnchorPoint(ccp(0.5, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setScale(0.5);
            label->setFontSize(40);
        }
//        label->setPositionX(w / 2 + 20);
        node->addChild(label);
        
        int line = cnt/4-i/4;
        int cellX = 0;
        if (line>0) {
            cellX = (i%4 - 2) * cellW + cellW/2 + centerX;
        }
        else {
            if(cnt%4 == 1) {
                cellX = centerX;
            }else if(cnt%4 == 2) {
                cellX = (i%2 - 1) * cellW + cellW/2 + centerX;
            }else if(cnt%4 == 3) {
                cellX = (i%3 - 2) * cellW + centerX;
            }else {//==0
                cellX = (i%4 - 2) * cellW + cellW/2 + centerX;
            }
        }
        int endY = -80;//node->getPositionY() + 150;
        int endX = centerX;//node->getPositionX();
        
        CCActionInterval * scaleto = nullptr;
        if (CCCommonUtils::isIosAndroidPad())
        {
            scaleto = CCScaleTo ::create(0.5, 2.0);
        }
        else
            scaleto = CCScaleTo ::create(0.5, 1);
        CCActionInterval * easeBackOut1 = CCEaseBackOut ::create(scaleto);
        CCActionInterval * moveto = CCMoveTo ::create(0.5, ccp(cellX, line*lineH+centerY));
        CCActionInterval * easeBackOut2 = CCEaseBackOut ::create(moveto);
        CCFiniteTimeAction * spawn1 =CCSpawn::create(easeBackOut1,easeBackOut2,NULL);
        
        CCActionInterval * delayT = CCDelayTime ::create(1.0);
        
        CCActionInterval * scaleto2 = CCScaleTo ::create(0.5, 0);
        CCActionInterval * CCEaseBackIn1 = CCEaseBackIn ::create(scaleto2);
        CCActionInterval * moveto2 = CCMoveTo ::create(0.5, ccp(endX, endY));
        CCActionInterval * CCEaseBackIn2 = CCEaseBackIn ::create(moveto2);
        CCFiniteTimeAction * spawn2 =CCSpawn::create(CCEaseBackIn1,CCEaseBackIn2,NULL);
        
        node->setScale(0);
        node->setPosition(centerX, centerY);
        topLayer->addChild(node);
        node->runAction(CCSequence::create(spawn1,delayT,spawn2,CCCallFuncO::create(this, callfuncO_selector(GCMRewardController::aniComplete),node),NULL));
        i++;
    }
}

std::string GCMRewardController::getPicByType(std::string type, int value){
    string picName = "";
    if (type == "gold") {
        picName = "ui_gold.png";
    }else if (type == "crystal") {
        picName = "shuijing.png";
    }else if (type == "money") {
        picName = CCCommonUtils::getResourceIconByType(Silver);
    }else if (type == "wood") {
        picName = CCCommonUtils::getResourceIconByType(Wood);
    }else if (type == "iron") {
        picName = CCCommonUtils::getResourceIconByType(Iron);
    }else if (type == "general") {
        picName = CCCommonUtils::getGeneralPicById(CC_ITOA(value));
    }else if(type == "food"){
        picName = CCCommonUtils::getResourceIconByType(Food);
    }else if(type == "stone"){
        picName = CCCommonUtils::getResourceIconByType(Stone);
    }else if(type == "chip"){
        picName = CCCommonUtils::getResourceIconByType(Chip);
    }else if(type == "diamond"){
        picName = CCCommonUtils::getResourceIconByType(Diamond);
    }else if(type == "equip"){
        picName = CCCommonUtils::getIcon(CC_ITOA(value));
    }else if (type == "exp") {
        if(value==-1){
            picName = "item505.png";
        }else{
            picName = "icon_exp.png";
        }
        
    }else if (type == "dragonFood"){
        picName = CCCommonUtils::getResourceIconByType(DragonFood);
    }else if (type == "dragonGold"){
        picName = CCCommonUtils::getResourceIconByType(DragonGold);
    }else if (type == "winPoint"){
        picName = "icon_liansheng.png";
    }
    return picName;
}
