//
//  TalentInfo.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-20.
//
//

#include "TalentInfo.h"
#include "LocalController.h"
#include "TalentController.h"

TalentInfo::TalentInfo(cocos2d::CCDictionary* talentDic)
{
    this->talentId = talentDic->valueForKey("id")->intValue();
    this->maxLevel = talentDic->valueForKey("levelmax")->intValue();
    this->effectNum = talentDic->valueForKey("effect")->intValue();
    this->open = talentDic->valueForKey("open")->getCString();
    this->relation = talentDic->valueForKey("relation")->getCString();
    string site = talentDic->valueForKey("site")->getCString();
    vector<string> pt;
    CCCommonUtils::splitString(site, ",", pt);
    this->posX = atoi(pt[0].c_str());
    this->posY = atoi(pt[1].c_str());
    
    string value = talentDic->valueForKey("value")->getCString();
    vector<string> effVal;
    CCCommonUtils::splitString(value, ";", effVal);
    for (int i=0; i<effVal.size(); i++) {
        this->effectValue.push_back( atoi(effVal[i].c_str()) );
    }
    this->level = 0;
}

bool TalentInfo::isLock()
{
    vector<string> conditions;
    CCCommonUtils::splitString(this->open, "|", conditions);
    for (int i=0; i<conditions.size(); i++) {
        string curCond = conditions[i];
        vector<string> cell;
        CCCommonUtils::splitString(curCond, ";", cell);
        if (cell.size()>=2) {
            int tmpId = atoi(cell[0].c_str());
            int tmpLv = atoi(cell[1].c_str());
            if ( TalentController::getInstance()->talentMap[tmpId].level < tmpLv ) {
                return false;
            }
        }
    }
    return true;
}