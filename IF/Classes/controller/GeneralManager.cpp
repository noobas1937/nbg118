//
//  GeneralManager.cpp
//  IF
//
//  Created by 李锐奇 on 13-11-27.
//
//

#include "GeneralManager.h"
#include "GeneralCommand.h"
#include "FunBuildController.h"

static GeneralManager* _instance;

GeneralManager::GeneralManager()
{
    auto LoadDetails = LocalController::shared()->DBXMLManager()->getGroupByKey("lordDetails");
    if(LoadDetails)
    {
        CCDictElement* element;
        CCDICT_FOREACH(LoadDetails, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            int id = dictInfo->valueForKey("id")->intValue();
            auto attInfo = AttributeInfo(dictInfo);
            
            GeneralAttMap[id] = attInfo;
            GeneralAttAreaMap[attInfo.area].push_back(id);
        }
    }
}

GeneralManager::~GeneralManager() {
}

GeneralManager* GeneralManager::getInstance(){
    if(!_instance){
        _instance = new GeneralManager();
        _instance->initSortAttribute();
    }
    return _instance;
}

bool compareAttribute(int id1, int id2)
{
    auto& info1 = GeneralManager::getInstance()->GeneralAttMap[id1];
    auto& info2 = GeneralManager::getInstance()->GeneralAttMap[id2];
    if (info1.type < info2.type) {
        return true;
    }
    else {
        return false;
    }
}

void GeneralManager::initSortAttribute()
{
    for (int i=1; ; i++) {
        if (GeneralAttAreaMap.find(i) == GeneralAttAreaMap.end()) {
            break;
        }
        sort(GeneralAttAreaMap[i].begin(), (GeneralAttAreaMap[i]).end(), compareAttribute);
    }
}

void GeneralManager::addGeneral(CCDictionary* dic){
    string uuid = dic->valueForKey("uuid")->getCString();
    string generalId = dic->valueForKey("generalId")->getCString();
    GeneralInfo info = GeneralInfo(dic);
    info.maxExp = GlobalData::shared()->playerInfo.maxExp;
    info.currExp = GlobalData::shared()->playerInfo.exp;
    info.level = GlobalData::shared()->playerInfo.level;
    addGeneral(info);
}

void GeneralManager::addGeneral(GeneralInfo &info){

    GlobalData::shared()->generals[info.uuid] = info;
    std::map<std::string, std::map<std::string, GeneralSkillInfo*>>::iterator abilityIt;
    for(abilityIt = info.generalSkillMap.begin(); abilityIt != info.generalSkillMap.end(); abilityIt++){
        std::map<std::string, GeneralSkillInfo*>::iterator skillIt;
        for(skillIt = abilityIt->second.begin(); skillIt != abilityIt->second.end(); skillIt++){
            skillIt->second->retain();
        }
    }
    GlobalData::shared()->generalAlreadyGet[info.generalId] = 1;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GENERAL_NUM_CHANGE);
    if(info.state == GENERAL_PVE){
        int i = 1;
        GeneralManager::getInstance()->defenceGeneral.clear();
        map<std::string, GeneralInfo>::iterator it;
        for(it = GlobalData::shared()->generals.begin(); it != GlobalData::shared()->generals.end(); it++){
            if(it->second.state == GENERAL_PVE){
                GeneralManager::getInstance()->defenceGeneral[i] = it->first;
                i++;
            }
        }
        while(i < 4){
            GeneralManager::getInstance()->defenceGeneral[i] = "";
            i++;
        }
    }
}

bool GeneralManager::isAlreadyGetGeneral(std::string generalId){
    map<std::string, int>::iterator it = GlobalData::shared()->generalAlreadyGet.find(generalId);
    if(it == GlobalData::shared()->generalAlreadyGet.end()){
        return false;
    }
    return true;
}

void GeneralManager::removeGeneralByUid(std::string uid){
    GeneralFireCommand *cmd = new GeneralFireCommand(uid);
    cmd->sendAndRelease();
    map<std::string, GeneralInfo>::iterator it = GlobalData::shared()->generals.find(uid);
    if(it != GlobalData::shared()->generals.end()){
        it->second.release();
        GlobalData::shared()->generals.erase(it);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GENERAL_NUM_CHANGE);
    }
}

int GeneralManager::getGeneralNumById(std::string generalId){
    int count = 0;
    map<std::string, GeneralInfo>::iterator it;
    for(it = GlobalData::shared()->generals.begin(); it != GlobalData::shared()->generals.end(); it++){
        if(it->second.generalId == generalId && it->second.pos == 0){
            count++;
        }
    }
    return count;
}

bool GeneralManager::isGeneralFull(){
    if(GlobalData::shared()->generals.size() >= getTotalOpenGeneralPosition()){
        return true;
    }
    return false;
}

std::string GeneralManager::getGeneralArmyIcon(std::string generalId){
    std::string armyId = CCCommonUtils::getPropById(generalId, "arms");
    std::string type = CCCommonUtils::getPropById(armyId, "arm");
    return string("") + "arm" + type + "_icon.png";
}

std::string GeneralManager::getGeneralLevelStr(int level){
    if(level >= GlobalData::shared()->MaxHeroLv){
        return _lang("102183");
    }
    return CC_ITOA(level);
}

int GeneralManager::getTotalOpenGeneralPosition(){
    int free = GlobalData::shared()->generalConfig.bagFreeNum;
    int buy = GlobalData::shared()->bagGoldBuyNum;
    int freeFromBuilding = 0;
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
    if(bid != 0){
        map<int, FunBuildInfo>::iterator it = GlobalData::shared()->imperialInfo.find(bid);
        if(it != GlobalData::shared()->imperialInfo.end()){
            freeFromBuilding = it->second.para1;
        }
    }
    return free + buy + freeFromBuilding;
}

int GeneralManager::getBustPlistIndex(std::string generalId){
    std::string iconStr = CCCommonUtils::getPropById(generalId, "icon");
    std::string index = iconStr.substr(iconStr.length()-3,3);
    int resIndx = atoi(index.c_str());
    resIndx = (resIndx - 1) / 4 + 1;
    return resIndx;
}

void GeneralManager::addGeneralAbility(GeneralInfo &generalInfo, std::string itemId){
    std::string skills = CCCommonUtils::getPropById(itemId, "skill");
    std::string posStr = CCCommonUtils::getPropById(itemId, "position");
    std::string relationStr = CCCommonUtils::getPropById(itemId, "relation");
    vector<std::string> posVector;
    vector<std::string> relationVector;
    vector<std::string> vector1;
    vector<std::string> vector2;
    vector<std::string> vector;
    CCCommonUtils::splitString(skills, "|", vector);
    CCCommonUtils::splitString(posStr, "|", vector1);
    CCCommonUtils::splitString(relationStr, "|", vector2);

    if(vector.size() == 0){
        return;
    }
    int i = 0;
    int j = 0;
    while(i < vector.size()){
        posVector.clear();
        relationVector.clear();
        std::string skillId = vector[i];
        string temp = "";
        if (vector1.size()>i) {
            temp = vector1[i];
        }
        CCCommonUtils::splitString(vector1[i], ";", posVector);
        if(vector2.size() > i && vector2[i] != ""){
            CCCommonUtils::splitString(vector2[i], ";", relationVector);
        }
        GeneralSkillInfo *info = GeneralSkillInfo::create(skillId, 0, generalInfo.uuid, itemId);
        info->x = atoi(posVector[1].c_str());
        info->y = atoi(posVector[0].c_str());
        j = 0;
        while(j < relationVector.size()){
            info->next.push_back(relationVector[j]);
            j++;
        }
        info->retain();
        generalInfo.generalSkillMap[itemId][skillId] = info;
        i++;
    }
    generalInfo.ability.push_back(itemId);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GENERAL_ABILITY_CHOOSE);
}

void GeneralManager::removeGeneralAbility(GeneralInfo &generalInfo, std::string itemId){
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator it = generalInfo.generalSkillMap.find(itemId);
    if(it != generalInfo.generalSkillMap.end()){
        map<std::string, GeneralSkillInfo*>::iterator skillIt;
        for(skillIt = it->second.begin(); skillIt != it->second.end(); skillIt++){
            CC_SAFE_RELEASE(skillIt->second);
        }
        it->second.clear();
        generalInfo.generalSkillMap.erase(it);
    }
    vector<std::string> tmpVector;
    int index = 0;
    while(index < generalInfo.ability.size()){
        std::string id = generalInfo.ability[index];
        if(itemId == id){
            
        }else{
            tmpVector.push_back(id);
        }
        index++;
    }
    generalInfo.ability.clear();
    index = 0;
    while(index < tmpVector.size()){
        generalInfo.ability.push_back(tmpVector[index]);
        index++;
    }
    tmpVector.clear();
}

void GeneralManager::resetGeneralAbility(GeneralInfo &generalInfo, std::string str){
    generalInfo.abilityToChoose.clear();
    CCCommonUtils::splitString(str, "|", generalInfo.abilityToChoose);
}

int GeneralManager::getTotalSkillPoint(GeneralInfo &generalInfo){
//    if(GlobalData::shared()->generalConfig.skillPointAddPerLevel.find(generalInfo.color) == GlobalData::shared()->generalConfig.skillPointAddPerLevel.end()){
//        return 0;
//    }
//    int skillPointPerLevel = GlobalData::shared()->generalConfig.skillPointAddPerLevel[generalInfo.color];
    int userLv = GlobalData::shared()->lordInfo.levelUp;
    int key = 100100 + userLv;
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(key));
    int curEndowment = dict->valueForKey("endowment")->intValue();
    return curEndowment;
    //return generalInfo.level * skillPointPerLevel;
}

int GeneralManager::getTotalUsedSkillPoint(GeneralInfo &generalInfo){
    int ret = 0;
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator it;
    for(it = generalInfo.generalSkillMap.begin(); it != generalInfo.generalSkillMap.end(); it++){
        map<std::string, GeneralSkillInfo*>::iterator skillIt;
        for(skillIt = it->second.begin(); skillIt != it->second.end(); skillIt++){
            ret += skillIt->second->getTotalSkillPoint();
        }
    }
    return ret;
}

int GeneralManager::getTotalFreeSkillPoint(GeneralInfo &generalInfo){
    int totalPt = getTotalSkillPoint(generalInfo);
    int usedPt = getTotalUsedSkillPoint(generalInfo);
    return totalPt - usedPt;
}

void GeneralManager::updateGeneralSkill(GeneralInfo &generalInfo, std::string activityId, std::string skillId, int level){
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator it = generalInfo.generalSkillMap.find(activityId);
    if(it != generalInfo.generalSkillMap.end()){
        map<std::string, GeneralSkillInfo*>::iterator skillIt = it->second.find(skillId);
        if(skillIt != it->second.end()){
            skillIt->second->level = level;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GENERAL_SKILL_UPDATE);
        }
    }
}

int GeneralManager::getTotalAbilitySkillPoint(GeneralInfo &generalInfo, std::string abilityId){
    int ret = 0;
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator it = generalInfo.generalSkillMap.find(abilityId);
    if(it != generalInfo.generalSkillMap.end()){
        map<std::string, GeneralSkillInfo*>::iterator skillIt;
        for(skillIt = it->second.begin(); skillIt != it->second.end(); skillIt++){
            ret += skillIt->second->getTotalSkillPoint();
        }
    }
    return ret;
}

int GeneralManager::getTotalOpenedAbility(GeneralInfo &generalInfo){
    int max = GlobalData::shared()->generalConfig.generalMaxAbility[generalInfo.color];
    int ret = 0;
    map<int, int>::iterator it;
    for(it = GlobalData::shared()->generalConfig.abilityOpenedLevel.begin(); it != GlobalData::shared()->generalConfig.abilityOpenedLevel.end(); it++){
        if(ret < it->second && it->first <= generalInfo.level){
            ret = it->second;
        }
    }
    return ret > max ? max : ret;
}

int GeneralManager::getNextAbilityOpenLevel(GeneralInfo &generalInfo){
    int total = getTotalOpenedAbility(generalInfo);
    int max = GlobalData::shared()->generalConfig.generalMaxAbility[generalInfo.color];
    if(total >= max){
        return -1;
    }
    int result = INT_MAX;
    map<int, int>::iterator it;
    for(it = GlobalData::shared()->generalConfig.abilityOpenedLevel.begin(); it != GlobalData::shared()->generalConfig.abilityOpenedLevel.end(); it++){
        if(result > it->first && it->first > generalInfo.level){
            result = it->first;
        }
    }
    return result;
}

int GeneralManager::getGeneralAbilityNum(){
    return GlobalData::shared()->generalConfig.generalMaxAbility[WHITE];
}

float GeneralManager::getValueByEffect(int effect){
    std::map<int, float>::iterator it = generalSkillEffect.find(effect);
    if(it != generalSkillEffect.end()){
        return it->second;
    }
    return 0.0f;
}

void GeneralManager::resetGeneralSkillEffectValue(){
    generalSkillEffect.clear();
    vector<std::string> v1;
    vector<std::string> v2;
    vector<std::string> v3;

    std::map<std::string, GeneralInfo>::iterator generalIt;
    for(generalIt = GlobalData::shared()->generals.begin(); generalIt != GlobalData::shared()->generals.end(); generalIt++)
    {
        std::map<std::string, map<std::string, GeneralSkillInfo*>>::iterator abilityIt;
        for(abilityIt = generalIt->second.generalSkillMap.begin(); abilityIt != generalIt->second.generalSkillMap.end(); abilityIt++)
        {
            std::map<std::string, GeneralSkillInfo*>::iterator skillIt;
            for(skillIt = abilityIt->second.begin(); skillIt != abilityIt->second.end(); skillIt++){
                if(skillIt->second->level > 0){
                    int type = atoi(CCCommonUtils::getPropById(skillIt->second->skillId, "type").c_str());
                    if(type != 0){
                        continue;
                    }
                    v1.clear();
                    v2.clear();
                    v3.clear();
                    std::string paraStr = CCCommonUtils::getPropById(skillIt->second->skillId, "para1");
                    if(paraStr == ""){
                        continue;
                    }
                    std::string baseStr = CCCommonUtils::getPropById(skillIt->second->skillId, "base");
                    if(baseStr == ""){
                        continue;
                    }

                    std::string addStr = CCCommonUtils::getPropById(skillIt->second->skillId, "add");
                    if(addStr == ""){
                        continue;
                    }

                    CCCommonUtils::splitString(paraStr, "|", v1);
                    CCCommonUtils::splitString(baseStr, "|", v2);
                    CCCommonUtils::splitString(addStr, "|", v3);
                    if(v1.size() != v2.size() || v1.size() != v3.size() || v2.size() != v3.size()){
                        continue;
                    }
                    
                    int showType  = atoi(CCCommonUtils::getPropById(skillIt->second->skillId, "type_value").c_str());
                    int i = 0;
                    while(i < v1.size()){
                        int effect = atoi(v1[i].c_str());
                        float base = atoi(v2[i].c_str());
                        float add = atoi(v3[i].c_str());

                        float value = base + (skillIt->second->level - 1) * add;
                        if(showType == 0){
                            value = value / 100.0f;
                        }
                        auto effectIt = generalSkillEffect.find(effect);
                        if(effectIt != generalSkillEffect.end()){
                            effectIt->second += value;
                        }else{
                            generalSkillEffect[effect] = value;
                        }
                        i++;
                    }
                }
            }
        }
    }
}

bool GeneralManager::isSkillClick(std::string generalUid, std::string skillId){
    std::string key = generalUid + "_" + skillId;
    int value = CCUserDefault::sharedUserDefault()->getIntegerForKey(key.c_str());
    return value == 1;
}

void GeneralManager::saveSkillClick(std::string generalUid, std::string skillId){
    std::string key = generalUid + "_" + skillId;
    CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), 1);
    CCUserDefault::sharedUserDefault()->flush();
}

bool GeneralManager::isAbilityLevelClick(std::string generalUid, int level){
    std::string key = generalUid + "_clickLevel";
    int value = CCUserDefault::sharedUserDefault()->getIntegerForKey(key.c_str());
    return value >= level;
}

void GeneralManager::saveAbilityLevelClick(std::string generalUid, int level){
    std::string key = generalUid + "_clickLevel";
    int value = CCUserDefault::sharedUserDefault()->getIntegerForKey(key.c_str());
    if(value >= level){
        return;
    }
    CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), level);
    CCUserDefault::sharedUserDefault()->flush();
}

int GeneralManager::getProtectTime(){
    map<std::string, SkillCDInfo*>::iterator it = SkillCDMap.find("612000");
    double gapTime = 0;
    if(it != SkillCDMap.end()){
        gapTime = GlobalData::shared()->getWorldTime() - it->second->endTime;
    }
    if (gapTime<=0) {
        gapTime = 0;
    }
    return (int)gapTime;
}

string GeneralManager::getBgBySkill(std::string skillId){
    string bg = "";
    std::string skills = CCCommonUtils::getPropById("50000", "skill");
    vector<std::string> vector;
    CCCommonUtils::splitString(skills, "|", vector);
    int num = vector.size();
    for(int i=0;i<num;i++){
        if(vector[i]==skillId){
            bg = "UI_Skill_icon_frame_red.png";
        }
    }
    if(bg==""){
        skills = CCCommonUtils::getPropById("50001", "skill");
        vector.clear();
        CCCommonUtils::splitString(skills, "|", vector);
        num = vector.size();
        for(int i=0;i<num;i++){
            if(vector[i]==skillId){
                bg = "UI_Skill_icon_frame_blue.png";
            }
        }
    }
    if(bg==""){
        skills = CCCommonUtils::getPropById("50002", "skill");
        vector.clear();
        CCCommonUtils::splitString(skills, "|", vector);
        num = vector.size();
        for(int i=0;i<num;i++){
            if(vector[i]==skillId){
                bg = "UI_Skill_icon_frame_green.png";
            }
        }
    }
    if(bg==""){
        bg = "UI_Skill_icon_frame_red.png";
    }
    return bg;
}

void GeneralManager::saveSkill(std::string abilityId, std::string skillId){
    CCDictionary *dict = CCDictionary::create();
    GeneralInfo* m_info = &GlobalData::shared()->generals.begin()->second;
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator it = m_info->generalSkillMap.find(abilityId);
    if(it == m_info->generalSkillMap.end()){
        return;
    }
    map<std::string, GeneralSkillInfo*>::iterator skillIt;
    for(skillIt = it->second.begin(); skillIt != it->second.end(); skillIt++){
        if(skillIt->second->level != 0){
            dict->setObject(CCInteger::create(skillIt->second->level), skillIt->first);
        }
    }
    GeneralManager::getInstance()->resetGeneralSkillEffectValue();
    GeneralSkillSaveCommand *cmd = new GeneralSkillSaveCommand(m_info->uuid, abilityId, dict,true,skillId);
    cmd->sendAndRelease();
}

void GeneralManager::saveSkillFail(std::string abilityId, std::string skillId){
    GeneralInfo* m_info = &GlobalData::shared()->generals.begin()->second;
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator it = m_info->generalSkillMap.find(abilityId);
    if(it == m_info->generalSkillMap.end()){
        return;
    }
    map<std::string, GeneralSkillInfo*>::iterator skillIt = it->second.find(skillId);
    if(skillIt!=it->second.end()){
        skillIt->second->level =  skillIt->second->level - 1;
    }
    GeneralManager::getInstance()->resetGeneralSkillEffectValue();
}