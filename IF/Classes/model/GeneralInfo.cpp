//
//  GeneralInfo.cpp
//  IF
//
//  Created by wangzhenlei on 13-9-22.
//
//

#include "GeneralInfo.h"
#include "UIComponent.h"
#include "GeneralManager.h"

GeneralInfo::GeneralInfo(CCDictionary* dict) {
    if (dict->objectForKey("generalId")) {
        generalId = dict->valueForKey("generalId")->getCString();
    }
    if (dict->objectForKey("level")) {
        level = dict->valueForKey("level")->intValue();
    }
    if (dict->objectForKey("att")) {
        attack = dict->valueForKey("att")->intValue();
    }
//    color = dict->valueForKey("color")->intValue();
    color = 0;
    if (dict->objectForKey("defence")) {
        defense = dict->valueForKey("defence")->intValue();
    }
    if (dict->objectForKey("exp")) {
        currExp = dict->valueForKey("exp")->intValue();
    }
    if (dict->objectForKey("maxExp")) {
        maxExp = dict->valueForKey("maxExp")->intValue();
    }
    if (dict->objectForKey("status")) {
        state = dict->valueForKey("status")->intValue();
    }
    if (dict->objectForKey("uuid")) {
        uuid = dict->valueForKey("uuid")->getCString();
    }
    if (dict->objectForKey("maxForces")) {
        maxForces = dict->valueForKey("maxForces")->intValue();
    }
    if (dict->objectForKey("pos")) {
        pos = dict->valueForKey("pos")->intValue();
    }
    if (dict->objectForKey("hp")) {
        hp = dict->valueForKey("hp")->intValue();
    }
    if (dict->objectForKey("num")) {
        num = dict->valueForKey("num")->intValue();
    }
    if (dict->objectForKey("rank")) {
        rank = dict->valueForKey("rank")->intValue();
    }
    
    GeneralManager::getInstance()->addGeneralAbility(*this, "50000");
    GeneralManager::getInstance()->addGeneralAbility(*this, "50001");
    GeneralManager::getInstance()->addGeneralAbility(*this, "50002");
    if(dict->objectForKey("ability")){
        CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("ability"));
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj) {
            CCDictionary *abilityDict = dynamic_cast<CCDictionary*>(obj);
            if(abilityDict)
            {
                if(abilityDict->objectForKey("ids")){
                    std::string ids = abilityDict->valueForKey("ids")->getCString();
                    abilityToChoose.clear();
                    CCCommonUtils::splitString(ids, "|", abilityToChoose);
                }else{
                    CCDictElement *ele;
                    std::string abilityId = abilityDict->valueForKey("id")->getCString();
                    //GeneralManager::getInstance()->addGeneralAbility(*this, abilityId);
                    if(generalSkillMap.find(abilityId) == generalSkillMap.end()){
                        continue;
                    }
                    auto map = generalSkillMap[abilityId];
                    CCDICT_FOREACH(abilityDict, ele) {
                        std::string key = ele->getStrKey();
                        std::string value = abilityDict->valueForKey(key)->getCString();
                        if(map.find(key) == map.end()){
                            continue;
                        }
                        if(key != "id" && map.size() != 0){
                            auto itSkill = map.find(key);
                            if (itSkill!=map.end()) {
                                map[key]->level = atoi(value.c_str());
                            }
                        }
                    }
                }
            }
        }
    }
}

void GeneralInfo::parseCDTime(CCDictionary* dict){
    GeneralManager::getInstance()->SkillCDMap.clear();
    CCArray* skillsArr = dynamic_cast<CCArray*>(dict->objectForKey("skill"));
    if (skillsArr!=NULL) {
        int num = skillsArr->count();
        for (int i=0;i<num; i++) {
            auto cdDic = _dict(skillsArr->objectAtIndex(i));
            if (cdDic) {
                string skillId = cdDic->valueForKey("skillId")->getCString();
                SkillCDInfo* cdInfo = SkillCDInfo::create();
                cdInfo->parse(cdDic);
                cdInfo->retain();
                GeneralManager::getInstance()->SkillCDMap[skillId] = cdInfo;
            }
        }
    }
}

void GeneralInfo::setXMLGeneralInfo(std::string generalId){
    this->generalId = generalId;
    auto generalDic = _dict(LocalController::shared()->DBXMLManager()->getGroupByKey("general")->objectForKey(generalId.c_str()));
    this->color = generalDic->valueForKey("color")->intValue();
    this->level = 1;
    this->hp = generalDic->valueForKey("captain")->intValue();
    float baseAtt = generalDic->valueForKey("attack")->floatValue();
    this->attack = 1.0 * (level + GlobalData::shared()->generalConfig.up1) * baseAtt;
    float baseDefence = generalDic->valueForKey("defense")->floatValue();
    this->defense = 1.0 * (level + GlobalData::shared()->generalConfig.up2) * baseDefence;
    this->open = generalDic->valueForKey("open")->intValue();
    std::string talentStr = generalDic->valueForKey("talent")->getCString();
    std::string cost = generalDic->valueForKey("cost")->getCString();
    std::vector<std::string> costV;
    CCCommonUtils::splitString(cost,";",costV);
    if(costV.size()==2){
        this->costType = atoi(costV[0].c_str());
        this->costValue = atoi(costV[1].c_str());
    }
    defaultSkill = generalDic->valueForKey("ability")->getCString();
    GeneralManager::getInstance()->addGeneralAbility(*this, defaultSkill);
    currExp = 0;
    maxExp = 25;
    name = CCCommonUtils::getNameById(generalId);
}

void GeneralInfo::updateGeneralData(CCDictionary* dict,bool ref/* = true*/){
    if(dict==NULL){
        return;
    }
    const CCString *pStr = dict->valueForKey("maxForces");
    if (pStr->compare("")!=0) {
        maxForces = pStr->intValue();
    }
    pStr = dict->valueForKey("level");
    if (pStr->compare("")!=0) {
        level = pStr->intValue();
    }
    pStr = dict->valueForKey("exp");
    if (pStr->compare("")!=0) {
        currExp = pStr->intValue();
    }
    pStr = dict->valueForKey("maxExp");
    if (pStr->compare("")!=0) {
        maxExp = pStr->intValue();
    }
    pStr = dict->valueForKey("att");
    if (pStr->compare("")!=0) {
        attack = pStr->intValue();
    }
    pStr = dict->valueForKey("defence");
    if (pStr->compare("")!=0) {
        defense = pStr->intValue();
    }
    pStr = dict->valueForKey("status");
    if (pStr->compare("")!=0) {
        state = pStr->intValue();
    }
    pStr = dict->valueForKey("hp");
    if (pStr->compare("")!=0) {
        hp = pStr->intValue();
    }
    pStr = dict->valueForKey("color");
    if (pStr->compare("")!=0) {
        color = pStr->intValue();
    }
    pStr = dict->valueForKey("rank");
    if (pStr->compare("")!=0) {
        rank = pStr->intValue();
    }
}

GeneralSkillInfo *GeneralInfo::getSkill(std::string abilityId, std::string skillId){
    std::map<std::string, std::map<std::string, GeneralSkillInfo*>>::iterator it = generalSkillMap.find(abilityId);
    if(it != generalSkillMap.end()){
        std::map<std::string, GeneralSkillInfo*>::iterator skillIt = it->second.find(skillId);
        if(skillIt != it->second.end()){
            return skillIt->second;
        }
    }
    return NULL;
}

bool GeneralInfo::checkHaveStudy(std::string skillId){
    GeneralSkillInfo * info = NULL;
    info = this->getSkill("50000",skillId);
    if (info==NULL) {
        info = this->getSkill("50001",skillId);
    }
    if (info==NULL) {
        info = this->getSkill("50002",skillId);
    }
    if(info && info->level>=1){
        return true;
    }else{
        return false;
    }
}
string GeneralInfo::getAbilityBySkillId(std::string skillId){
    string abilityId = "50000";
    if(getSkill("50001",skillId)!=NULL){
        abilityId = "50001";
    }
    if (getSkill("50002",skillId)!=NULL) {
        abilityId = "50002";
    }
    return abilityId;
}

int GeneralInfo::getUsePoint(std::string abilityId){
    int point = 0 ;
    std::map<std::string, std::map<std::string, GeneralSkillInfo*>>::iterator it = generalSkillMap.find(abilityId);
    if(it != generalSkillMap.end()){
        std::map<std::string, GeneralSkillInfo*>::iterator skillIt;
        for (skillIt = it->second.begin(); skillIt!=it->second.end(); skillIt++) {
            point += skillIt->second->level;
        }
    }
    return point;
}

std::string GeneralInfo::getName(){
    return CCCommonUtils::getNameById(generalId);
}

std::string GeneralInfo::getDesc(){
    std::string dialogId = CCCommonUtils::getPropById(generalId, "description");
    return _lang(dialogId);
}

std::string GeneralInfo::getCostName(int type){
    std::string costName = "";
    switch (type) {
        case COST_WOOD:
            costName = _lang("108639");
            break;
        case COST_STONE:
            costName = _lang("108640");
            break;
        case COST_IRON:
            costName = _lang("108641");
            break;
        case COST_FOOD:
            costName = _lang("108642");
            break;
        case COST_MONEY:
            costName = _lang("108643");
            break;
        case COST_GOLD:
            costName = _lang("108644");
            break;
    }
    return costName;
}

GeneralInfo::~GeneralInfo(){
    ability.clear();
    abilityToChoose.clear();
    std::map<std::string, std::map<std::string, GeneralSkillInfo*>>::iterator abilityIt;
    for(abilityIt = generalSkillMap.begin(); abilityIt != generalSkillMap.end(); abilityIt++){
        std::map<std::string, GeneralSkillInfo*>::iterator skillIt;
        for(skillIt = abilityIt->second.begin(); skillIt != abilityIt->second.end(); skillIt++){
            skillIt->second->release();
        }
        abilityIt->second.clear();
    }
    generalSkillMap.clear();
}

int GeneralInfo::getAbilityIndex(std::string abilityId){
    if(abilityId == ""){
        return ability.size();
    }
    int i = 0;
    while(i < ability.size()){
        if(abilityId == ability[i]){
            return i;
        }
        i++;
    }
    return 0;
}

int GeneralInfo::getPower(){
    return 0;
}
////---------

GeneralSkillInfo *GeneralSkillInfo::create(std::string skillId, int level, std::string generalUid, std::string abilityId){
    GeneralSkillInfo *ret = new GeneralSkillInfo(skillId, level, generalUid, abilityId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GeneralSkillInfo::isLocked(){
    if(generalUid == ""){
        return true;
    }
    if(level > 0){
        return false;
    }
    std::map<std::string, GeneralInfo>::iterator generalIt = GlobalData::shared()->generals.find(generalUid);
    if(generalIt == GlobalData::shared()->generals.end()){
        return true;
    }
    std::string conditionSkill = CCCommonUtils::getPropById(skillId, "condition_skill");
    if(conditionSkill == ""){
        return false;
    }
    vector<std::string> vector1;
    vector<std::string> vector;
    CCCommonUtils::splitString(conditionSkill, "|", vector);
    int i = 0;
    while(i < vector.size()){
        vector1.clear();
        CCCommonUtils::splitString(vector[i], ";", vector1);
        int requiredLevel = atoi(vector1[1].c_str());
        std::string requiredskillId = vector1[0];
        GeneralSkillInfo *skillInfo = generalIt->second.getSkill(abilityId, requiredskillId);
        if(skillInfo!=NULL && skillInfo->level >= requiredLevel){
            return false;
        }
        i++;
    }
    return true;
}

bool GeneralSkillInfo::canConnect(std::string upSkillId){
    GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
    std::string conditionSkill = CCCommonUtils::getPropById(skillId, "condition_skill");
    if(conditionSkill == ""){
        return true;
    }
    vector<std::string> vector1;
    vector<std::string> vector;
    CCCommonUtils::splitString(conditionSkill, "|", vector);
    int i = 0;
    while(i < vector.size()){
        vector1.clear();
        CCCommonUtils::splitString(vector[i], ";", vector1);
        int requiredLevel = atoi(vector1[1].c_str());
        std::string requiredskillId = vector1[0];
        GeneralSkillInfo *skillInfo = info->getSkill("50000", requiredskillId);
        if(skillInfo==NULL){
            skillInfo = info->getSkill("50001", requiredskillId);
        }else if(skillInfo==NULL){
            skillInfo = info->getSkill("50002", requiredskillId);
        }
        if(skillInfo && skillInfo->level < requiredLevel && upSkillId==requiredskillId){
            return false;
        }
        i++;
    }
    return true;
}

bool GeneralSkillInfo::init(){
    maxLevel = atoi(CCCommonUtils::getPropById(skillId, "max_lv").c_str());
    vector<std::string> vector;
    type = atoi(CCCommonUtils::getPropById(skillId, "type").c_str());
    showType = atoi(CCCommonUtils::getPropById(skillId, "type_value").c_str());
    CCCommonUtils::splitString(CCCommonUtils::getPropById(skillId, "base"), "|", vector);
    CCLOG("basestr: %s", CCCommonUtils::getPropById(skillId, "base").c_str());
    while(vector.size() > 0){
        float baseValue = atof(vector.back().c_str());
        vector.pop_back();
        base.push_back(baseValue);
    }
    vector.clear();
    CCCommonUtils::splitString(CCCommonUtils::getPropById(skillId, "add"), "|", vector);
    while(vector.size() > 0){
        float addValue = atof(vector.back().c_str());
        vector.pop_back();
        add.push_back(addValue);
    }

    vector.clear();
    CCCommonUtils::splitString(CCCommonUtils::getPropById(skillId, "para1"), "|", vector);
    while(vector.size() > 0){
        float paraValue = atof(vector.back().c_str());
        vector.pop_back();
        param1.push_back(paraValue);
    }

    CCCommonUtils::splitString(CCCommonUtils::getPropById(skillId, "des_value"), "|", des);

    return true;
}

int GeneralSkillInfo::getTotalSkillPoint(){
    return level;
}

int GeneralSkillInfo::getUpgradeSkillPointNeed(){
    return 1;
}

SkillCDInfo *SkillCDInfo::create(){
    SkillCDInfo *ret = new SkillCDInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SkillCDInfo::init(){
    
    return true;
}

void SkillCDInfo::parse(CCDictionary* dict){
    if(dict==NULL) return ;
    if(dict->objectForKey("uuid")){
        uuid = dict->valueForKey("uuid")->getCString();
    }
    if(dict->objectForKey("ownerId")){
        ownerId = dict->valueForKey("ownerId")->getCString();
    }
    if(dict->objectForKey("skillId")){
        skillId = dict->valueForKey("skillId")->getCString();
    }
    auto tmp = dict->valueForKey("startTime")->doubleValue()/1000;
    if (tmp == 0) {
        this->startTime = 0;
    }else{
        this->startTime = GlobalData::shared()->changeTime(tmp);
    }
    tmp = dict->valueForKey("endTime")->doubleValue()/1000;
    if (tmp == 0) {
        this->endTime = 0;
    }else{
        this->endTime = GlobalData::shared()->changeTime(tmp);
    }
    if(dict->objectForKey("stat")){
        stat = dict->valueForKey("stat")->intValue();
    }
    
    //添加技能持续特效
    if(dict->objectForKey("actTime")){
        tmp = dict->valueForKey("actTime")->doubleValue()/1000;
        if (tmp == 0) {
            this->effectEndTime = 0;
        }else{
            this->effectEndTime = GlobalData::shared()->changeTime(tmp);
        }
    }
}

AttributeInfo::AttributeInfo(CCDictionary* dict)
{
    if (dict->objectForKey("id")) {
        this->aid = dict->valueForKey("id")->intValue();
        this->area = dict->valueForKey("area")->intValue();
        this->type = dict->valueForKey("type")->intValue();
        this->effect = dict->valueForKey("effect")->getCString();
        this->show = dict->valueForKey("show")->intValue();
        this->name = dict->valueForKey("name")->getCString();
        this->icon = dict->valueForKey("icon")->getCString();
        this->description = dict->valueForKey("description")->getCString();
        
        this->buff = dict->valueForKey("buff")->intValue();
        
        string where = dict->valueForKey("where")->getCString();
        if(!where.empty()){
            if(where.find("mail")!=std::string::npos){
                bMail = true;
            }
        }
        this->mail_order = dict->valueForKey("mail_order")->intValue();
        string defAtk = dict->valueForKey("attack_defend")->getCString();
        if(!defAtk.empty()){
            if(defAtk.find("attack") != std::string::npos){
                bAttack = true;
            }
            if(defAtk.find("defend") != std::string::npos){
                bDefence = true;
            }
        }
    }
}