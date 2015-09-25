//
//  ScienceInfo.h
//  IF
//
//  Created by 邹 程 on 13-11-4.
//
//

#ifndef __IF__ScienceInfo__
#define __IF__ScienceInfo__

#include "cocos2d.h"
#include "ScienceItemInfo.h"

USING_NS_CC;
using namespace std;

class ScienceInfo :
public CCObject
{
public:
    bool IsInit;
    bool IsHasData;
    int currentResearchId;
    map<int, ScienceItemInfo> infoMap;
    
    ScienceInfo():
    IsInit(false),
    IsHasData(false),
    currentResearchId(-1),
    infoMap(map<int,ScienceItemInfo>())
    {}
    
    void initSelf();
    void initScienceState(CCArray* arr);
    
    void doResearchFinish(CCObject* obj);
};

class AllianceScienceInfo : public CCObject
{
public:
    AllianceScienceInfo():
    scienceId(-1)
    ,name("")
    ,des("")
    ,icon("")
    ,floor(0)
    ,curStar(0)
    ,sumStars(0)
    ,lockPoints(0)
    ,level(0)
    ,maxLv(1)
    ,currentPro(0)
    ,needPro(0)
    ,researchTime(0)
    ,finishTime(0)
    ,para1("")
    ,para2("")
    ,nextPara2("")
    ,highRes(0)
    ,highDonateNum(0)
    ,highContribute(0)
    ,highProgress(0)
    ,midRes(0)
    ,midDonateNum(0)
    ,midContribute(0)
    ,midProgress(0)
    ,lowRes(0)
    ,lowDonateNum(0)
    ,lowContribute(0)
    ,lowProgress(0)
    ,show(0)
    ,midAvailable(0)
    ,highAvailable(0)
    {}
    
    int scienceId;
    string name;
    string des;
    string icon;
    int floor;
    int curStar;
    int sumStars;
    int lockPoints;
    
    int level;
    int maxLv;
    int currentPro;
    int needPro;
    int researchTime;
    int finishTime;
    
    string para1;
    string para2;
    string nextPara2;
    
    int highRes;
    int highDonateNum;
    int highContribute;
    int highProgress;
    
    int midRes;
    int midDonateNum;
    int midContribute;
    int midProgress;
    
    int lowRes;
    int lowDonateNum;
    int lowContribute;
    int lowProgress;
    
    int midAvailable;
    int highAvailable;
    
    int show;
    
    AllianceScienceInfo(CCDictionary* dict);
    void SetInfoFromServer(CCDictionary* dict, bool isRefersh=false);
    void UpdatePushInfo(CCDictionary* dict);
};

#endif /* defined(__IF__ScienceInfo__) */
