//
//  PlayerInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-31.
//
//

#include "PlayerInfo.h"
#include "VipUtil.h"
#include "GlobalData.h"
#include "LocalController.h"
#include "CCCommonUtils.h"

bool PlayerInfo::isInAlliance()
{
    return m_allianceId.length() != 0;
}

void PlayerInfo::updateInfo(CCDictionary* dict){
    lastBattlePower = this->getTotalPower();
    if(dict->objectForKey("serverName")){
        serverName = dict->valueForKey("serverName")->getCString();
    }
    if(dict->objectForKey("serverId")){
        selfServerId = dict->valueForKey("serverId")->intValue();
    }
    if(dict->objectForKey("sciencePower")){
        sciencePower = dict->valueForKey("sciencePower")->doubleValue();
    }
    if(dict->objectForKey("battleLose")){
        battleLose = dict->valueForKey("battleLose")->intValue();
    }
    if(dict->objectForKey("armyDead")){
        armyDead = dict->valueForKey("armyDead")->intValue();
    }
    if(dict->objectForKey("armyPower")){
        armyPower = dict->valueForKey("armyPower")->doubleValue();
    }
    if(dict->objectForKey("fortPower")){
        fortPower = dict->valueForKey("fortPower")->doubleValue();
    }
    if(dict->objectForKey("armyCure")){
        armyCure = dict->valueForKey("armyCure")->intValue();
    }
    if(dict->objectForKey("battleWin")){
        battleWin = dict->valueForKey("battleWin")->intValue();
    }
    if(dict->objectForKey("playerPower")){
        playerPower = dict->valueForKey("playerPower")->doubleValue();
    }
    if(dict->objectForKey("armyKill")){
        armyKill = dict->valueForKey("armyKill")->intValue();
    }
    if(dict->objectForKey("buildingPower")){
        buildingPower = dict->valueForKey("buildingPower")->doubleValue();
    }
    if (dict->objectForKey("equipPower")) {
        equipPower = dict->valueForKey("equipPower")->doubleValue();
    }
    if(dict->objectForKey("defenseWin")){
        defenseWin = dict->valueForKey("defenseWin")->intValue();
    }
    if(dict->objectForKey("heroPower")){
        heroPower = dict->valueForKey("heroPower")->doubleValue();
    }
    if(dict->objectForKey("attackLose")){
        attackLose = dict->valueForKey("attackLose")->intValue();
    }
    if(dict->objectForKey("detectNum")){
        detectNum = dict->valueForKey("detectNum")->intValue();
    }
    if(dict->objectForKey("cityDes")){
        cityDes = dict->valueForKey("cityDes")->intValue();
    }
    if(dict->objectForKey("attackWin")){
        attackWin = dict->valueForKey("attackWin")->intValue();
    }
    if(dict->objectForKey("questpower")){
        questpower = dict->valueForKey("questpower")->doubleValue();
    }
    if(dict->objectForKey("name")){
        name = dict->valueForKey("name")->getCString();
    }
    if(dict->objectForKey("uid")){
        uid = dict->valueForKey("uid")->getCString();
    }
    if(dict->objectForKey("score")){
        
        int oldVipPoints = vipPoints;
        vipPoints = dict->valueForKey("score")->intValue();
        if (vipPoints>oldVipPoints && uid==GlobalData::shared()->playerInfo.uid) {
            updateVipEffect();
        }
    }
    if(dict->objectForKey("vipEndTime")){
        vipEndTime = dict->valueForKey("vipEndTime")->intValue();
    }
    if(dict->objectForKey("power")){
        power = dict->valueForKey("power")->doubleValue();
    }
    if(dict->objectForKey("isApplied")){
        isApplied = dict->valueForKey("isApplied")->intValue();
    }
    if(dict->objectForKey("rank")){
        rank = dict->valueForKey("rank")->intValue();
    }
    if(dict->objectForKey("gmFlag")){
        gmFlag = dict->valueForKey("gmFlag")->intValue();
    }
    if(dict->objectForKey("pointId")){
        pointId = dict->valueForKey("pointId")->intValue();
    }
    if(dict->objectForKey("invited")){
        invited = dict->valueForKey("invited")->boolValue();
    }
    if(dict->objectForKey("playerName")){//对象起的别名
        name = dict->valueForKey("playerName")->getCString();
    }
    if(dict->objectForKey("playerId")){//对象起的别名
        uid = dict->valueForKey("playerId")->getCString();
    }
    if(dict->objectForKey("exp")){
        exp = dict->valueForKey("exp")->intValue();
    }
    if(dict->objectForKey("maxExp")){
        maxExp = dict->valueForKey("maxExp")->intValue();
    }
    if(dict->objectForKey("level")){
        level = dict->valueForKey("level")->intValue();
    }
    if(dict->objectForKey("mainCityLv")){
        mainCityLv = dict->valueForKey("mainCityLv")->intValue();
    }
    if(dict->objectForKey("pic")){
        pic = dict->valueForKey("pic")->getCString();
    }
    if(dict->objectForKey("regCountry")){
        regCountry = dict->valueForKey("regCountry")->getCString();
    }
    if(pic==""||pic=="0"){
        pic = "g044";
    }
    if(dict->objectForKey("userGenerals")){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("userGenerals"));
        if (m_generals == NULL) {
            m_generals = CCArray::create();
        }else{
            m_generals->removeAllObjects();
        }
        int length = arr->count();
        for (int i=0; i<length; i++) {
            auto dic = _dict(arr->objectAtIndex(i));
            GeneralInfo* info = new GeneralInfo(dic);
            info->autorelease();
            m_generals->addObject(info);
        }
    }
    if (dict->objectForKey("picVer"))
    {
        picVer = dict->valueForKey("picVer")->intValue();
    }
    if (dict->objectForKey("lastUpdateTime"))
    {
        lastUpdateTime = dict->valueForKey("lastUpdateTime")->intValue();
    }
    if(dict->objectForKey("officer")){
        officer = dict->valueForKey("officer")->getCString();
    }
    
    if(dict->objectForKey("showEquip")){//是否装备别人可见
        showEquip = dict->valueForKey("showEquip")->boolValue();
    }
    if(dict->objectForKey("banTime")){
        pic_banTime = dict->valueForKey("banTime")->doubleValue();
    }
    if(dict->objectForKey("nextUpdateTime")){
        pic_nextUpdateTime = dict->valueForKey("nextUpdateTime")->doubleValue();
    }
    if(dict->objectForKey("banCount")){
        pic_banCount = dict->valueForKey("banCount")->intValue();
    }
    
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("equip"));
    m_equip.clear();
    if( arr ){
        CCDictionary *item = NULL;
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            int itemId = item->valueForKey("itemId")->intValue();
            m_equip.push_back(itemId);
        }
    }
    
//#if COCOS2D_DEBUG > 0
//    picVer = 1;
//#endif
    battlePower = this->getTotalPower();
}

double PlayerInfo::getTotalPower(){
    double total = buildingPower + sciencePower + armyPower + fortPower + questpower + playerPower + heroPower + equipPower;
    total = MAX(total, 0);
    return total;
}

void PlayerInfo::updateVipEffect(bool isXml)
{
    int lv = VipUtil::getVipLevel(vipPoints);
    GlobalData::shared()->vipEffectMap.clear();
    string key = CC_ITOA(7000+lv-1);
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(key);
    if (dict) {
        if (dict->objectForKey("effect")) {
            string effectStr = dict->valueForKey("effect")->getCString();
            vector<string> vecEffect;
            CCCommonUtils::splitString(effectStr, "|", vecEffect);
            for (int i=0; i<vecEffect.size(); i++) {
                vector<string> effectCell;
                CCCommonUtils::splitString(vecEffect[i], ";", effectCell);
                if (effectCell.size()==2) {
                    GlobalData::shared()->vipEffectMap[atoi(effectCell[0].c_str())] = atoi(effectCell[1].c_str());
                }
            }
        }
    }
    if(isXml){
        CCDictionary* vipGroup = LocalController::shared()->DBXMLManager()->getGroupByKey("vip");
        if(vipGroup)
        {
            CCDictElement* element;
            CCDICT_FOREACH(vipGroup, element)
            {
                CCDictionary* dictInfo = _dict(element->getObject());
                dictInfo->retain();
                string vipId = dictInfo->valueForKey("id")->getCString();
                GlobalData::shared()->vipListMap[atoi(vipId.c_str())] = dictInfo;
            }
        }
    }
}

void PlayerInfo::parseGenerals(CCArray* array){
    if(array==NULL || array->count()<=0) return;
    if (m_generals == NULL) {
        m_generals = new CCArray();
        m_generals->init();
    }else{
        m_generals->removeAllObjects();
    }
    int length = array->count();
    for (int i=0; i<length; i++) {
        auto dic = _dict(array->objectAtIndex(i));
        GeneralInfo* info = new GeneralInfo(dic);
        m_generals->addObject(info);
    }
}

string PlayerInfo::getPic(){
    if(pic=="" || pic=="0"){
        return "g044.png";
    }
    std::string head = pic;
    head.append(".png");
    return head;
}

//begin a by ljf
string PlayerInfo::getVipPic(){
    if(pic=="" || pic=="0"){
        return "g044_vip.png";
    }
    std::string head = pic;
    head.append("_vip.png");
    return head;
}
//end a by ljf

string PlayerInfo::getBodyPic(){
    if(pic=="" || pic=="0"){
//        return "g044_bust.png";
        return "g044_middle.png";
    }
    std::string body = pic;
//    body.append("_bust.png");
    body.append("_middle.png");
    return body;
}

bool PlayerInfo::isInSelfServer(){
    return currentServerId == selfServerId;
}

bool PlayerInfo::isMOD(){
    if(gmFlag == 2 || gmFlag == 4 || gmFlag == 5){
        return true;
    }
    return false;
}

int PlayerInfo::getAttributeByKey(string key)
{
    if (key == "war_win") {
        return battleWin;
    }else if (key == "war_lose") {
        return battleLose;
    }else if (key == "attack_win") {
        return attackWin;
    }else if (key == "attack_lose") {
        return attackLose;
    }else if (key == "defense_win") {
        return defenseWin;
    }else if (key == "win_rate") {
        if((battleWin+attackLose)!=0){
            return  battleWin*100/(battleWin+battleLose);
        }else{
            return 0;
        }
    }else if (key == "scout_num") {
        return detectNum;
    }else if (key == "kill_troops") {
        return armyKill;
    }else if (key == "loss_troops") {
        return armyDead;
    }else if (key == "heal_troops") {
        return armyCure;
    }else if (key == "power_sum") {
        return (playerPower+armyPower+buildingPower+sciencePower+fortPower + equipPower);
    }else if (key == "power_lord") {
        return playerPower;
    }else if (key == "power_troops") {
        return armyPower;
    }else if (key == "power_build") {
        return buildingPower;
    }else if (key == "power_science") {
        return sciencePower;
    }else if (key == "power_trap") {
        return fortPower;
    } else if (key == "power_equip"){
        return equipPower;
    }else {
        return -1;
    }
}

string PlayerInfo::getCustomPicUrl()
{
    return CCCommonUtils::getCustomPicUrl(uid, picVer);
}

bool PlayerInfo::isUseCustomPic()
{
    if (CCCommonUtils::isUseCustomPic(picVer))
    {
        return true;
    }
    else
    {
        return false;
    }
}