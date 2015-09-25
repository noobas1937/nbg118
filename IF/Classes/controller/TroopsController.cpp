//
//  TroopsController.cpp
//  IF
//
//  Created by fubin on 14-3-6.
//
//

#include "TroopsController.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "YesNoDialog.h"
#include "TroopsCommand.h"
#include "ArmyController.h"
#include "GeneralManager.h"
#include "FunBuildController.h"

static TroopsController *_instance = NULL;

TroopsController::TroopsController()
{
    m_curInfo = "";
    m_curPrepareNum = 0;
    m_prepareLimitNum = 0;
    
    m_curSelHeroNum = 0;
    m_curLoadNum = 0;
    m_minSpeed = 0;
    
    m_autoType = 0;
    m_isFirstCancelAuto = true;
    m_isBtn = false;
    m_isStartBattle = false;
    m_isNotice = true;
}

TroopsController::~TroopsController() {}

TroopsController* TroopsController::getInstance() {
    if (!_instance) {
        _instance = new TroopsController();
    }
    return _instance;
}

void TroopsController::purgeData() {
    delete _instance;
    _instance = NULL;
}

void TroopsController::initDefenceData(CCDictionary* dict)
{
    m_defenceInfos.clear();
    m_tmpAutoHeros.clear();
    
    CCDictElement* item=NULL;
    CCDICT_FOREACH(dict,  item)
    {
        string key = item->getStrKey();
        int value = dict->valueForKey(key)->intValue();
        m_defenceInfos[key] = value;
        
        if(m_autoType==1){
            m_tmpAutoHeros[key] = 1;
        }
    }
}

bool TroopsController::saveAutoDefence(int autoType)
{
    if ( m_tmpAutoHeros.size()<=0 && autoType==1 ) { //请求自动防御，并且不知道自动防御数据，请求发送。
        TroopsAutoCommand* cmd = new TroopsAutoCommand(autoType);
        cmd->sendAndRelease();
        m_autoType = autoType;
    }
    else if (m_isFirstCancelAuto && autoType==0 && m_autoType==1){ //请求取消自动防御，并且是第一次取消，并且当前是自动防御状态，请求发送
        m_isFirstCancelAuto = false;
        TroopsAutoCommand* cmd = new TroopsAutoCommand(autoType);
        cmd->sendAndRelease();
        m_autoType = autoType;
    }
    else if (autoType==1 && m_autoType==0) { //请求自动防御，自动防御数据已经存在，并且当前是非自动，更新当前临时防御数据
        reSetAutoDefHero();
    }
    
    return true;
}

void TroopsController::retSaveAutoDefence(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        CCCommonUtils::flyText(_lang("102271"));
    }
    else {
        if (m_autoType == 1) {
            m_defenceInfos.clear();
            m_tmpAutoHeros.clear();
            auto defGen = _dict(dict->objectForKey("defense"));
            CCDictElement* gitem=NULL;
            CCDICT_FOREACH(defGen,  gitem)
            {
                string key = gitem->getStrKey();
                int value = defGen->valueForKey(key)->intValue();
                m_defenceInfos[key]=value;
                m_tmpAutoHeros[key]=value;
            }
            reSetAutoDefHero();
        }
    }
}

void TroopsController::reSetAutoDefHero()
{
    map<string, int>::iterator it;
    for (it = m_tmpDefenceInfos.begin(); it != m_tmpDefenceInfos.end(); it++) {
        if(m_tmpAutoHeros.find(it->first)==m_tmpAutoHeros.end()) {
            m_tmpDefenceInfos[it->first] = 0;
        }
        else {
            m_tmpDefenceInfos[it->first] = 1;
        }
    }
    m_curSelHeroNum = m_tmpAutoHeros.size();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_BATTLE);
}

bool TroopsController::saveDefence(int autoType, bool isBtn)
{
    m_isBtn = isBtn;
    string info = "";
    map<string, int>::iterator it;
    for (it = m_tmpDefenceInfos.begin(); it != m_tmpDefenceInfos.end(); it++) {
        if(m_tmpDefenceInfos[it->first]>0) {
            info = info + it->first + ":" + CC_ITOA(0) + "|" ;
            GlobalData::shared()->generals[it->first].state = GENERAL_PVE;
        }
        else {
            GlobalData::shared()->generals[it->first].state = FREE;
        }
    }
    if (info.length()>0) {
        info = info.substr(0, info.length()-1);
    }
    
    m_curInfo = info;
    TroopsCommand* cmd = new TroopsCommand(TROOPS_DEFENCE_COMMAND, info, autoType);
    cmd->sendAndRelease();
    m_autoType = autoType;
    return true;
}

void TroopsController::retSaveDefence(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        CCCommonUtils::flyText(_lang("102271"));
    }
    else {
        m_defenceInfos.clear();
        auto defGen = _dict(dict->objectForKey("defense"));
        CCDictElement* gitem=NULL;
        CCDICT_FOREACH(defGen,  gitem)
        {
            string key = gitem->getStrKey();
            int value = defGen->valueForKey(key)->intValue();
            m_defenceInfos[key]=value;
        }
        
        if (m_isBtn) {
            if (m_defenceInfos.size()<=0) {
                CCCommonUtils::flyText(_lang("102101"));
            }
            else {
                CCCommonUtils::flyText(_lang("102199"));
            }
        }
    }
}

bool TroopsController::isHaveDefHeroInBattle()
{
    map<string, int>::iterator it;
    for (it = m_tmpBattleInfos.begin(); it != m_tmpBattleInfos.end(); it++) {
        if (m_tmpBattleInfos[it->first] > 0 && m_defenceInfos.find(it->first)!=m_defenceInfos.end()) {
            return true;
        }
    }
    return false;
}

CCDictionary* TroopsController::saveBattle()
{
    auto dict = CCDictionary::create();
    m_saveSoldierNum = 0;
    map<string, int>::iterator it;
    for (it = m_tmpBattleInfos.begin(); it != m_tmpBattleInfos.end(); it++) {
        if (m_tmpBattleInfos[it->first] > 0) {
            dict->setObject(CCInteger::create(m_tmpBattleInfos[it->first]), it->first);
            m_saveSoldierNum += m_tmpBattleInfos[it->first];
        }
    }
    m_isStartBattle = true;
    return dict;
}

void TroopsController::reFreshDefInfo()
{
    map<int, std::string>::iterator it;
    for (it = m_generalToSend.begin(); it != m_generalToSend.end(); it++) {
        if(it->second == ""){
            continue;
        }else{
            std::string uid = it->second;
            map<int, std::string>::iterator git;
            for(git = GeneralManager::getInstance()->defenceGeneral.begin(); git != GeneralManager::getInstance()->defenceGeneral.end(); git++){
                if(git->second == uid){
                    git->second = "";
                    break;
                }
            }
        }
    }
}

void TroopsController::retSaveBattle()
{
    if (!m_isStartBattle) {
        return;
    }
    m_isStartBattle = false;
    map<int, std::string>::iterator it;
    for (it = m_generalToSend.begin(); it != m_generalToSend.end(); it++) {
        if(it->second == ""){
            continue;
        }else{
            map<std::string, GeneralInfo>::iterator generalIt = GlobalData::shared()->generals.find(it->second);
            if(generalIt != GlobalData::shared()->generals.end()){
                generalIt->second.state = COUNTRY_BATTLE;
                
            }
        }
    }
    
    std::map<string,int>::iterator sIt;

    for (sIt = m_tmpFreeSoldiers.begin(); sIt != m_tmpFreeSoldiers.end(); sIt++) {
        map<std::string, ArmyInfo>::iterator armyIt = GlobalData::shared()->armyList.find(sIt->first);
        if(armyIt != GlobalData::shared()->armyList.end()){
            armyIt->second.march += (armyIt->second.free - sIt->second);
            armyIt->second.free = sIt->second;
        }
    }
    
    reFreshDefInfo();
}

void TroopsController::pushDefenceData(CCDictionary* dict)
{
    auto defGenerals = _dict(dict->objectForKey("defense"));
    m_defenceInfos.clear();
    m_tmpAutoHeros.clear();
    CCDictElement* itemGen=NULL;
    CCDICT_FOREACH(defGenerals,  itemGen)
    {
        string key = itemGen->getStrKey();
        int value = dict->valueForKey(key)->intValue();
        m_defenceInfos[key] = value;
        m_tmpAutoHeros[key] = 1;
        if (GlobalData::shared()->generals.find(key) != GlobalData::shared()->generals.end()) {
            GlobalData::shared()->generals[key].state = GENERAL_PVE;
        }
    }
}

void TroopsController::makeCurDefenceSoldiersType()
{
    m_tmpPrepareInfos.clear();
    map<string, int>::iterator it;
    for (it = m_prepareInfos.begin(); it!=m_prepareInfos.end(); it++) {
        m_tmpPrepareInfos[it->first] = it->second;
    }
    
    for (it = m_defenceInfos.begin(); it!=m_defenceInfos.end(); it++) {
        if (it->second > 0) {
            auto& general = GlobalData::shared()->generals[it->first];
            auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(general.generalId);
            string sType = dict->valueForKey("arms")->getCString();
            if (m_tmpPrepareInfos.find(sType) == m_tmpPrepareInfos.end()) {
                m_tmpPrepareInfos[sType] = 0;
            }
        }
    }
}

int TroopsController::updateTmpDefenceHeroData(string key, int type)
{
    m_tmpDefenceInfos[key] = type;
    if (type == 0) {
        selHero(-1);
    }
    else if (type == 1) {
        selHero(1);
    }
    return type;
}

int TroopsController::updateTmpDefenceData(string key, int num, string soldierId)
{
    int oldNum = m_tmpDefenceInfos[key];
    int add = num - m_tmpDefenceInfos[key];
    if (add<0) {
        m_tmpDefenceInfos[key] += add;
        m_tmpFreeSoldiers[soldierId] -= add;
    }
    else {
        if (m_tmpFreeSoldiers[soldierId] >= add) {
            m_tmpDefenceInfos[key] += add;
            m_tmpFreeSoldiers[soldierId] -= add;
        }
        else {
            m_tmpDefenceInfos[key] += m_tmpFreeSoldiers[soldierId];
            m_tmpFreeSoldiers[soldierId] = 0;
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_SOLDIER_UPDATE);
    int newNum = m_tmpDefenceInfos[key];
    if (oldNum>0 && newNum<=0) {
        selHero(-1);
    }
    else if (oldNum<=0 && newNum>0) {
        selHero(1);
    }
    return m_tmpDefenceInfos[key];
}

int TroopsController::updateTmpBattleData(string key, int num, string soldierId)
{
    bool isIncludeGeneralEffect = m_generalToSend.size() > 0;
    int oldNum = m_tmpBattleInfos[key];
    int add = num - m_tmpBattleInfos[key];
    if (add<0) {
        m_tmpBattleInfos[key] += add;
        m_tmpFreeSoldiers[soldierId] -= add;
        makeLoadNum(soldierId, add, isIncludeGeneralEffect);
    }
    else {
        if (m_tmpFreeSoldiers[soldierId] >= add) {
            m_tmpBattleInfos[key] += add;
            m_tmpFreeSoldiers[soldierId] -= add;
            makeLoadNum(soldierId, add, isIncludeGeneralEffect);
        }
        else {
            m_tmpBattleInfos[key] += m_tmpFreeSoldiers[soldierId];
            makeLoadNum(soldierId, m_tmpFreeSoldiers[soldierId], isIncludeGeneralEffect);
            m_tmpFreeSoldiers[soldierId] = 0;
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_SOLDIER_UPDATE);
    int newNum = m_tmpBattleInfos[key];
    if (oldNum>0 && newNum<=0) {
        selHero(-1);
    }
    else if (oldNum<=0 && newNum>0) {
        selHero(1);
    }
    
    return m_tmpBattleInfos[key];
}

bool TroopsController::selHero(int num)
{
    if ((m_curSelHeroNum+num)<=m_limitHeroNum) {
        m_curSelHeroNum += num;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_BATTLE);
        return true;
    }
    else {
        return false;
    }
}

bool TroopsController::isFullHero()
{
    if (m_curSelHeroNum>=m_limitHeroNum) {
        return true;
    }
    return false;
}

void TroopsController::makeLoadNum(string soldierId, int num, bool isIncludeGeneralEffect)
{
    m_curLoadNum = 0.0f;
    for(auto it = m_tmpBattleInfos.begin(); it != m_tmpBattleInfos.end(); it++){
        std::string armyId = it->first;
        int count = it->second;
        if(GlobalData::shared()->armyList.find(armyId) != GlobalData::shared()->armyList.end()){
            m_curLoadNum +=floor(ArmyController::getInstance()->getLoad(&GlobalData::shared()->armyList[armyId], isIncludeGeneralEffect, count));  //负重单个队伍向下取整
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_BATTLE_LOAD);
//    map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(soldierId);
//    if(it != GlobalData::shared()->armyList.end()){
//        float load = ArmyController::getInstance()->getLoad(&GlobalData::shared()->armyList[soldierId], isIncludeGeneralEffect);
//        m_curLoadNum += num*load;
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_BATTLE_LOAD);
//    }
}

void TroopsController::changeArrTime()
{
    m_minSpeed = 0;
    map<string, int>::iterator it;
    for (it = m_tmpBattleInfos.begin(); it!=m_tmpBattleInfos.end();it++) {
        if (it->second > 0) {
            string sType = it->first;
            int speed = 0;
            map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(sType);
            if(it != GlobalData::shared()->armyList.end()){
                speed = it->second.speed;
            }
            if (m_minSpeed==0 || m_minSpeed>speed) {
                m_minSpeed = speed;
            }
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_TIME);
}

void TroopsController::sortByIsHaveArmy(CCArray* arr, bool reverse){
    int total = arr->count();
    int i = 0;
    int j = 0;
    while(i < total){
        string g1Id = dynamic_cast<CCString*>(arr->objectAtIndex(i))->getCString();
        auto& general1 = GlobalData::shared()->generals[g1Id];
        auto dict1 = LocalController::shared()->DBXMLManager()->getObjectByKey(general1.generalId);
        string soldierId1 = dict1->valueForKey("arms")->getCString();
        int cntNum1 = TroopsController::getInstance()->m_tmpConfSoldiers[soldierId1];
        if (cntNum1<=0) {
            j = i+1;
            bool sweepFlag = false;
            while(j < total){
                string g2Id = dynamic_cast<CCString*>(arr->objectAtIndex(j))->getCString();
                auto& general2 = GlobalData::shared()->generals[g2Id];
                auto dict2 = LocalController::shared()->DBXMLManager()->getObjectByKey(general2.generalId);
                string soldierId2 = dict2->valueForKey("arms")->getCString();
                int cntNum2 = TroopsController::getInstance()->m_tmpConfSoldiers[soldierId2];
                
                if(cntNum2>0){
                    sweepFlag = true;
                    arr->swap(i, j);
                    break;
                }
                j++;
            }
            if (!sweepFlag) {
                break;
            }
        }
        
        i++;
    }
    
    if(reverse){
        arr->reverseObjects();
    }
}

void TroopsController::resetGeneralToSend(){
    int i = 0;
    while(i < m_limitHeroNum){
        m_generalToSend[i] = "";
        ++i;
    }
}

int TroopsController::getMaxSoilder(){
//    auto &build = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    int id = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TRAINFIELD);
    int total = 0;
    if(id != 0){
        auto &build = FunBuildController::getInstance()->getFunbuildById(id);
        total = build.para1;
    }
    return (FunBuildController::getInstance()->building_base_k5 + total + CCCommonUtils::getEffectValueByNum(56)) * (1.0f + CCCommonUtils::getEffectValueByNum(COMMAND_EFFECT) / 100.0f);
}

int TroopsController::getMaxSendTime(){
    auto &build = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    return build.para2;
}