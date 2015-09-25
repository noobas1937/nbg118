//
//  MarchDetailView.h
//  IF
//
//  Created by 邹 程 on 14-3-27.
//
//

#ifndef __IF__MarchDetailView__
#define __IF__MarchDetailView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "WorldController.h"
#include "PopupBaseView.h"


static const float _fadeInTime = 0.5;
static const float _fadeOutTime = 0.5;
static const float _delayTime = 5.0;




class MarchGeneralDetail {
public:
    string generalId;
    int generalLv;
    
    MarchGeneralDetail():
    generalId("")
    ,generalLv(0)
    {}
};

class MarchSoldierDetail {
public:
    string armyId;
    int count;
    std::string playerName;
    bool isLeader;
    string monsterId;
    
    MarchSoldierDetail() :
    armyId("")
    , count(0)
    , playerName("")
    , isLeader(false)
    ,monsterId("")
    {};
};

class MarchDetailInfo {
public:
    std::string userUid;
    string uuid;
    unsigned int targetPointIndex;
    MarchStateType marchState;
    WorldCityType marchTarget;
    MarchMethodType marchMethod;
    int totalTroop;
    double startStamp;
    double endStamp;
    int actionMax;
    string name;
    std::map<int, vector<MarchSoldierDetail>> soldiers;
    vector<MarchGeneralDetail> generals;
    
    bool inUpdate;
    
MarchDetailInfo():
    uuid("")
    ,targetPointIndex(0)
    ,marchState(StateMarch)
    ,marchTarget(OriginTile)
    ,marchMethod(MethodBattle)
    ,totalTroop(0)
    ,startStamp(0.0)
    ,endStamp(0.0)
    ,actionMax(0)
    ,name("")
    ,generals(vector<MarchGeneralDetail>())
    ,soldiers(std::map<int, vector<MarchSoldierDetail>>())
    ,inUpdate(false)
    {}
    
    void setInfo(CCDictionary* dict);
};
#endif /* defined(__IF__MarchDetailView__) */
