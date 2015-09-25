//
//  TalentController.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-20.
//
//

#include "TalentController.h"
#include "TalentCommand.h"

static TalentController *_instance = NULL;

TalentController::TalentController()
{
    curEndowment = 0;
    sumEndowment = 0;
    auto allDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("endowment");
    if(allDictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(allDictInfo, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            auto info = TalentInfo(dictInfo);
            string strId = element->getStrKey();
            talentMap[atoi(strId.c_str())] = info;
        }
    }
}

TalentController::~TalentController() {}

TalentController *TalentController::getInstance(){
    if(_instance == NULL){
        _instance = new TalentController();
    }
    return _instance;
}

void TalentController::initController(CCDictionary *dict){
    auto talentArr = dynamic_cast<CCArray*>(dict->objectForKey("talent"));
    CCDictionary *item = NULL;
    for (int i=0; i<talentArr->count(); i++) {
        item = _dict(talentArr->objectAtIndex(i));
        int tid = item->valueForKey("id")->intValue();
        int lv = item->valueForKey("level")->intValue();
        if (talentMap.find(tid) != talentMap.end()) {
            talentMap[tid].level = lv;
        }
    }
    makeEndowment();
}

void TalentController::makeEndowment()
{
    sumEndowment = 0;
    int userLv = GlobalData::shared()->playerInfo.level;
    int key = 100100 + userLv;
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(key));
    sumEndowment = dict->valueForKey("endowment")->intValue();
    
    int tmpValue = 0;
    std::map<int, TalentInfo>::iterator it = talentMap.begin();
    for (; it != talentMap.end(); it++) {
        tmpValue += (it->second).level;
    }
    
    curEndowment = sumEndowment - tmpValue;
}

TalentInfo* TalentController::getInfoById(int talentId){
    map<int, TalentInfo>::iterator it = talentMap.find(talentId);
    if(it != talentMap.end()){
        return &(it->second);
    }
    return NULL;
}

void TalentController::updateTalentLevel(int talentId){
    if (talentMap[talentId].level < talentMap[talentId].maxLevel && curEndowment>0) {
        talentMap[talentId].level += 1;
        curEndowment -= 1;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TALENT_STATUS_CHANGE, CCInteger::create(talentId));
    }
}

void TalentController::saveTalentData()
{
    int idx = 0;
    string strData = "";
    std::map<int, TalentInfo>::iterator it = talentMap.begin();
    for (; it != talentMap.end(); it++) {
        if ((it->second).level > 0) {
            if (idx>0) {
                strData += "|";
            }
            idx++;
            string tmp = "";
            tmp = tmp + CC_ITOA(it->first)+";"+CC_ITOA((it->second).level);
            strData += tmp;
        }
    }
    
    TalentSaveCommand* cmd = new TalentSaveCommand(strData);
    cmd->sendAndRelease();
}

void TalentController::reSetTalentData(string toolUuid)
{
    TalentClearCommand* cmd = new TalentClearCommand(toolUuid);
    cmd->sendAndRelease();
    
    std::map<int, TalentInfo>::iterator it = talentMap.begin();
    for (; it != talentMap.end(); it++) {
        if ((it->second).level > 0) {
            (it->second).level = 0;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TALENT_STATUS_CHANGE, CCInteger::create(it->first));
        }
    }
    
    sumEndowment = 0;
    int userLv = GlobalData::shared()->playerInfo.level;
    int key = 100100 + userLv;
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(key));
    sumEndowment = dict->valueForKey("endowment")->intValue();
    curEndowment = sumEndowment;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TALENT_STATUS_CHANGE);
}

void TalentController::openTalentPanel(){
    
}

std::string getEffectStrByLevel(TalentInfo *info, int level){
    return "test";
}