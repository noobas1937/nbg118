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
#include "SceneController.h"
#include "ToolController.h"
#include "EquipmentController.h"

#include "RewardController.h"
#include "FlyCell.h"
#include "UIComponent.h"

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

//fusheng begin
void GCMRewardController::retReward3(Array *arr, float time, bool move, Point fromPt)
{
    if(arr == NULL || arr->count() == 0){
        return ;
    }
    
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
                break;
            }
            case R_STONE:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lStone = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_IRON:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lIron = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_FOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lFood = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_SILVER:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lMoney = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_GOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.gold = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_CHIP:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lChip = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_DIAMOND:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lDiamond = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_DRAGONFOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lDragonFood = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_DRAGONGOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lDragonGold = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_GENERAL:{
                auto arr = dynamic_cast<CCArray*>(dict->objectForKey("general"));
                CCDictionary *item = NULL;
                for (int i=0; i<arr->count(); i++) {
                    item = _dict(arr->objectAtIndex(i));
                    GeneralManager::getInstance()->addGeneral(item);
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
                    goodsDict->setObject(CCString::create(CC_ITOA(rewardAdd)) , "rewardAdd");
                }
                string uuid = goodsDict->valueForKey("uuid")->getCString();
                ToolController::getInstance()->addTool(itemId, num, uuid);
                goodsDict->setObject(CCString::createWithFormat("%d",rewardAdd), "count");
                break;
            }
            case R_EQUIP:{
                CCDictionary *equipDict = _dict(dict->objectForKey("value"));
                auto equipArr = CCArray::create();
                equipArr->addObject(equipDict);
                EquipmentController::getInstance()->addEquip(equipArr);
                break;
            }
            case R_EXP:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.exp = dict->valueForKey("total")->intValue();
                }
                break;
            }
            case R_WIN_POINT:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.winPoint = dict->valueForKey("total")->intValue();
                }
                break;
            }
            default:
                break;
        }
    }
    flyRewardFromPointToPoint(arr,time,move,fromPt);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
}

void GCMRewardController::flyRewardFromPointToPoint(Array *arr, float time, bool move, Point fromPt)
{
    int i = 0;
    int countPerRow = arr->count();//每排数量
    if (countPerRow>4) {
        countPerRow = 4;
    }
    float gap = CCDirector::sharedDirector()->getWinSize().width;
    if (countPerRow > 0) {
        gap = CCDirector::sharedDirector()->getWinSize().width / countPerRow;//每个的宽度
    }
    while(i < countPerRow){
        CCDictionary* dic = _dict(arr->objectAtIndex(i));
        if (dic==NULL) {
            continue;
        }
        float w = 100;
        int outPut = 0;
        int type = dic->valueForKey("type")->intValue();
        auto node = CCNode::create();
        auto node1 = CCNode::create();
        auto node2 = CCNode::create();
        if (type == R_GOODS) {
            CCDictionary *goodsDict = _dict(dic->objectForKey("value"));
            int itemId = goodsDict->valueForKey("itemId")->intValue();
            int num = goodsDict->valueForKey("count")->intValue();
            int rewardAdd = 0;
            if(goodsDict->objectForKey("rewardAdd")){
                rewardAdd = goodsDict->valueForKey("rewardAdd")->intValue();
            }else{
                auto &info = ToolController::getInstance()->getToolInfoById(itemId);
                int currentNum = info.getCNT();
                rewardAdd = num - currentNum;
                goodsDict->setObject(CCString::create(CC_ITOA(rewardAdd)) , "rewardAdd");
            }
            
            int value = rewardAdd;
            auto bg = CCLoadSprite::createSprite("icon_BG.png");
            //            bg->setScale(1.2);
            auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(itemId)).c_str(),true,CCLoadSpriteType_GOODS);
            sprite->setAnchorPoint(ccp(0.5, 0));
            CCCommonUtils::setSpriteMaxSize(sprite, w);
            node1->addChild(bg);
            node2->addChild(sprite);
            
            std::string valueStr = "";
            valueStr.append("+ ").append(CC_ITOA_K((long)value));
            CCLabelBMFont* numIF = CCLabelBMFont::create(valueStr, "pve_fnt_boss.fnt");
            numIF->setAnchorPoint(ccp(0.5, 1));
            numIF->setPositionY(-5);
            numIF->setScale(0.6);
            node2->addChild(numIF);
            node->addChild(node1);
            node->addChild(node2);
        }
        else {
            int value = dic->valueForKey("value")->intValue();
            int temp = value;
            if (type != R_GENERAL) {
                temp = -1;
            }
            
            auto bg = CCLoadSprite::createSprite("icon_BG.png");
            //            bg->setScale(1.2);
            auto sprite = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, temp).c_str());
            sprite->setScaleX(w / sprite->getContentSize().width);
            sprite->setScaleY(w / sprite->getContentSize().height);
            outPut = value;
            node1->addChild(bg);
            node2->addChild(sprite);
            
            std::string valueStr = "";
            valueStr.append(" + ").append(CC_ITOA_K((long)value));
            CCLabelBMFont* numIF = CCLabelBMFont::create(valueStr, "pve_fnt_boss.fnt");
            numIF->setAnchorPoint(ccp(0.5, 1));
            numIF->setPositionY(-5);
            numIF->setScale(0.6);
            node2->addChild(numIF);
            node->addChild(node1);
            node->addChild(node2);
        }
        
        node->setPosition(0, 0);
        float startY = 0;//startX/Y为出现在屏幕上的初始位置
        float startX = CCDirector::sharedDirector()->getWinSize().width / 2;
        float endY = 0;//endX/Y为动画最后结束的位置
        float endX = 0;
        gap = 140;//每个奖励的间隔
        if (CCCommonUtils::isIosAndroidPad()) {
            gap = w * 2.4;
        }
        switch (countPerRow) {//奖励最多显示4个
            case 2:
                startX += (-0.5 + i) * gap;
                break;
            case 3:
                startX += (-1 + i) * gap;
                break;
            case 4:
                startX += (-1.5 + i) * gap;
                break;
            default:
                break;
        }
        auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
        node1->setPosition(startX, 350);
        node2->setPosition(startX, 350);
        if (CCCommonUtils::isIosAndroidPad()) {
            node1->setPosition(startX, 700);
            node2->setPosition(startX, 700);
        }
        startY = node2->getPositionY();
        topLayer->addChild(node);
        if (type == R_EXP) {//经验飞到头像
            endX = 45;
            endY = CCDirector::sharedDirector()->getWinSize().height - 45;
            if (CCCommonUtils::isAdriodPad()) {
                endX = 90;
                endY = CCDirector::sharedDirector()->getWinSize().height - 90;
            }
        }
        else if (type == R_GOLD) {//金币飞到右上角
            endX = CCDirector::sharedDirector()->getWinSize().width - 45;
            endY = CCDirector::sharedDirector()->getWinSize().height - 45;
            if (CCCommonUtils::isAdriodPad()) {
                endX = CCDirector::sharedDirector()->getWinSize().width - 90;
                endY = CCDirector::sharedDirector()->getWinSize().height - 90;
            }
        }
        else {
            endX = 325;
            endY = 35;
            if (CCCommonUtils::isIosAndroidPad()) {
                endX = 800;
                endY = 80;
            }
        }
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            node1->setScale(2.0);
            node2->setScale(2.0);
        }
        
        node1->setScale(0);
        node2->setScale(0);
        if (move) {
            node1->setPosition(fromPt);//当move为true时，需要从fromPt飞到startPt
            node2->setPosition(fromPt);
        }
        CCActionInterval * scaleto = CCScaleTo ::create(0.3, 1);//从0变大到1
        if (CCCommonUtils::isIosAndroidPad())
        {
            scaleto = CCScaleTo::create(0.3, 2.0);
        }
        CCActionInterval * moveto = CCMoveTo ::create(0.3, ccp(startX, startY));
        CCActionInterval * delay = CCDelayTime::create(time);
        CCActionInterval * scaleto2 = CCScaleTo::create(0.5, 0);
        CCActionInterval * scaleto3 = CCScaleTo::create(0.0, 0);
        CCActionInterval * moveto2 = CCMoveTo ::create(0.5, ccp(endX, endY));
        CCFiniteTimeAction * spawn =CCSpawn::create(scaleto2,moveto2,NULL);
        CCFiniteTimeAction * spawn1 = NULL;
        CCActionInterval * fadeIn = CCScaleTo::create(0.0, 1);
        if (CCCommonUtils::isIosAndroidPad()) {
            fadeIn = CCScaleTo::create(0.0, 2);
        }
        CCActionInterval * fadeOut = CCScaleTo::create(0.0, 0);
        if (move) {
            spawn1 = Spawn::create(moveto,scaleto, NULL);
        }else {
            spawn1 = Spawn::create(scaleto, NULL);
        }
        if (type == Food || type == Wood || type == Iron || type == Stone) {
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(String::create(CC_ITOA(startX)), "x");
            dic->setObject(String::create(CC_ITOA(startY)), "y");
            dic->setObject(String::create(CC_ITOA(type)), "type");
            if (move) {
                node1->runAction(CCSequence::create(spawn1->clone(), DelayTime::create(time), fadeOut, NULL));
                node2->runAction(CCSequence::create(spawn1,delay,scaleto3,CCCallFuncO::create(this, callfuncO_selector(GCMRewardController::aniComplete),node),CCCallFuncO::create(this, callfuncO_selector(GCMRewardController::flyResAni),dic),NULL));
            }
            else {
                node1->runAction(CCSequence::create(fadeIn, DelayTime::create(0.2+time), fadeOut, NULL));
                node2->runAction(CCSequence::create(spawn1,delay,scaleto3,CCCallFuncO::create(this, callfuncO_selector(GCMRewardController::aniComplete),node),CCCallFuncO::create(this, callfuncO_selector(GCMRewardController::flyResAni),dic),NULL));
            }
        }
        else {
            if (move) {
                node1->runAction(CCSequence::create(spawn1->clone(), DelayTime::create(time), fadeOut, NULL));
                node2->runAction(CCSequence::create(spawn1,delay,spawn,CCCallFuncO::create(this, callfuncO_selector(GCMRewardController::aniComplete),node),NULL));
            }
            else {
                node1->runAction(CCSequence::create(fadeIn, DelayTime::create(0.2+time), fadeOut, NULL));
                node2->runAction(CCSequence::create(spawn1,delay,spawn,CCCallFuncO::create(this, callfuncO_selector(GCMRewardController::aniComplete),node),NULL));
            }
        }
        i++;
    }
}

//fusheng end








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

void GCMRewardController::flyResAni(CCObject *obj)
{
    auto dic = _dict(obj);
    if(dic){
        string picStr = "Food";
        int resType = dic->valueForKey("type")->intValue();
        float ptX = dic->valueForKey("x")->floatValue();
        float ptY = dic->valueForKey("y")->floatValue();
        Point endPt = ccp(0, 0);
//        if (resType == Food) {
//            picStr = "Food";
//            endPt = ccp(200, -60);
//            if (CCCommonUtils::isIosAndroidPad()) {
//                endPt = ccpMult(endPt, 2.4);
//            }
//        }
//        else if (resType == Wood) {
//            picStr = "Wood";
//            endPt = ccp(90, -40);
//            if (CCCommonUtils::isIosAndroidPad()) {
//                endPt = ccpMult(endPt, 2.4);
//            }
//        }
//        else if (resType == Iron) {
//            picStr = "Iron";
//            endPt = ccp(320, -40);
//            if (CCCommonUtils::isIosAndroidPad()) {
//                endPt = ccpMult(endPt, 2.4);
//            }
//        }
//        else if (resType == Stone) {
//            picStr = "Stone";
//            resType = Stone;
//            endPt = ccp(430, -40);
//            if (CCCommonUtils::isIosAndroidPad()) {
//                endPt = ccpMult(endPt, 2.4);
//            }
//        }
//        
        
        if (resType == Food) {

            endPt = UIComponent::getInstance()->m_foodIcon->convertToWorldSpace(Point::ZERO);
            picStr = "Food";
            resType = Food;
        }else if (resType == Wood) {
       
            endPt = UIComponent::getInstance()->m_woodIcon->convertToWorldSpace(Point::ZERO);
            picStr = "Wood";
            resType = Wood;
        }
        else if (resType == Iron) {
  
            endPt = UIComponent::getInstance()->m_ironIcon->convertToWorldSpace(Point::ZERO);
            picStr = "Iron";
            resType = Iron;
        }
        else if (resType == Stone) {

            endPt = UIComponent::getInstance()->m_stoneIcon->convertToWorldSpace(Point::ZERO);
            picStr = "Stone";
            resType = Stone;
        }

        
        
//        ptY = ptY-CCDirector::sharedDirector()->getWinSize().height;
        Point startPt = ccp(ptX, ptY);
        
        startPt = UIComponent::getInstance()->convertToWorldSpace(startPt);
        
        
        srand((unsigned)time(0));
        int tmp = 10;
        int idx = 0;
        while (tmp>0) {
            auto flyCell = FlyCell::create(startPt,endPt,resType, picStr, 5, UIComponent::getInstance()->m_collectBatch, idx*0.2);
            UIComponent::getInstance()->m_collectNode->addChild(flyCell);
            tmp--;
            idx++;
        }
    }
}
