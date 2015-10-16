//
//  ArmyController.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-5.
//
//

#include "FunBuildController.h"
#include "ArmyController.h"
#include "TreatCommand.h"
#include "QueueController.h"
#include "ToolController.h"
#include "TroopsController.h"
#include "TrainSoldiertimeCommand.h"
#include "FortCommand.h"
#include "ScienceController.h"
#include "fireandcomman.h"
#include "GuideController.h"
#include "ActivityController.h"

static ArmyController *_instance = NULL;

ArmyController *ArmyController::getInstance(){
    if(_instance == NULL){
        _instance = new ArmyController();
    }
    return _instance;
}

int ArmyController::getTrainTime(int num, ArmyInfo &info){
    int time = info.time;
    map<int, FunBuildInfo>::iterator it;
    int total = 0;
    int effect = 0;
    if(info.isArmy){
        for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
            if(it->first / 1000 == FUN_BUILD_BARRACK){
                total += it->second.para2;
            }
        }
        std::string arm = CCCommonUtils::getPropById(info.armyId, "arm");
        int armType = atoi(arm.c_str());
        switch (armType) {
            case ARM_BU:
            case ARM_QIANG:
                effect = BU_PRODUCT_SPEED_EFFECT;
                break;
            case ARM_RIDE:
            case ARM_RIDE_SHE:
                effect = QI_PRODUCT_SPEED_EFFECT;
                break;
            case ARM_GONG:
            case ARM_NU:
                effect = GONG_PRODUCT_SPEED_EFFECT;
                break;
            case ARM_TOU_SHI_BING:
            case ARM_CHE:
                effect = CHE_PRODUCT_SPEED_EFFECT;
                break;
        }
        float skillEff = effect == 0? 0: CCCommonUtils::getEffectValueByNum(effect);
        int timeTmp = int((num * time / (1.0f + total / 100.0f))*1000);
        timeTmp = int(timeTmp / (1.0f + ( skillEff + CCCommonUtils::getEffectValueByNum(FORCE_PRODUCT_SPEED_EFFECT))/ 100.0f) / 1000.0f);
        return timeTmp;
    }else{
        for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
            if(it->first / 1000 == FUN_BUILD_FORT){
                total += it->second.para2;
            }
        }
        effect = FORT_PRODUCT_SPEED_EFFECT;
    }
    return (num * time / (1.0f + total / 100.0f)) / (1.0f + CCCommonUtils::getEffectValueByNum(effect) / 100.0f);
}

int ArmyController::getTrainSilver(int num, ArmyInfo &info){
    return num * info.silver;
}

int ArmyController::getTrainWood(int num, ArmyInfo &info){
    return num * info.wood;
}

int ArmyController::getTrainFood(int num, ArmyInfo &info){
    return num * info.food;
}

int ArmyController::getTrainIron(int num, ArmyInfo &info){
    return num * info.iron;
}

int ArmyController::getTrainStone(int num, ArmyInfo &info){
    return num * info.stone;
}

int ArmyController::getTreatTime(int num, TreatInfo &info){
    ArmyInfo &armyInfo = GlobalData::shared()->armyList[info.armyId];
    if(GlobalData::shared()->isCrossService){
        float factor = 1.0/(1.0*ActivityController::getInstance()->wb_info_k9);
        float baseTime = armyInfo.time * (armyInfo.healTime / 1000.0f) * 1.0 / (1.0f + CCCommonUtils::getEffectValueByNum(121) / 100.0f);
        float speedLater = baseTime*factor;
        float totalNum = pow(num * speedLater, ActivityController::getInstance()->wb_info_k10);
        return (int)totalNum;
    }else{
        return num * armyInfo.time * (armyInfo.healTime / 1000.0f) * 1.0 / (1.0f + CCCommonUtils::getEffectValueByNum(121) / 100.0f);
    }
}

int ArmyController::getTreatSilver(int num, TreatInfo &info){
    float effectValue = CCCommonUtils::getEffectValueByNum(108);
    ArmyInfo &armyInfo = GlobalData::shared()->armyList[info.armyId];
    return getTrainSilver(num, armyInfo) * (armyInfo.healSilver / 100.0*(1-effectValue/100));
}

int ArmyController::getTreatWood(int num, TreatInfo &info){
    float effectValue = CCCommonUtils::getEffectValueByNum(108);
    ArmyInfo &armyInfo = GlobalData::shared()->armyList[info.armyId];
    return getTrainWood(num, armyInfo) * (armyInfo.healRes / 100.0*(1-effectValue/100));
}

int ArmyController::getTreatFood(int num, TreatInfo &info){
    float effectValue = CCCommonUtils::getEffectValueByNum(108);
    ArmyInfo &armyInfo = GlobalData::shared()->armyList[info.armyId];
    return getTrainFood(num, armyInfo) * (armyInfo.healRes / 100.0*(1-effectValue/100));
}

int ArmyController::getTreatIron(int num, TreatInfo &info){
    float effectValue = CCCommonUtils::getEffectValueByNum(108);
    ArmyInfo &armyInfo = GlobalData::shared()->armyList[info.armyId];
    return getTrainIron(num, armyInfo) * (armyInfo.healRes / 100.0*(1-effectValue/100));
}

int ArmyController::getTreatStone(int num, TreatInfo &info){
    float effectValue = CCCommonUtils::getEffectValueByNum(108);
    ArmyInfo &armyInfo = GlobalData::shared()->armyList[info.armyId];
    return getTrainStone(num, armyInfo) * (armyInfo.healRes / 100.0*(1-effectValue/100));
}
int ArmyController::getTreatSolidierMaxNum(){
    CCArray* arr = CCArray::create();
    map<std::string, TreatInfo>::iterator it;
    for (it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++) {
        int i = 0;
        bool isFlag = false;
        string armyId =it->first;
        if(it->second.dead<=0){
            continue;
        }
        while (i<arr->count()) {
            string armyId1 = dynamic_cast<CCString*>(arr->objectAtIndex(i))->getCString();
            int level1 = atoi(armyId1.c_str()) % 100;
            int level = atoi(armyId.c_str()) % 100;
            if(level1 < level){
                arr->insertObject(CCString::create(armyId), i);
                isFlag = true;
                break;
            }
            i++;
        }
        if(!isFlag){
            arr->addObject(CCString::create(it->first));
            
        }
    }
    //根据兵种的排序规则，计算当前的资源可以治疗多少兵。
    int total= 0;
    CCObject* obj;
    string itemId = "";
    long totalFood = GlobalData::shared()->resourceInfo.lFood;
    long totalWood = GlobalData::shared()->resourceInfo.lWood;
    long totalIron = GlobalData::shared()->resourceInfo.lIron;
    long totalStone = GlobalData::shared()->resourceInfo.lStone;
    float effectValue = CCCommonUtils::getEffectValueByNum(108);
    CCARRAY_FOREACH(arr, obj){
        itemId = dynamic_cast<CCString*>(obj)->getCString();
        ArmyInfo &armyInfo = GlobalData::shared()->armyList[itemId];
        //取得一个最小值
        int minNum = 0;
        if(armyInfo.food>0){
            minNum = (totalFood/armyInfo.food)*100/(armyInfo.healRes*(1-effectValue/100.0));
        }
        if(armyInfo.wood>0){
            if(minNum>(totalWood/armyInfo.wood)*100/(armyInfo.healRes*(1-effectValue/100.0))){
                minNum = (totalWood/armyInfo.wood)*100/(armyInfo.healRes*(1-effectValue/100.0));
            }
        }
        if(armyInfo.iron>0){
            if(minNum>(totalIron/armyInfo.iron)*100/(armyInfo.healRes*(1-effectValue/100.0))){
                minNum = (totalIron/armyInfo.iron)*100/(armyInfo.healRes*(1-effectValue/100.0));
            }
        }
        if(armyInfo.stone>0){
            if(minNum>(totalStone/armyInfo.stone)*100/(armyInfo.healRes*(1-effectValue/100.0))){
                minNum = (totalStone/armyInfo.stone)*100/(armyInfo.healRes*(1-effectValue/100.0));
            }
        }
        auto &trearInfo = GlobalData::shared()->treatList[itemId];
        if(trearInfo.dead>minNum){
            trearInfo.prepareToTreat = minNum;
            total+= trearInfo.prepareToTreat;
            break;
        }else{
            trearInfo.prepareToTreat = trearInfo.dead;
            total+= trearInfo.prepareToTreat;
        }
        
        totalFood-= getTreatFood(trearInfo.prepareToTreat,trearInfo);
        totalWood-= getTreatWood(trearInfo.prepareToTreat,trearInfo);
        totalIron-= getTreatIron(trearInfo.prepareToTreat,trearInfo);
        totalStone-= getTreatStone(trearInfo.prepareToTreat,trearInfo);
        
    }
    return total;
}
int ArmyController::getTotalArmy()
{
    int ret = 0;
    map<string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.begin();
    for (; it!=GlobalData::shared()->armyList.end(); it++) {
        ret += (it->second.free + it->second.march);
    }    
    return ret;
}

int ArmyController::getTotalFree(){
    int ret = 0;
    map<string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.begin();
    for (; it!=GlobalData::shared()->armyList.end(); it++) {
        ret += it->second.free;
    }
    return ret;
}

int ArmyController::getTotalTreat()
{
    int ret = 0;
    map<std::string, TreatInfo>::iterator it;
    for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
        ret += it->second.heal;
    }
    return ret;
}

int ArmyController::getTotalDead()
{
    int ret = 0;
    map<std::string, TreatInfo>::iterator it;
    for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
        ret += it->second.dead;
    }
    return ret;
}

int ArmyController::getTotalFreeByType(int type)
{
    int bing1 = 1;
    int bing2 = 2;
    if (type == 1) {
        bing1 = 1;
        bing2 = 2;
    }
    else if (type == 2) {
        bing1 = 3;
        bing2 = 4;
    }
    else if (type == 3) {
        bing1 = 5;
        bing2 = 6;
    }
    else if (type == 4) {
        bing1 = 7;
        bing2 = 8;
    }
    
    int ret = 0;
    map<string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.begin();
    for (; it!=GlobalData::shared()->armyList.end(); it++) {
        string arm = CCCommonUtils::getPropById(it->first, "arm");
        int iarm = atoi(arm.c_str());
        if (iarm == bing1 || iarm == bing2) {
            ret += it->second.free;
        }
    }
    return ret;
}

int ArmyController::getMaxNumByType(int type, bool isOri){
    map<int, FunBuildInfo>::iterator it;
    int total = 0;
    for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
        if(it->first / 1000 == FUN_BUILD_HOSPITAL && type == TREAT_ARMY){
            total += it->second.para1;
        }else if(it->first / 1000 == FUN_BUILD_BARRACK && type == ARMY){
            total += it->second.para1; //para3
        }else if(it->first / 1000 == FUN_BUILD_WALL && type == FORT){
            total += it->second.para1;
        }
    }
    
    if (isOri) {
        return total;
    }
    
    if(type == TREAT_ARMY){
        total = (total + CCCommonUtils::getEffectValueByNum(TREAT_NUM_MAX_EFFECT_ADD)) * (1.0f + CCCommonUtils::getEffectValueByNum(TREAT_NUM_MAX_EFFECT) / 100.0f);
    }
    if(type == FORT){
        total += CCCommonUtils::getEffectValueByNum(TRAP_NUM_MAX_EFFECT);
    }
    return total;
}

void ArmyController::refreshFortFreeNum(CCDictionary *dict){
    std::string armyId = dict->valueForKey("id")->getCString();
    int free = dict->valueForKey("free")->intValue();
    map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(armyId);
    if(it != GlobalData::shared()->fortList.end()){
        it->second.free = free;
    }
}

void ArmyController::refreshArmyFreeNum(CCDictionary *dict,bool refresh){
    std::string armyId = dict->valueForKey("id")->getCString();
    int free = dict->valueForKey("free")->intValue();
    map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(armyId);
    if(it != GlobalData::shared()->armyList.end()){
        it->second.free = free;
        if(dict->objectForKey("march")){
            int march = dict->valueForKey("march")->intValue();
            it->second.march = march;
        }
    }
    if (refresh) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ARMY_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
    }
}

void ArmyController::refreshTreatDeadNum(CCDictionary *dict){
    std::string armyId = dict->valueForKey("id")->getCString();
    int dead = dict->valueForKey("dead")->intValue();
    map<std::string, TreatInfo>::iterator it = GlobalData::shared()->treatList.find(armyId);
    if(it != GlobalData::shared()->treatList.end()){
        it->second.dead = dead;
    }else{
        std::string itemId = dict->valueForKey("id")->getCString();
        GlobalData::shared()->treatList[itemId] = TreatInfo(dict);
    }
}

void ArmyController::refreshTreat(CCDictionary *dict){
    std::string armyId = dict->valueForKey("id")->getCString();
    int dead = dict->valueForKey("dead")->intValue();
    map<std::string, TreatInfo>::iterator it = GlobalData::shared()->treatList.find(armyId);
    if(it != GlobalData::shared()->treatList.end()){
        it->second.dead = dead;
        it->second.heal = 0;
    }
}

void ArmyController::setTreatFinishTime(time_t time){
    treatFinishTime = time;
    if(treatFinishTime != 0){
        treatFinishTime = GlobalData::shared()->changeTime(treatFinishTime);
    }
}

void ArmyController::setTreatEndTime(double time){
    if(time == 0){
        map<std::string, TreatInfo>::iterator it;
        for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
            GlobalData::shared()->armyList[it->first].free += it->second.heal;
            it->second.heal = 0;
        }
        CCCommonUtils::flyHint("hospital_icon.png", _lang("103677"), _lang("103678"));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ARMY_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GO_TO_HEALTHING);
    }
    treatFinishTime = time;
}

void ArmyController::cancelTreate()
{
    treatFinishTime = 0;
    map<std::string, TreatInfo>::iterator it;
    for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
        it->second.dead += it->second.heal;
        it->second.heal = 0;
    }
}

bool ArmyController::costCD(int itemId, string toolUUID){
    if(toolUUID != "") {
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(toolUUID.c_str()));
        if (toolInfo.getCNT() <= 0) {
            CCCommonUtils::flyText(_lang("102198"));
            return false;
        }
        int tCnt = toolInfo.getCNT() - 1;
        toolInfo.setCNT(tCnt);
    }
        
    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(itemId));
    if (qid > 0) {
        QueueController::getInstance()->startCCDQueue(qid, toolUUID,false,0,"",1);
    }
        
    return true;

}

int ArmyController::getTotalUpKeep(){
    map<std::string, ArmyInfo>::iterator it;
    float total = 0;
    for(it = GlobalData::shared()->armyList.begin(); it != GlobalData::shared()->armyList.end(); it++){
        total += it->second.upkeep * (it->second.free + it->second.march);
    }
    return ceil(total * MAX(0, 1.0f - CCCommonUtils::getEffectValueByNum(64) / 100.0f));
}

int ArmyController::getUpKeep(ArmyInfo *info){
    float total = info->upkeep * (info->free + info->march);
    return ceil(total * MAX(0, 1.0f - CCCommonUtils::getEffectValueByNum(64) / 100.0f));
}

int ArmyController::getUpKeep(ArmyInfo *info, int num){
    float total = info->upkeep * num;
    return ceil(total * MAX(0, 1.0f - CCCommonUtils::getEffectValueByNum(64) / 100.0f));
}

void ArmyController::startFinishForce(std::string armyId, std::string uid,int buildingKey){
    GlobalData::shared()->armyList[armyId].finishTime = 0;
    TrainSoldiertimeCommand *cmd = new TrainSoldiertimeCommand(armyId, uid,buildingKey);
    if (GuideController::share()->getCurGuideID() == GUIDE_TROOP_GET) {
        cmd->putParam("guideStep", CCString::create(GUIDE_TROOP_GET));
    }
    cmd->sendAndRelease();
}
int ArmyController::getTotalFortNum(){
    int total = 0;
    map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.begin();
    while (it != GlobalData::shared()->fortList.end()){
        total += it->second.free ;
        ++it;
    }
    return total;
}
void ArmyController::startFinishFort(std::string armyId, std::string uid){
    GlobalData::shared()->fortList[armyId].finishTime = 0;
    FortEndCommand *cmd = new FortEndCommand(armyId, uid);
    cmd->sendAndRelease();
    
}

void ArmyController::startFinishHospital(std::string uid){
    TreatEndCommand *cmd = new TreatEndCommand(uid);
    cmd->sendAndRelease();
    treatFinishTime = 0;
}

float ArmyController::getLoad(ArmyInfo *info, bool isIncludeGeneralEffect, int num){
    return num * info->load * (1.0 + CCCommonUtils::getEffectValueByNum(ARMY_LOAD_EFFECT, isIncludeGeneralEffect) / 100.0);
}

vector<int> ArmyController::getSoldierSqerNum()
{
    // guo.jiang
//    int qiNum = 3600;//ArmyController::getInstance()->getTotalFreeByType(2);
//    int buNum = 3600;//ArmyController::getInstance()->getTotalFreeByType(1);
//    int gongNum = 3600;//ArmyController::getInstance()->getTotalFreeByType(3);
//    int cheNum = 3600;//ArmyController::getInstance()->getTotalFreeByType(4);
    
    int qiNum = ArmyController::getInstance()->getTotalFreeByType(2);
    int buNum = ArmyController::getInstance()->getTotalFreeByType(1);
    int gongNum = ArmyController::getInstance()->getTotalFreeByType(3);
    int cheNum = ArmyController::getInstance()->getTotalFreeByType(4);
    
    int sqerNum = 30;//方阵总个数
    int numPerSqer = 3600/10;//每个方阵士兵数
    int c1 = buNum/numPerSqer + 1;
    int c2 = qiNum/numPerSqer + 1;
    int c3 = gongNum/numPerSqer + 1;
    int c4 = cheNum/numPerSqer + 1;
    int cCnt = c1+c2+c3+c4;
    if (cCnt>sqerNum) {
        float d1 = sqerNum*1.0*c1/cCnt;
        float d2 = sqerNum*1.0*c2/cCnt;
        float d3 = sqerNum*1.0*c3/cCnt;
        float d4 = sqerNum*1.0*c4/cCnt;
        if (d1>=d2 && d1>=d3 && d1>=d4 ) {
            c2 = (ceil(d2)-d2)>=0.5 ? ceil(d2) : floor(d2);
            c3 = (ceil(d3)-d3)>=0.5 ? ceil(d3) : floor(d3);
            c4 = (ceil(d4)-d4)>=0.5 ? ceil(d4) : floor(d4);
            c1 = sqerNum - c2 - c3 - c4;
        }
        else if (d2>=d1 && d2>=d3 && d2>=d4 ) {
            c1 = (ceil(d1)-d1)>=0.5 ? ceil(d1) : floor(d1);
            c3 = (ceil(d3)-d3)>=0.5 ? ceil(d3) : floor(d3);
            c4 = (ceil(d4)-d4)>=0.5 ? ceil(d4) : floor(d4);
            c2 = sqerNum - c1 - c3 - c4;
        }
        else if (d3>=d2 && d3>=d1 && d3>=d4 ) {
            c2 = (ceil(d2)-d2)>=0.5 ? ceil(d2) : floor(d2);
            c1 = (ceil(d1)-d1)>=0.5 ? ceil(d1) : floor(d1);
            c4 = (ceil(d4)-d4)>=0.5 ? ceil(d4) : floor(d4);
            c3 = sqerNum - c2 - c1 - c4;
        }
        else if (d4>=d2 && d4>=d3 && d4>=d1 ) {
            c2 = (ceil(d2)-d2)>=0.5 ? ceil(d2) : floor(d2);
            c3 = (ceil(d3)-d3)>=0.5 ? ceil(d3) : floor(d3);
            c1 = (ceil(d1)-d1)>=0.5 ? ceil(d1) : floor(d1);
            c4 = sqerNum - c2 - c3 - c1;
        }
    }
    
    vector<int> ret;
    ret.push_back(c2);
    ret.push_back(c1);
    ret.push_back(c3);
    ret.push_back(c4);
    ret.push_back(qiNum);
    ret.push_back(buNum);
    ret.push_back(gongNum);
    ret.push_back(cheNum);
    return ret;
}

static bool IsFort = false;
bool comperUnLockLv(string s1, string s2)
{
    ArmyInfo* info1 = NULL;
    ArmyInfo* info2 = NULL;
    if (IsFort) {
        info1 = &(GlobalData::shared()->fortList[s1]);
        info2 = &(GlobalData::shared()->fortList[s2]);
    }else {
        info1 = &(GlobalData::shared()->armyList[s1]);
        info2 = &(GlobalData::shared()->armyList[s2]);
    }
    
    if (info1->unlockLevel > info2->unlockLevel) {
        return false;
    }else {
        return true;
    }
}

vector<string> ArmyController::getCreateSoldierIds(string sildiers, bool isFort)
{
    IsFort = isFort;
    
    vector<string> ret;
    std::vector<std::string> armys;
    CCCommonUtils::splitString(sildiers,"|",armys);
    int size = armys.size();
    std::vector<std::string> armyVector;
    if(isFort){
        for (int i=0; i<size; i++) {
            armyVector.clear();
            CCCommonUtils::splitString(armys[i],";",armyVector);
            int level = atoi(armyVector[0].c_str());
            map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(armyVector[1]);
            if(it!=GlobalData::shared()->fortList.end()){
                it->second.unlockLevel = level;
                ret.push_back(it->first);
            }
        }
    }else{
        for (int i=0; i<size; i++) {
            armyVector.clear();
            CCCommonUtils::splitString(armys[i],";",armyVector);
            int level = atoi(armyVector[0].c_str());
            map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(armyVector[1]);
            if(it!=GlobalData::shared()->armyList.end()){
                it->second.unlockLevel = level;
                ret.push_back(it->first);
            }
        }
    }
    
    sort(ret.begin(), ret.end(), comperUnLockLv);
    
    return ret;
}