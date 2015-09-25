//
//  RewardController.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-20.
//
//

#include "RewardController.h"
#include "SceneController.h"
#include "QuestController.h"
#include "ToolController.h"
#include "EquipmentController.h"

static RewardController *_instance = NULL;

RewardController* RewardController::getInstance() {
    if (!_instance) {
        _instance = new RewardController();
    }
    return _instance;
}

std::string RewardController::retReward(CCArray* arr, bool flyAni)
{
    if(arr == NULL || arr->count() == 0){
        return "";
    }
    CCArray *aniArr = NULL;
    if(flyAni){
        aniArr = CCArray::create();
    }

    CCArray *tmpArr = CCArray::create();
    int i = 0;
    while (i < arr->count()) {
        auto dic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
        int type = dic->valueForKey("type")->intValue();
        if(type != R_GOODS && type != R_GENERAL && type != R_EQUIP){
            if(type == R_EXP){
                tmpArr->insertObject(dic, 0);
            }else if(type == R_POWER){
                if(tmpArr->count() == 0){
                    tmpArr->insertObject(dic, 0);
                }else{
                    auto dic1 = dynamic_cast<CCDictionary*>(tmpArr->objectAtIndex(0));
                    int type1 = dic1->valueForKey("type")->intValue();
                    if(type1 == R_EXP){
                        tmpArr->insertObject(dic, 1);
                    }else{
                        tmpArr->insertObject(dic, 0);
                    }
                }
            }else{
                tmpArr->addObject(dic);
            }
        }else{
            tmpArr->addObject(dic);
        }
        i++;
    }
    arr->removeAllObjects();

    CCObject *obj;
    CCARRAY_FOREACH(tmpArr, obj){
        auto dict = dynamic_cast<CCDictionary*>(obj);
        int type = dict->valueForKey("type")->intValue();
        switch (type) {
            case R_EXP:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.exp = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_EXP));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_POWER:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.questpower = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_POWER));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_WOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lWood = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_WOOD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_STONE:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lStone = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_STONE));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_IRON:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->resourceInfo.lIron = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_IRON));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_FOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    GlobalData::shared()->resourceInfo.lFood = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_FOOD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_SILVER:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    GlobalData::shared()->resourceInfo.lMoney = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_SILVER));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_GOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.gold = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_GOLD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_HONOR:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.allianceInfo.accPoint = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_HONOR));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_ALLIANCE_POINT:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
                    GlobalData::shared()->playerInfo.allianceInfo.alliancepoint = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_ALLIANCE_POINT));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_CHIP:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    GlobalData::shared()->resourceInfo.lChip = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_CHIP));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_DIAMOND:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    GlobalData::shared()->resourceInfo.lDiamond = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_DIAMOND));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_DRAGONFOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    GlobalData::shared()->resourceInfo.lDragonFood = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_DRAGONFOOD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_DRAGONGOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    GlobalData::shared()->resourceInfo.lDragonGold = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_DRAGONGOLD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_GENERAL:{
                break;
            }
            case R_GOODS:{
                CCDictionary *goodsDict = _dict(dict->objectForKey("value"));
                int itemId = goodsDict->valueForKey("itemId")->intValue();
                int num = goodsDict->valueForKey("count")->intValue();
                string uuid = goodsDict->valueForKey("uuid")->getCString();
                auto &info = ToolController::getInstance()->getToolInfoById(itemId);
                int currentNum = info.getCNT();
                ToolController::getInstance()->addTool(itemId, num, uuid);
                aniArr->addObject(CCInteger::create(R_GOODS));
                aniArr->addObject(CCInteger::create(itemId));
                aniArr->addObject(CCInteger::create(num - currentNum));
                break;
            }
            case R_EQUIP:{
                CCDictionary *equipDict = _dict(dict->objectForKey("value"));
                int itemId = equipDict->valueForKey("itemId")->intValue();
                int num = equipDict->valueForKey("count")->intValue();
                string uuid = equipDict->valueForKey("uuid")->getCString();
                
                auto equipArr = CCArray::create();
                equipArr->addObject(equipDict);
                EquipmentController::getInstance()->addEquip(equipArr);
                
                aniArr->addObject(CCInteger::create(R_EQUIP));
                aniArr->addObject(CCInteger::create(itemId));
                aniArr->addObject(CCInteger::create(1));
                break;
            }
            case R_WIN_POINT:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    GlobalData::shared()->playerInfo.winPoint = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_WIN_POINT));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            default:
                break;
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    if(aniArr){
        return flyReward(aniArr);
    }
    return "";
}

std::string RewardController::flyReward(CCArray* arr){
    int i = 0;
    std::string ret = "";
    while(i < arr->count()){
        int type = dynamic_cast<CCInteger*>(arr->objectAtIndex(i))->getValue();
        i++;
        int value = dynamic_cast<CCInteger*>(arr->objectAtIndex(i))->getValue();
        i++;
        int num = dynamic_cast<CCInteger*>(arr->objectAtIndex(i))->getValue();
        std::string valueStr = getNameByType(type, value);
        if(type == R_GENERAL){
            
        }else{
            if(type == R_GOODS){
                valueStr.append(" + ").append(CC_CMDITOA(num));
            }else{
                valueStr.append(" + ").append(CC_CMDITOA(value));
            }
        }
        ret = ret + valueStr + "  ";
        i++;
    }
    return ret;
}

void RewardController::aniComplete(CCNode* node){
    if(node->getParent()){
        node->removeFromParent();
    }
}

std::string RewardController::getPicByType(int type, int value){
    string picName = "";
    if (type == R_GOLD) {
        picName = "ui_gold.png";
    }else if (type == R_SILVER) {
        picName = CCCommonUtils::getResourceIconByType(Silver);
    }else if (type == R_WOOD) {
        picName = CCCommonUtils::getResourceIconByType(Wood);
    }else if (type == R_IRON) {
        picName = CCCommonUtils::getResourceIconByType(Iron);
    }else if (type == R_STONE) {
        picName = CCCommonUtils::getResourceIconByType(Stone);
    }else if (type == R_FOOD) {
        picName = CCCommonUtils::getResourceIconByType(Food);
    }else if (type == R_GENERAL) {
        picName = CCCommonUtils::getGeneralPicById(CC_ITOA(value));
    }else if (type == R_POWER) {
        picName = "icon_combat.png";
    }else if (type == R_EXP) {
        if(value==-1){
            picName = "item505.png";
        }else{
            picName = "icon_exp.png";
        }
        
    }else if (type == R_GOODS) {
        picName = CCCommonUtils::getIcon(CC_ITOA(value));
    }else if (type == R_HONOR) {
        picName = "Contribution_icon.png";
    }else if (type == R_ALLIANCE_POINT) {
        picName = "Contribution_icon1.png";
    }else if (type == R_CHIP){
        picName = CCCommonUtils::getResourceIconByType(Chip);
    }else if (type == R_DIAMOND){
        picName = CCCommonUtils::getResourceIconByType(Diamond);
    }else if (type == R_DRAGONFOOD){
        picName = CCCommonUtils::getResourceIconByType(DragonFood);
    }else if (type == R_DRAGONGOLD){
        picName = CCCommonUtils::getResourceIconByType(DragonGold);
    }else if (type == R_WIN_POINT){
        picName = "icon_liansheng.png";
    }else if (type == R_CRYSTAL){
        picName = "shuijing.png";
    }
    return picName;
}

CCArray* RewardController::getBossReward(std::string itemId){
    CCArray *arr = CCArray::create();
    auto generateItem = [](int type,std::string value,std::string id, CCArray *arr) {
        CCDictionary *dict = CCDictionary::create();
        dict->setObject(CCString::create(CC_ITOA(type)), "t");
        dict->setObject(CCString::create(value), "v");
        dict->setObject(CCString::create(id), "id");
        arr->addObject(dict);
    };
    vector<std::string> vector1;
    vector<std::string> vector;
    
    CCCommonUtils::splitString(itemId, "|", vector);
    int i = 0;
    while(i < vector.size()){
        std::string rewardStr = vector[i];
        vector1.clear();
        CCCommonUtils::splitString(rewardStr, ",", vector1);
        if(vector1[0]=="exp"){
            generateItem(R_EXP, vector1[2],vector1[1], arr);
        }else if(vector1[0]=="power"){
            generateItem(R_POWER, vector1[2],vector1[1], arr);
        }else if(vector1[0]=="silver"){
            generateItem(R_SILVER, vector1[2],vector1[1], arr);
        }else if(vector1[0]=="stone"){
            generateItem(R_STONE, vector1[2],vector1[1], arr);
        }else if(vector1[0]=="wood"){
            generateItem(R_WOOD, vector1[2],vector1[1], arr);
        }else if(vector1[0]=="iron"){
            generateItem(R_IRON,vector1[2],vector1[1], arr);
        }else if(vector1[0]=="food"){
            generateItem(R_FOOD,vector1[2],vector1[1], arr);
        }else if(vector1[0]=="goods"){
            generateItem(R_GOODS,vector1[2],vector1[1], arr);
        }
        i++;
    }
    return arr;
}

std::string RewardController::getNameByType(int type, int value){
    string nameStr = "";
    if (type == R_GOLD) {
        nameStr = _lang("107518");
    }else if (type == R_SILVER) {
        nameStr = CCCommonUtils::getResourceNameByType(Silver);
    }else if (type == R_WOOD) {
        nameStr = CCCommonUtils::getResourceNameByType(Wood);
    }else if (type == R_IRON) {
        nameStr = CCCommonUtils::getResourceNameByType(Iron);
    }else if (type == R_STONE) {
        nameStr = CCCommonUtils::getResourceNameByType(Stone);
    }else if (type == R_FOOD) {
        nameStr = CCCommonUtils::getResourceNameByType(Food);
    }else if (type == R_GENERAL) {
        nameStr = "";
    }else if (type == R_POWER) {
        nameStr = _lang("107510");
    }else if (type == R_EXP) {
        nameStr = _lang("107509");
    }else if (type == R_GOODS){
        nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
    }else if (type == R_GENERAL){
        nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
    }else if (type == R_HONOR){
        nameStr = _lang("115810");
    }else if (type == R_ALLIANCE_POINT){
        nameStr = _lang("115815");
    }else if (type == R_CHIP){
        nameStr = CCCommonUtils::getResourceNameByType(Chip);
    }else if (type == R_DIAMOND){
        nameStr = CCCommonUtils::getResourceNameByType(Diamond);
    }else if (type == R_EQUIP){
        nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
    }else if (type == R_DRAGONFOOD){
        nameStr = CCCommonUtils::getResourceNameByType(DragonFood);
    }else if (type == R_DRAGONGOLD){
        nameStr = CCCommonUtils::getResourceNameByType(DragonGold);
    }else if (type == R_WIN_POINT){
        nameStr = _lang("138155");
    }else if (type == R_CRYSTAL){
        nameStr = _lang("111654");
    }
    
    return nameStr;
}

int RewardController::getTypeByName(std::string name){
    if("exp" == name){
        return R_EXP;
    }
    if("power" == name){
        return R_POWER;
    }
    if("silver" == name){
        return R_SILVER;
    }
    if("stone" == name){
        return R_STONE;
    }
    if("wood" == name){
        return R_WOOD;
    }
    if("iron" == name){
        return R_IRON;
    }
    if("food" == name){
        return R_FOOD;
    }
    if("goods" == name){
        return R_GOODS;
    }
    if("general" == name){
        return R_GENERAL;
    }
    if("gold" == name){
        return R_GOLD;
    }
    if("honor" == name){
        return R_HONOR;
    }
    if("alliance_point" == name){
        return R_ALLIANCE_POINT;
    }
    if("chip" == name){
        return R_CHIP;
    }
    if("diamond" == name){
        return R_DIAMOND;
    }
    if("equip" == name){
        return R_EQUIP;
    }
    if("dragonFood" == name){
        return R_DRAGONFOOD;
    }
    if("dragonGold" == name){
        return R_DRAGONGOLD;
    }
    if("crystal" == name){
        return R_CRYSTAL;
    }
    return R_SILVER;
}

CCArray *RewardController::getRewardArr(CCDictionary *dict){
    CCArray *arr = CCArray::create();
    auto generateItem = [](std::string typeStr, int typeValue, CCDictionary *dict, CCArray *arr) {
        if(dict->objectForKey(typeStr)){
            std::string value = dict->valueForKey(typeStr)->getCString();
            int notCollect = 0;
            std::string notCollectTypeStr = typeStr + "_not_collected";
            if(dict->objectForKey(notCollectTypeStr)){
                notCollect = dict->valueForKey(notCollectTypeStr)->intValue();
            }
            CCDictionary *tempDic = CCDictionary::create();
            tempDic->setObject(CCString::create(CC_ITOA(typeValue)), "type");
            tempDic->setObject(CCString::create(value), "value");
            tempDic->setObject(CCString::create(CC_ITOA(notCollect)), "notCollect");
            arr->addObject(tempDic);
        }
    };
    generateItem("exp", R_EXP, dict, arr);
    generateItem("power", R_POWER, dict, arr);
    generateItem("wood", R_WOOD, dict, arr);
    generateItem("stone", R_STONE, dict, arr);
    generateItem("iron", R_IRON,dict, arr);
    generateItem("food", R_FOOD,dict, arr);
    generateItem("silver", R_SILVER, dict, arr);
    generateItem("chip", R_CHIP, dict, arr);
    generateItem("diamond", R_DIAMOND, dict, arr);
    generateItem("dragonFood", R_DRAGONFOOD, dict, arr);
    generateItem("dragonGold", R_DRAGONGOLD, dict, arr);
    
    return arr;
}

CCArray *RewardController::retRewardArr(CCArray *arr){
    if(arr == NULL || arr->count() == 0){
        return NULL;
    }
    CCArray *aniArr = CCArray::create();
    
    CCArray *tmpArr = CCArray::create();
    int i = 0;
    while (i < arr->count()) {
        auto dic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
        int type = dic->valueForKey("type")->intValue();
        if(type != R_GOODS && type != R_GENERAL){
            if(type == R_EXP){
                tmpArr->insertObject(dic, 0);
            }else if(type == R_POWER){
                if(tmpArr->count() == 0){
                    tmpArr->insertObject(dic, 0);
                }else{
                    auto dic1 = dynamic_cast<CCDictionary*>(tmpArr->objectAtIndex(0));
                    int type1 = dic1->valueForKey("type")->intValue();
                    if(type1 == R_EXP){
                        tmpArr->insertObject(dic, 1);
                    }else{
                        tmpArr->insertObject(dic, 0);
                    }
                }
            }else{
                tmpArr->addObject(dic);
            }
        }else{
            tmpArr->addObject(dic);
        }
        i++;
    }
//    arr->removeAllObjects();
    
    CCObject *obj;
    CCARRAY_FOREACH(tmpArr, obj){
        auto dict = dynamic_cast<CCDictionary*>(obj);
        int type = dict->valueForKey("type")->intValue();
        switch (type) {
            case R_EXP:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
//                    GlobalData::shared()->playerInfo.exp = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_EXP));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_POWER:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
//                    GlobalData::shared()->playerInfo.questpower = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_POWER));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_WOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
//                    GlobalData::shared()->resourceInfo.lWood = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_WOOD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_STONE:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
//                    GlobalData::shared()->resourceInfo.lStone = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_STONE));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_IRON:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
//                    GlobalData::shared()->resourceInfo.lIron = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_IRON));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_FOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
//                    GlobalData::shared()->resourceInfo.lFood = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_FOOD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_SILVER:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
//                    GlobalData::shared()->resourceInfo.lMoney = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_SILVER));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_GOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                if(dict->objectForKey("total")){
//                    GlobalData::shared()->playerInfo.gold = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_GOLD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_CHIP:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
//                    GlobalData::shared()->resourceInfo.lChip = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_CHIP));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_DIAMOND:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
//                    GlobalData::shared()->resourceInfo.lDiamond = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_DIAMOND));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_DRAGONFOOD:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
//                    GlobalData::shared()->resourceInfo.lDragonFood = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_DRAGONFOOD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_DRAGONGOLD:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
//                    GlobalData::shared()->resourceInfo.lDragonGold = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_DRAGONGOLD));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_GENERAL:{
                break;
            }
            case R_GOODS:{
                CCDictionary *goodsDict = _dict(dict->objectForKey("value"));
                int itemId = goodsDict->valueForKey("itemId")->intValue();
                int num = goodsDict->valueForKey("count")->intValue();
                string uuid = goodsDict->valueForKey("uuid")->getCString();
                auto &info = ToolController::getInstance()->getToolInfoById(itemId);
                int currentNum = info.getCNT();
//                ToolController::getInstance()->addTool(itemId, num, uuid);
                aniArr->addObject(CCInteger::create(R_GOODS));
                aniArr->addObject(CCInteger::create(itemId));
                aniArr->addObject(CCInteger::create(num - currentNum));
                break;
            }
            case R_HONOR:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    //                    GlobalData::shared()->resourceInfo.lDiamond = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_HONOR));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_ALLIANCE_POINT:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    //                    GlobalData::shared()->resourceInfo.lDiamond = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_ALLIANCE_POINT));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            case R_EQUIP:{
                int addValue = dict->valueForKey("value")->intValue();
                if (dict->objectForKey("total")) {
                    //                    GlobalData::shared()->resourceInfo.lDiamond = dict->valueForKey("total")->intValue();
                }
                aniArr->addObject(CCInteger::create(R_EQUIP));
                aniArr->addObject(CCInteger::create(addValue));
                aniArr->addObject(CCInteger::create(0));
                break;
            }
            default:
                break;
        }
    }
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    return aniArr;
}

std::string RewardController::getRewardNameInArrayByIndex(CCArray *arr, int idx ){
    CCArray *array=retRewardArr(arr);
    int i = idx*3;
    int type = dynamic_cast<CCInteger*>(array->objectAtIndex(i))->getValue();
    i++;
    int value = dynamic_cast<CCInteger*>(array->objectAtIndex(i))->getValue();
    std::string valueStr = getNameByType(type, value);
    return valueStr;
}

int RewardController::getRewardCountInArrayByIndex(CCArray *arr, int idx ){
    CCArray *array=retRewardArr(arr);
    int i = idx*3;
    
    int type = dynamic_cast<CCInteger*>(array->objectAtIndex(i))->getValue();
    i++;
    int value = dynamic_cast<CCInteger*>(array->objectAtIndex(i))->getValue();
    i++;
    int num = dynamic_cast<CCInteger*>(array->objectAtIndex(i))->getValue();
    if(type == R_GENERAL){

    }else{
        if(type == R_GOODS){
            return num;
        }else{
            return value;
        }
    }
    return 1;
}