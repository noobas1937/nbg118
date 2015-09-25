//
//  ScienceController.cpp
//  IF
//
//  Created by fubin on 14-3-19.
//
//

#include "ScienceController.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "YesNoDialog.h"
#include "QueueController.h"
#include "ScienceCommand.h"
#include "ToolController.h"
#include "AllianceScienceCommand.h"
#include "FunBuildController.h"

static ScienceController *_instance = NULL;

ScienceController::ScienceController()
{
    m_callBackFunc = NULL;
    auto allDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("scienceType");
    if(allDictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(allDictInfo, element)
        {
            string strId = element->getStrKey();
            int id = atoi(strId.c_str());
            m_scienceType.push_back(id);
        }
        sort(m_scienceType.begin(), m_scienceType.end());
    }
    
    auto allianceDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("alliancescience");
    if(allianceDictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(allianceDictInfo, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            int hide = dictInfo->valueForKey("hide")->intValue();
            if (hide == 0) {
                AllianceScienceInfo info = AllianceScienceInfo(dictInfo);
                allianceScienceMap[info.scienceId] = info;
                aScienceFloorMap[info.floor].push_back(info.scienceId);
            }
        }
    }
}

ScienceController::~ScienceController() {}

ScienceController* ScienceController::getInstance() {
    if (!_instance) {
        _instance = new ScienceController();
    }
    return _instance;
}

void ScienceController::purgeData() {
    delete _instance;
    _instance = NULL;
}

bool ScienceController::costCD(int itemId, string toolUUID){
    if(toolUUID != "") {
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(toolUUID.c_str()));
        if (toolInfo.getCNT() <= 0) {
            CCCommonUtils::flyText(_lang("102198"));
            return false;
        }
        int toolCNT = toolInfo.getCNT() - 1;
        toolInfo.setCNT(toolCNT);
    }
    
    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(itemId));
    if (qid > 0) {
        QueueController::getInstance()->startCCDQueue(qid, toolUUID,false, 0,"",4);
    }
    
    return true;
}

bool ScienceController::startResearchScience(int scienceid, int gold)
{
    ScienceResearchCommand* cmd = new ScienceResearchCommand(CC_ITOA(scienceid), gold);
    cmd->sendAndRelease();
    return true;
}

void ScienceController::retResearchScience(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else {
        
//        (sfs_object) queue:
//            (sfs_object) itemObj:
//               (utf_string) itemId: 220000
//            
//            (int) qid: 1
//            (int) type: 6
//            (long) startTime: 1395833723905
//            (long) updateTime: 1395835523879
//            (long) endTime: 0
//            (utf_string) uuid: 099dedad8a42432b9da07ee4291b0323
//		
//		(sfs_object) resource:
//            (int) food: 88278
//            (int) silver: 100000
//            (int) iron: 89424
//            (int) wood: 88646
//            (int) stone: 88059
        
        auto queue = _dict(dict->objectForKey("queue"));
        auto obj = _dict(queue->objectForKey("itemObj"));
        if (obj->objectForKey("itemId")) {
            int itemId = obj->valueForKey("itemId")->intValue();
            int tmp = queue->valueForKey("updateTime")->doubleValue()/1000;
            if (tmp > 0) {
               tmp = GlobalData::shared()->changeTime(tmp);
            }
            GlobalData::shared()->scienceInfo.infoMap[itemId].finishTime = tmp;
        }
        QueueController::getInstance()->addQueueInfo(queue);
        
        if (dict->objectForKey("gold")) {
            int tmpInt = dict->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        
        auto resDict = _dict(dict->objectForKey("resource"));
        GlobalData::shared()->resourceInfo.setResourceData(resDict);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_CELL_STATE);
    }
}

bool ScienceController::startDirResearchScience(int scienceid)
{
    ScienceDirectlyCommand* cmd = new ScienceDirectlyCommand(CC_ITOA(scienceid));
    cmd->sendAndRelease();
    return true;
}

void ScienceController::retDirResearchScience(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else {
        if (dict->objectForKey("itemId")) {
            int itemId = dict->valueForKey("itemId")->intValue();
            auto& info = GlobalData::shared()->scienceInfo.infoMap[itemId];
            
            vector<string> curPara;
            int oldValue = 0;
            CCCommonUtils::splitString(info.para2, "|", curPara);
            if (curPara.size()>0) {
                oldValue = atoi(curPara[0].c_str());
            }
            
            if (dict->objectForKey("level")) {
                info.scienceLv = dict->valueForKey("level")->intValue();
            }
            if (dict->objectForKey("time_research")) {
                info.time_need = dict->valueForKey("time_research")->intValue();
            }
            if (dict->objectForKey("para1")) {
                info.para1 = dict->valueForKey("para1")->getCString();
            }
            if (dict->objectForKey("para2")) {
                info.para2 = dict->valueForKey("para2")->getCString();
            }
            if (dict->objectForKey("nextPara2")) {
                info.nextPara2 = dict->valueForKey("nextPara2")->getCString();
            }
            if (dict->objectForKey("research_need")) {
                info.resCondition = dict->valueForKey("research_need")->getCString();
            }
            if (dict->objectForKey("building_condition")) {
                info.buildCondition = dict->valueForKey("building_condition")->getCString();
            }
            if (dict->objectForKey("max_lv")) {
                info.scienceMaxLv = dict->valueForKey("max_lv")->intValue();
            }
            info.finishTime = 0;
            
            if(dict->objectForKey("resource")) {
                auto resDict = _dict(dict->objectForKey("resource"));
                GlobalData::shared()->resourceInfo.lFood = resDict->valueForKey("food")->intValue();
                GlobalData::shared()->resourceInfo.lMoney = resDict->valueForKey("silver")->intValue();
                GlobalData::shared()->resourceInfo.lIron = resDict->valueForKey("iron")->intValue();
                GlobalData::shared()->resourceInfo.lWood = resDict->valueForKey("wood")->intValue();
                GlobalData::shared()->resourceInfo.lStone = resDict->valueForKey("stone")->intValue();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            }
            
            if (dict->objectForKey("gold")) {
                int tmpInt = dict->valueForKey("gold")->intValue();
                UIComponent::getInstance()->updateGold(tmpInt);
            }
            
            if (dict->objectForKey("sciencePower")) {
                int sciencePower = dict->valueForKey("sciencePower")->intValue();
                updateSciencePower(sciencePower);
            }
            
            if(info.para1 != "trap" && info.para1 != "army") {
//                int key = atoi(info.para1.c_str());
//                if (key > 0) {
//                    GlobalData::shared()->effectValues[key] = atoi(info.para2.c_str());
//                }
                
                vector<string> effectNum;
                CCCommonUtils::splitString(info.para1, "|", effectNum);
                vector<string> effectValue;
                CCCommonUtils::splitString(info.para2, "|", effectValue);
                for (int i=0; i<effectNum.size(); i++) {
                    int eNum = atoi(effectNum[i].c_str());
                    if (effectValue.size()>0) {
                        int eVal = atoi(effectValue[0].c_str());
                        if (eNum > 0) {
                            if (GlobalData::shared()->effectValues.find(eNum) == GlobalData::shared()->effectValues.end()) {
                                GlobalData::shared()->effectValues[eNum] = 0;
                            }
                            GlobalData::shared()->effectValues[eNum] += (eVal-oldValue);
                            
                            if(eNum == 125){
                                unsigned int cityDefMax = GlobalData::shared()->imperialInfo[FUN_BUILD_WALL_ID].para2 + GlobalData::shared()->effectValues[125];
                                unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal + (eVal-oldValue);
                                if(cityDefFix > cityDefMax){
                                    cityDefFix = cityDefMax;
                                }
                                GlobalData::shared()->cityDefenseVal = cityDefFix;
                            }
                        }
                    }
                }
            }
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_RESEARCH_FINISH, CCInteger::create(itemId));
            CCCommonUtils::flyHint("science_icon.png", _lang("121984"), _lang_2("121985", CCCommonUtils::getNameById(CC_ITOA(info.scienceId)).c_str(),CC_ITOA(info.scienceLv)), 3, 0, true);
        }
    }
}

bool ScienceController::startFinishScience(int scienceid, string qUUid)
{
    ScienceResearchFinishCommand* cmd = new ScienceResearchFinishCommand(CC_ITOA(scienceid), qUUid);
    cmd->sendAndRelease();
    return true;
}

int ScienceController::getScienceLevel(int scienceId){
    map<int, ScienceItemInfo>::iterator it = GlobalData::shared()->scienceInfo.infoMap.find(scienceId);
    if(it == GlobalData::shared()->scienceInfo.infoMap.end()){
        return 0;
    }
    return it->second.scienceLv;
}

void ScienceController::retFinishScience(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        //
    }
    else {
//        (utf_string) itemId: 220000
//        (int) level: 1
//        (utf_string) time_research: 3600
//        (utf_string) para1: 101
//        (utf_string) research_need: 2
//        8760
//        (utf_string) nextPara2: 20
//        (utf_string) building_condition: 400000
//        (utf_string) max_lv: 10
//        (utf_string) para2: 10
        if (dict->objectForKey("itemId")) {
            int itemId = dict->valueForKey("itemId")->intValue();
            auto& info = GlobalData::shared()->scienceInfo.infoMap[itemId];
            
            vector<string> curPara;
            int oldValue = 0;
            CCCommonUtils::splitString(info.para2, "|", curPara);
            if (curPara.size()>0) {
                oldValue = atoi(curPara[0].c_str());
            }
            
            if (dict->objectForKey("level")) {
                info.scienceLv = dict->valueForKey("level")->intValue();
            }
            if (dict->objectForKey("time_research")) {
                info.time_need = dict->valueForKey("time_research")->intValue();
            }
            if (dict->objectForKey("para1")) {
                info.para1 = dict->valueForKey("para1")->getCString();
            }
            if (dict->objectForKey("para2")) {
                info.para2 = dict->valueForKey("para2")->getCString();
            }
            if (dict->objectForKey("nextPara2")) {
                info.nextPara2 = dict->valueForKey("nextPara2")->getCString();
            }
            if (dict->objectForKey("research_need")) {
                info.resCondition = dict->valueForKey("research_need")->getCString();
            }
            if (dict->objectForKey("building_condition")) {
                info.buildCondition = dict->valueForKey("building_condition")->getCString();
            }
            if (dict->objectForKey("max_lv")) {
                info.scienceMaxLv = dict->valueForKey("max_lv")->intValue();
            }
            info.finishTime = 0;
            
            if (dict->objectForKey("sciencePower")) {
                int sciencePower = dict->valueForKey("sciencePower")->intValue();
                updateSciencePower(sciencePower);
            }
            
            if(info.para1 != "trap" && info.para1 != "army") {
//                int key = atoi(info.para1.c_str());
//                if (key > 0) {
//                    GlobalData::shared()->effectValues[key] = atoi(info.para2.c_str());
//                }
                
                vector<string> effectNum;
                CCCommonUtils::splitString(info.para1, "|", effectNum);
                vector<string> effectValue;
                CCCommonUtils::splitString(info.para2, "|", effectValue);
                for (int i=0; i<effectNum.size(); i++) {
                    int eNum = atoi(effectNum[i].c_str());
                    if (effectValue.size()>0) {
                        int eVal = atoi(effectValue[0].c_str());
                        if (eNum > 0) {
                            if (GlobalData::shared()->effectValues.find(eNum) == GlobalData::shared()->effectValues.end()) {
                                GlobalData::shared()->effectValues[eNum] = 0;
                            }
                            GlobalData::shared()->effectValues[eNum] += (eVal-oldValue);
                            
                            if(eNum == 125){
                                unsigned int cityDefMax = GlobalData::shared()->imperialInfo[FUN_BUILD_WALL_ID].para2 + GlobalData::shared()->effectValues[125];
                                unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal + (eVal-oldValue);
                                if(cityDefFix > cityDefMax){
                                    cityDefFix = cityDefMax;
                                }
                                GlobalData::shared()->cityDefenseVal = cityDefFix;
                            }
                        }
                    }
                }
            }
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_RESEARCH_FINISH, CCInteger::create(itemId));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_CELL_STATE);
            CCCommonUtils::flyHint("science_icon.png", _lang("121984"), _lang_2("121985", CCCommonUtils::getNameById(CC_ITOA(info.scienceId)).c_str(),CC_ITOA(info.scienceLv)), 3, 0, true);
        }
    }
}

void ScienceController::updateSciencePower(int power)
{
    if(power>0) {
        GlobalData::shared()->playerInfo.sciencePower = power;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_COLLECT_SOLDIER_ADD_POWER);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_POWER_ADD);
    }
}

void ScienceController::initAllianceScienceData(CCArray* array)
{
    if (array) {
        for (int i=0; i<array->count(); i++) {
            CCDictionary* dict = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
            if (dict) {
                int sid = dict->valueForKey("scienceId")->intValue();
                if (allianceScienceMap.find(sid) != allianceScienceMap.end()) {
                    allianceScienceMap[sid].SetInfoFromServer(dict);
                    vector<string> effVec;
                    CCCommonUtils::splitString(allianceScienceMap[sid].para1, ";", effVec);
                    float value = atof(allianceScienceMap[sid].para2.c_str());
                    for (int i=0; i<effVec.size(); i++) {
                        int effNum = atoi(effVec[i].c_str());
                        alScEffMap[effNum] += value;
                    }
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("msg_update_all_sc_da");
    }
}

int ScienceController::getTotalAllScPoints()
{
    int ret = 0;
    map<int, AllianceScienceInfo>::iterator it;
    for (it=allianceScienceMap.begin(); it!=allianceScienceMap.end(); it++) {
        ret += it->second.level;
    }
    return ret;
}

bool ScienceController::startResearchAllSc(int sid)
{
    if (findResearchingAlSc() > 0) {
        CCCommonUtils::flyHint("", "", _lang_1("115807",""));
        return false;
    }
    AllScienceResearchCommand* cmd = new AllScienceResearchCommand(sid);
    cmd->sendAndRelease();
    return true;
}

void ScienceController::endResearchAccSc(CCDictionary* dict)
{
//    int sid = dict->valueForKey("scienceId")->intValue();
//    if (allianceScienceMap.find(sid) != allianceScienceMap.end()) {
//        allianceScienceMap[sid].SetInfoFromServer(dict, true);
//    }
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_DONATE);
}

int ScienceController::findResearchingAlSc()
{
    int curTime = GlobalData::shared()->getWorldTime();
    map<int, AllianceScienceInfo>::iterator it;
    for (it=allianceScienceMap.begin(); it!=allianceScienceMap.end(); it++) {
        if (it->second.finishTime > curTime) {
            return it->first;
        }
    }
    return 0;
}

bool ScienceController::startDonate(int sid, int type)
{
    GameController::getInstance()->showWaitInterface();
    AllScienceDonateCommand * cmd = new AllScienceDonateCommand(sid, type);
    cmd->sendAndRelease();
    return true;
}

void ScienceController::endDonate(CCDictionary* dict)
{
    GameController::getInstance()->removeWaitInterface();
    //清除所有捐赠信息
    map<int,AllianceScienceInfo>::iterator it = allianceScienceMap.begin();
    for (; it != allianceScienceMap.end() ; it++) {
        it->second.lowRes = -1;
        it->second.lowContribute = 0;
        it->second.lowProgress = 0;
        it->second.lowDonateNum = 0;
        
        it->second.midRes = -1;
        it->second.midContribute = 0;
        it->second.midProgress = 0;
        it->second.midDonateNum = 0;
        
        it->second.highRes = -1;
        it->second.highContribute = 0;
        it->second.highProgress = 0;
        it->second.highDonateNum = 0;
    }
    
    if (dict->objectForKey("point")) {
        int tmpInt = dict->valueForKey("point")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.point = tmpInt;
       // CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_AL_SCIENCE_POINT);
    }
    if (dict->objectForKey("accPoint")) {
        int accPoint = dict->valueForKey("accPoint")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.accPoint = accPoint;
         CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_AL_SCIENCE_POINT);
    }
    if (dict->objectForKey("donateAvailable")) {
        int tmpInt = dict->valueForKey("donateAvailable")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.donateAvailable = tmpInt;
    }
    if (dict->objectForKey("donateCDTime")) {
        int tmpInt = dict->valueForKey("donateCDTime")->doubleValue()/1000;
        if (tmpInt>0) {
            GlobalData::shared()->playerInfo.allianceInfo.donateCDTime = GlobalData::shared()->changeTime(tmpInt);
        }
    }
    
    if (dict->objectForKey("gold")) {
        int tmpInt = dict->valueForKey("gold")->intValue();
        UIComponent::getInstance()->updateGold(tmpInt);
    }
    if (dict->objectForKey("resource")) {
        auto resDic = dynamic_cast<CCDictionary*>(dict->objectForKey("resource"));
        GlobalData::shared()->resourceInfo.setResourceData(resDic);
    }
    
    int sid = dict->valueForKey("scienceId")->intValue();
    if (allianceScienceMap.find(sid) != allianceScienceMap.end()) {
        if(dict->objectForKey("options")) {
            auto optDic = dynamic_cast<CCDictionary*>(dict->objectForKey("options"));
            allianceScienceMap[sid].SetInfoFromServer(optDic, true);
        }
        allianceScienceMap[sid].UpdatePushInfo(dict);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_DONATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_AL_SCIENCE_PUSH, CCInteger::create(sid));
}

bool ScienceController::startDonateRefresh(int sid)
{
    AllScienceDonateRefreshCommand * cmd = new AllScienceDonateRefreshCommand(sid);
    cmd->sendAndRelease();
    return true;
}

void ScienceController::endDonateRefresh(CCDictionary* dict)
{
    int sid = dict->valueForKey("scienceId")->intValue();
    if (allianceScienceMap.find(sid) != allianceScienceMap.end()) {
        allianceScienceMap[sid].SetInfoFromServer(dict, true);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_DONATE);
}

void ScienceController::pushUpdateAllInfo(CCDictionary* dict)
{
    int sid = dict->valueForKey("scienceId")->intValue();
    if (allianceScienceMap.find(sid) != allianceScienceMap.end()) {
        allianceScienceMap[sid].UpdatePushInfo(dict);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_AL_SCIENCE_PUSH, CCInteger::create(sid));
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_DONATE);
}

void ScienceController::onQuitAlliance()
{
    alScEffMap.clear();
    map<int,AllianceScienceInfo>::iterator it = allianceScienceMap.begin();
    for (; it != allianceScienceMap.end() ; it++) {
        it->second.lowRes = -1;
        it->second.lowContribute = 0;
        it->second.lowProgress = 0;
        it->second.lowDonateNum = 0;
        
        it->second.midRes = -1;
        it->second.midContribute = 0;
        it->second.midProgress = 0;
        it->second.midDonateNum = 0;
        
        it->second.highRes = -1;
        it->second.highContribute = 0;
        it->second.highProgress = 0;
        it->second.highDonateNum = 0;
        
        it->second.level = 0;
        it->second.currentPro = 0;
        it->second.needPro = 0;
        it->second.curStar = 0;
        it->second.researchTime = 0;
        it->second.finishTime = 0;
        
        it->second.para1 = "";
        it->second.para2 = "";
        it->second.nextPara2 = "";
    }
}

bool ScienceController::costCDDonate()
{
    AllScienceDonateCDCommand * cmd = new AllScienceDonateCDCommand();
    cmd->sendAndRelease();
    return true;
}

void ScienceController::endCDDonate(CCDictionary* dict)
{
    if (dict) {
        if (dict->objectForKey("gold")) {
            int tmpInt = dict->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        GlobalData::shared()->playerInfo.allianceInfo.donateCDTime = 0;
        if (dict->objectForKey("donateCDTime")) {
            int tmpInt = dict->valueForKey("donateCDTime")->doubleValue()/1000;
            if (tmpInt>0) {
                GlobalData::shared()->playerInfo.allianceInfo.donateCDTime = GlobalData::shared()->changeTime(tmpInt);
            }
        }
        GlobalData::shared()->playerInfo.allianceInfo.donateAvailable = 1;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_DONATE);
}