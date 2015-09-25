//
//  ScienceItemInfo.h
//  IF
//
//  Created by 邹 程 on 13-11-4.
//
//

#ifndef __IF__ScienceItemInfo__
#define __IF__ScienceItemInfo__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class ScienceItemInfo
{
public:
ScienceItemInfo():
    scienceId(-1)
    ,scienceType(-1)
    ,scienceLv(-1)
    ,investMax(-1)
    ,investNum(-1)
    ,posX(0)
    ,posY(0)
    ,relation("")
    ,scienceCondition("")
    ,buildCondition("")
    ,resCondition("")
    ,time_need(0)
    ,para1("")
    ,para2("")
    ,nextPara2("")
    ,finishTime(0)
    ,show(0)
    {}

    int scienceId;
    int scienceType;
    int scienceLv;
    int scienceMaxLv;
    int investMax;
    int investNum;
    
    int posX;
    int posY;
    string relation;
    
    string scienceCondition;
    string buildCondition;
    string resCondition;
    int time_need;
    
    string para1;
    string para2;
    string nextPara2;
    double finishTime;
    int show;
    
    map<int,int> getInvest();
    vector<int> getPositionIndex();
    string getName();
    string getIcon();
    string getLine();
    string getDescription();
    
    int getCondition();
    bool CanResearch();
    
    void initItemInfo(CCDictionary* dict);
    
    ScienceItemInfo(CCDictionary* dict);
    
    
};
#endif /* defined(__IF__ScienceItemInfo__) */
