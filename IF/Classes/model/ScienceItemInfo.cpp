//
//  ScienceItemInfo.cpp
//  IF
//
//  Created by 邹 程 on 13-11-4.
//
//

#include "ScienceItemInfo.h"
#include "GlobalData.h"
#include "GameController.h"
#include "ScienceInfo.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"

ScienceItemInfo::ScienceItemInfo(CCDictionary* dict) {
//    <ItemSpec id="220000" tab="1" name="107100" description="107200" icon="ico220000" position="1;2" relation="2;1|2;3"/>
    scienceId = dict->valueForKey("id")->intValue();
    scienceType = dict->valueForKey("tab")->intValue();
    show = dict->valueForKey("show")->intValue();
    
    string pos = dict->valueForKey("position")->getCString();
    std::vector<string> posList;
    CCCommonUtils::splitString(pos, ";", posList);
    posX = atoi(posList[1].c_str());
    posY = atoi(posList[0].c_str());
    
    relation = dict->valueForKey("relation")->getCString();
}

void ScienceItemInfo::initItemInfo(cocos2d::CCDictionary *dict) {
    if (dict->objectForKey("level")) {
        this->scienceLv = dict->valueForKey("level")->intValue() <= 0 ? 0 :dict->valueForKey("level")->intValue();
    }
    if (dict->objectForKey("max_lv")) {
        this->scienceMaxLv = dict->valueForKey("max_lv")->intValue();
    }
    if (dict->objectForKey("time_research")) {
        this->time_need = dict->valueForKey("time_research")->intValue();
    }
    if (dict->objectForKey("research_need")) {
        this->resCondition = dict->valueForKey("research_need")->getCString();
    }
    if (dict->objectForKey("building_condition")) {
        this->buildCondition = dict->valueForKey("building_condition")->getCString();
    }
    if (dict->objectForKey("para1")) {
        this->para1 = dict->valueForKey("para1")->getCString();
    }
    if (dict->objectForKey("para2")) {
        this->para2 = dict->valueForKey("para2")->getCString();
    }
    if (dict->objectForKey("nextPara2")) {
        this->nextPara2 = dict->valueForKey("nextPara2")->getCString();
    }
    if (dict->objectForKey("science_condition")) {
        scienceCondition = dict->valueForKey("science_condition")->getCString();
    }
    
    if (dict->objectForKey("cd")) {
        finishTime = dict->valueForKey("cd")->doubleValue()/1000;
        if (finishTime>0) {
            finishTime = GlobalData::shared()->changeTime(finishTime);
        }
//        GlobalData::shared()->scienceInfo.currentResearchId = scienceId;
//        auto timeStamp = TimeStamp();
//        timeStamp.type = Stamp_SCIENCE_RESEARCH;
//        timeStamp.finishTime = GlobalData::shared()->changeTime(dict->valueForKey("cd")->intValue()/1000);
//        GlobalData::shared()->timeStampMap[scienceId] = timeStamp;
//        GameController::getInstance()->addTimeObserver(Stamp_SCIENCE_RESEARCH, &(GlobalData::shared()->scienceInfo), NULL,timeFinish_selector(ScienceInfo::doResearchFinish));
    }
}

map<int,int> ScienceItemInfo::getInvest() {
    vector<string> tmp;
    map<int,int> ret;
    return ret;
}

vector<int> ScienceItemInfo::getPositionIndex() {
    string tmp = CCCommonUtils::getPropById(CC_ITOA(scienceId), "position");
    vector<string> tmp2;
    CCCommonUtils::splitString(tmp, ";", tmp2);
    vector<int> ret;
    ret.push_back(atoi(tmp2[1].c_str()));
    ret.push_back(atoi(tmp2[0].c_str()));
    return ret;
}

string ScienceItemInfo::getName() {
    return _lang(CCCommonUtils::getPropById(CC_ITOA(scienceId), "name"));
}

string ScienceItemInfo::getIcon() {
    return (CCCommonUtils::getPropById(CC_ITOA(scienceId), "icon")+".png");
}

string ScienceItemInfo::getLine() {
    if (!LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(scienceId))->objectForKey("relation")) {
        return string();
    }
    return (CCCommonUtils::getPropById(CC_ITOA(scienceId), "relation")+".png");
}

string ScienceItemInfo::getDescription() {
    return _lang(CCCommonUtils::getPropById(CC_ITOA(scienceId), "description"));
}


int ScienceItemInfo::getCondition() {
    int ret = 0;
    if (scienceCondition!="") {
        auto &scienceMap = GlobalData::shared()->scienceInfo.infoMap;
        vector<string> scienceConItem;
        CCCommonUtils::splitString(scienceCondition, ";", scienceConItem);
        for (int i=0; i<scienceConItem.size(); i++) {
            int curCond = atoi(scienceConItem[i].c_str());
            int scienceConditionLv = curCond%100;
            int scienceConditionId = curCond - scienceConditionLv;
            if (scienceMap.find(scienceConditionId) == scienceMap.end() || scienceMap[scienceConditionId].scienceLv < scienceConditionLv ) {
                ret += 1;
            }
        }
    }
    return ret;
}

bool ScienceItemInfo::CanResearch()
{
    bool _tOK = true;
    if (getCondition() > 0 ) {
        _tOK = false;
    }
    
//    if (_tOK) {
//        vector<string> resItems;
//        CCCommonUtils::splitString(resCondition, "|", resItems);
//        for (int i=0; i<resItems.size(); i++) {
//            vector<string> resCell;
//            CCCommonUtils::splitString(resItems[i], ";", resCell);
//            int resType = atoi(resCell[0].c_str());
//            int resNum = atoi(resCell[1].c_str());
//            if(!CCCommonUtils::isEnoughResourceByType(resType, resNum)){
//                _tOK = false;
//                break;
//            }
//        }
//    }
    
    if (_tOK) {
        vector<string> buildItems;
        CCCommonUtils::splitString(buildCondition, "|", buildItems);
        for (int i=0; i<buildItems.size(); i++) {
            int buildId = atoi(buildItems[i].c_str());
            int lv = buildId%1000;
            buildId = buildId-lv;
            if (!FunBuildController::getInstance()->isExistBuildByTypeLv(buildId, lv)) {
                _tOK = false;
                break;
            }
        }
    }
    return _tOK;
}