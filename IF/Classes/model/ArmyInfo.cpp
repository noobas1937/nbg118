//
//  ArmyInfo.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-28.
//
//

#include "ArmyInfo.h"
#include "GlobalData.h"
#include "TrainSoldiertimeCommand.h"
#include "FortCommand.h"
#include "QueueController.h"

ArmyInfo::ArmyInfo(cocos2d::CCDictionary *dict)
:power(1.0),train(0),march(0)
{
    range = dict->valueForKey("range")->intValue();
    armyId = dict->valueForKey("id")->getCString();
    healSilver = dict->valueForKey("heal_silver")->intValue();
    healRes = dict->valueForKey("heal_res")->intValue();
    healTime = dict->valueForKey("heal_time")->intValue();
    time = dict->valueForKey("time")->intValue();
    free = dict->valueForKey("free")->intValue();
    upkeep = dict->valueForKey("upkeep")->floatValue();
    defen = dict->valueForKey("defen")->intValue();
    move = dict->valueForKey("move")->intValue();
    attack = dict->valueForKey("attack")->intValue();
    defence = dict->valueForKey("defen")->intValue();
    pve = dict->valueForKey("pve")->intValue();
    wood = dict->valueForKey("wood")->intValue();
    food = dict->valueForKey("food")->intValue();
    silver = dict->valueForKey("silver")->intValue();
    iron = dict->valueForKey("iron")->intValue();
    stone = dict->valueForKey("stone")->intValue();
    if(dict->objectForKey("train")){
        train = dict->valueForKey("train")->intValue();
    }
    level = dict->valueForKey("level")->intValue();
    load = dict->valueForKey("load")->intValue();
    armyLevel = atoi(armyId.c_str()) % 100;
    speed = dict->valueForKey("speed")->intValue();
    march = dict->valueForKey("march")->intValue();
    health = dict->valueForKey("health")->intValue();
    if(dict->objectForKey("power")){
        power = dict->valueForKey("power")->floatValue();
    }else{
        power = 1;
    }
    finishTime = 0;
    isArmy = true;
    setFinishiTime(dict);
}

void ArmyInfo::setArmyInfo(cocos2d::CCDictionary *dict)
{
    if (dict->objectForKey("range")) {
        range = dict->valueForKey("range")->intValue();
    }
    if (dict->objectForKey("id")) {
        armyId = dict->valueForKey("id")->getCString();
    }
    if (dict->objectForKey("heal_silver")) {
        healSilver = dict->valueForKey("heal_silver")->intValue();
    }
    if (dict->objectForKey("heal_res")) {
        healRes = dict->valueForKey("heal_res")->intValue();
    }
    if (dict->objectForKey("heal_time")) {
        healTime = dict->valueForKey("heal_time")->intValue();
    }
    if (dict->objectForKey("time")) {
        time = dict->valueForKey("time")->intValue();
    }
    if (dict->objectForKey("free")) {
        free = dict->valueForKey("free")->intValue();
    }
    if (dict->objectForKey("upkeep")) {
        upkeep = dict->valueForKey("upkeep")->floatValue();
    }
    if (dict->objectForKey("defen")) {
        defen = dict->valueForKey("defen")->intValue();
    }
    if (dict->objectForKey("move")) {
        move = dict->valueForKey("move")->intValue();
    }
    if (dict->objectForKey("attack")) {
        attack = dict->valueForKey("attack")->intValue();
    }
    if (dict->objectForKey("defen")) {
        defence = dict->valueForKey("defen")->intValue();
    }
    if (dict->objectForKey("pve")) {
        pve = dict->valueForKey("pve")->intValue();
    }
    if (dict->objectForKey("wood")) {
        wood = dict->valueForKey("wood")->intValue();
    }
    if (dict->objectForKey("food")) {
        food = dict->valueForKey("food")->intValue();
    }
    if (dict->objectForKey("silver")) {
        silver = dict->valueForKey("silver")->intValue();
    }
    if (dict->objectForKey("iron")) {
        iron = dict->valueForKey("iron")->intValue();
    }
    if (dict->objectForKey("stone")) {
        stone = dict->valueForKey("stone")->intValue();
    }
    if (dict->objectForKey("level")) {
        level = dict->valueForKey("level")->intValue();
    }
    if (dict->objectForKey("load")) {
        load = dict->valueForKey("load")->intValue();
    }
    if (dict->objectForKey("speed")) {
        speed = dict->valueForKey("speed")->intValue();
    }
    if (dict->objectForKey("march")) {
        march = dict->valueForKey("march")->intValue();
    }
    if (dict->objectForKey("health")) {
        health = dict->valueForKey("health")->intValue();
    }
    if (dict->objectForKey("power")) {
        power = dict->valueForKey("power")->floatValue();
    }
    if(dict->objectForKey("train")){
        train = dict->valueForKey("train")->intValue();
    }
    if (dict->objectForKey("finish")) {
        finishTime = 0;
        setFinishiTime(dict);
    }
}

std::string ArmyInfo::getRealItemId(){
//    int id = atoi(armyId.c_str());
//    return CC_ITOA(id);
    return armyId;
}

std::string ArmyInfo::getName(){
    std::string itemId = getRealItemId();
    return CCCommonUtils::getNameById(itemId);
}

std::string ArmyInfo::getIcon(){
    std::string itemId = getRealItemId();
    return CCCommonUtils::getPropById(itemId, "icon") + ".png";
}

std::string ArmyInfo::getBodyIcon(){
    std::string itemId = getRealItemId();
    return "ico" + itemId+ ".png";
}

std::string ArmyInfo::getModelName(){
    std::string itemId = getRealItemId();
    return "3d/soldier/" + CCCommonUtils::getPropById(itemId, "icon") + ".c3b";
//    return "3d/soldier/c3d10000.c3b";
}

std::string ArmyInfo::getModelTexName(){
    std::string itemId = getRealItemId();
    return "3d/soldier/" + CCCommonUtils::getPropById(itemId, "image") + ".jpg";
//    return "3d/soldier/c3d10000.jpg";
}


void ArmyInfo::getModelAniByName(int actIndex, std::vector<std::string>& ret){
    // tao.yu  ret的三个string分别是 动作名 起始帧数 结束帧数
    std::string itemId = getRealItemId();
    std::string anims = CCCommonUtils::getPropById(itemId, "animations");
    vector<string> strArr;
    CCCommonUtils::splitString(anims, "|", strArr);
    
    if (actIndex >= strArr.size())
    {
        ret.push_back("");
        ret.push_back("");
        ret.push_back("");
        return;
    }
    
    vector<string> strArr1;
    CCCommonUtils::splitString(strArr.at(actIndex), ";", strArr1);
    ret.push_back("3d/soldier/"+strArr1.at(0)+".c3b");
    ret.push_back(strArr1.at(1));
    ret.push_back(strArr1.at(2));

//    for (int i = 0; i < strArr.size(); ++i) {
//        vector<string> strArr1;
//        CCCommonUtils::splitString(strArr.at(i), ";", strArr1);
//        if (strArr1.at(0) == name) {
//            string ico = getModelName();
//            int pos = ico.find(".c3b");
//            ico.insert(pos, "_"+strArr1.at(0));
//            ret.push_back(ico);
//            ret.push_back(strArr1.at(1));
//            ret.push_back(strArr1.at(2));
//            return;
//        }
//        
//    }
}

std::string ArmyInfo::getHeadIcon(){
    std::string itemId = getRealItemId();

    
    int soldierId = CCString::create(itemId)->intValue();
    
    
    
    return "ico" + itemId+ "_small.png";
}

void ArmyInfo::setFinishiTime(cocos2d::CCDictionary *dict){
    std::string str = dict->valueForKey("finish")->getCString();
    if(str != ""){
        finishTime = atof(str.c_str()) / 1000;
        if(finishTime != 0){
            finishTime = GlobalData::shared()->changeTime(finishTime);
        }
        train = dict->valueForKey("train")->intValue();
    }
}

std::string ArmyInfo::getDesc(){
    std::string itemId = getRealItemId();
    std::string dialog = CCCommonUtils::getPropById(itemId, "description");
    return _lang(dialog).c_str();
}

void ArmyInfo::setEndTime(double time, bool isCancel){
    // tao.yu 这个函数根本进不来
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    if(time == 0){
        if(!isCancel){
            free += train;
//            std::string icon = getBodyIcon();
            std::string icon = "";
            std::string name = getName();
            if(isArmy){
                int index = atof(armyId.c_str())/100 - 1070 + 200;
                CCLoadSprite::doResourceByCommonIndex(index, true);
                CCCommonUtils::flyHint(icon, _lang("103673"), _lang_1("103674", name.c_str()));
                CCLoadSprite::doResourceByCommonIndex(index, false);
            }else{
                CCCommonUtils::flyHint(icon, _lang("103675"), _lang_1("103676", name.c_str()));
            }

        }
        train = 0;
    }
    finishTime = time;
}

std::string ArmyInfo::getAttrIcon(int index)
{
    std::string itemId = getRealItemId();
    std::string icon = CCCommonUtils::getPropById(itemId, "featureicon");
    vector<string> strArr;
    CCCommonUtils::splitString(icon, ";", strArr);
    if (index >= strArr.size())
    {
        return "";
    }
    return strArr.at(index);
}

void ArmyInfo::getAttrDesc(int index, vector<string>& descVec)
{
    /*type==0取title, type==1取info*/
    std::string itemId = getRealItemId();
    std::string desc = CCCommonUtils::getPropById(itemId, "featuredescription");
    vector<string> strArr;
    CCCommonUtils::splitString(desc, "|", strArr);
    if (index >= strArr.size())
    {
        descVec.push_back("");
        descVec.push_back("");
        return;
    }
    vector<string> strArr1;
    CCCommonUtils::splitString(strArr.at(index), ";", strArr1);
    descVec.push_back(strArr1.at(0));
    descVec.push_back(strArr1.at(1));
}
