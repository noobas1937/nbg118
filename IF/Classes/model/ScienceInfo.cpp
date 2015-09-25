//
//  ScienceInfo.cpp
//  IF
//
//  Created by 邹 程 on 13-11-4.
//
//

#include "ScienceInfo.h"
#include "LocalController.h"
#include "GlobalData.h"
#include "GameController.h"
#include "ScienceCommand.h"
#include "ScienceController.h"

void ScienceInfo::initSelf() {
    auto& scienceInfo = GlobalData::shared()->scienceInfo;
    if (scienceInfo.IsInit) {
        return;
    }
    
    auto dictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("science");
    CCDictElement* pObj = NULL;
    CCDICT_FOREACH(dictInfo,pObj) {
        auto scienceInfoDict = _dict(pObj->getObject());
        if (!scienceInfoDict) {
            CCLOG("get science info failed!");
            return;
        }
        int scienceId = scienceInfoDict->valueForKey("id")->intValue();
        scienceInfo.infoMap[scienceId] = ScienceItemInfo(scienceInfoDict);
    }
    scienceInfo.currentResearchId = 0;
    scienceInfo.IsInit = true;
}

void ScienceInfo::initScienceState(cocos2d::CCArray *arr) {
    auto& scienceInfo = GlobalData::shared()->scienceInfo;
    CCObject* pObj;
    CCARRAY_FOREACH(arr, pObj) {
        auto info = _dict(pObj);
        if (info && info->objectForKey("itemId")) {
            int id = info->valueForKey("itemId")->intValue();
            if (scienceInfo.infoMap.find(id) != scienceInfo.infoMap.end()) {
                scienceInfo.infoMap[id].initItemInfo(info);
                scienceInfo.IsHasData = true;
            } else {
                CCLOG("science info init error: no xml init ,id:%d",id);
            }
        } else {
            CCLOG("science info init error: net info corrupt");
            return;
        }
    }
}

void ScienceInfo::doResearchFinish(cocos2d::CCObject *obj) {
    if (currentResearchId > 0) {
//        auto cmd = new ScienceResearchFinishCommand(CC_ITOA(currentResearchId+infoMap[currentResearchId].scienceLv));
//        cmd->sendAndRelease();
    }
}

////

AllianceScienceInfo::AllianceScienceInfo(CCDictionary* dict)
{//<ItemSpec id="200330" name="104126" description="102123" icon="ico200330" points="3" floor="3" hide="1"/>
    this->scienceId = dict->valueForKey("id")->intValue();
    this->name = dict->valueForKey("name")->getCString();
    this->icon = dict->valueForKey("icon")->getCString();
    this->des = dict->valueForKey("description")->getCString();
    this->lockPoints = dict->valueForKey("points")->intValue();
    this->floor = dict->valueForKey("floor")->intValue();
    this->sumStars = dict->valueForKey("donate")->intValue();
    this->show = dict->valueForKey("hide")->intValue();
    this->maxLv = dict->valueForKey("max_lv")->intValue();
    
    this->curStar = 0;
    this->level = 0;
    this->currentPro = 0;
    this->needPro = 0;
    
    this->para1 = "";
    this->para2 = "";
    this->nextPara2 = "";
    
    this->highRes = 0;
    this->highDonateNum = 0;
    this->highContribute = 0;
    this->highProgress = 0;
    this->midRes = 0;
    this->midDonateNum = 0;
    this->midContribute = 0;
    this->midAvailable = 0;
    this->highAvailable = 0;
    this->midProgress = 0;
    this->lowRes = 0;
    this->lowDonateNum = 0;
    this->lowContribute = 0;
    this->lowProgress = 0;
    
    this->researchTime = 0;
    this->finishTime = 0;
}

void AllianceScienceInfo::UpdatePushInfo(CCDictionary* dict)
{
    this->level = dict->valueForKey("level")->intValue();
    this->currentPro = dict->valueForKey("currentPro")->intValue();
    this->needPro = dict->valueForKey("needPro")->intValue();
    this->curStar = dict->valueForKey("star")->intValue();
    
    if (dict->objectForKey("startTime")) {
        int tmpTime = dict->valueForKey("startTime")->doubleValue()/1000;
        if (tmpTime>0) {
            this->researchTime = GlobalData::shared()->changeTime(tmpTime);
        }
    }
    
    if (dict->objectForKey("finishTime")) {
        int tmpTime = dict->valueForKey("finishTime")->doubleValue()/1000;
        if (tmpTime>0) {
            this->finishTime = GlobalData::shared()->changeTime(tmpTime);
        }
    }
    
    string tmpPara2 = dict->valueForKey("para2")->getCString();
    if (this->para2 != tmpPara2) {
        vector<string> effVec;
        CCCommonUtils::splitString(this->para1, ";", effVec);
        float value = atof(tmpPara2.c_str()) - atof(this->para2.c_str());
        for (int i=0; i<effVec.size(); i++) {
            int effNum = atoi(effVec[i].c_str());
            ScienceController::getInstance()->alScEffMap[effNum] += value;
        }
    }
    
    this->para1 = dict->valueForKey("para1")->getCString();
    this->para2 = dict->valueForKey("para2")->getCString();
    this->nextPara2 = dict->valueForKey("nextPara2")->getCString();
}

void AllianceScienceInfo::SetInfoFromServer(CCDictionary* dict, bool isRefersh)
{
    this->highRes = dict->valueForKey("highRes")->intValue();
    this->highDonateNum = dict->valueForKey("highNum")->intValue();
    this->highContribute = dict->valueForKey("highContribute")->intValue();
    this->highProgress = dict->valueForKey("highProgress")->intValue();
    this->midRes = dict->valueForKey("midRes")->intValue();
    this->midDonateNum = dict->valueForKey("midNum")->intValue();
    this->midContribute = dict->valueForKey("midContribute")->intValue();
    this->midProgress = dict->valueForKey("midProgress")->intValue();
    this->lowRes = dict->valueForKey("lowRes")->intValue();
    this->lowDonateNum = dict->valueForKey("lowNum")->intValue();
    this->lowContribute = dict->valueForKey("lowContribute")->intValue();
    this->lowProgress = dict->valueForKey("lowProgress")->intValue();
    this->midAvailable = dict->valueForKey("midAvailable")->intValue();
    this->highAvailable = dict->valueForKey("highAvailable")->intValue();
    
    if (isRefersh) {
        return;
    }
    this->level = dict->valueForKey("level")->intValue();
    this->currentPro = dict->valueForKey("currentPro")->intValue();
    this->needPro = dict->valueForKey("needPro")->intValue();
    this->curStar = dict->valueForKey("star")->intValue();
    
    if (dict->objectForKey("startTime")) {
        int tmpTime = dict->valueForKey("startTime")->doubleValue()/1000;
        if (tmpTime>0) {
            this->researchTime = GlobalData::shared()->changeTime(tmpTime);
        }
    }
    
    if (dict->objectForKey("finishTime")) {
        int tmpTime = dict->valueForKey("finishTime")->doubleValue()/1000;
        if (tmpTime>0) {
            this->finishTime = GlobalData::shared()->changeTime(tmpTime);
        }
    }
    
    this->para1 = dict->valueForKey("para1")->getCString();
    this->para2 = dict->valueForKey("para2")->getCString();
    this->nextPara2 = dict->valueForKey("nextPara2")->getCString();
}