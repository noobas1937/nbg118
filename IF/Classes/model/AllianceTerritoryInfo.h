//
//  AllianceTerritoryInfo.h
//  IF
//
//  Created by 〜TIAN~ on 15/4/9.
//
//

#ifndef __IF__AllianceTerritoryInfo__
#define __IF__AllianceTerritoryInfo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"
#include "GlobalData.h"
#include "CommonInclude.h"
#include "WorldController.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class AllianceTerritoryInfo:public cocos2d::CCObject
{
public:
    string m_territoryId;
    string m_territoryName;
    string m_allianceAbbr;
    string m_allianceName;
    string m_territoryPic;
    int m_territoryState;
    int m_territoryLv;
    int m_territoryDef;
    int m_territoryRes;
    int m_territoryPtIndex;
    WorldCityType m_territoryType;
    WorldResourceType m_territoryResourceType;
    bool m_lockFlag;//是否显示问号
    bool m_ownerFlag;
    void parseInfo(CCDictionary* dict);
    int m_territoryActive;//1：后台传的数据（后台传的数据必进初始化函数）、超级矿如未传数据时四个均为1。0：除超级矿四个全是造的数据外其他情况全为0
    int m_territoryCount;
    unsigned long m_totalResStorage;
    string m_banner;//国旗名称
};

#endif /* defined(__IF__AllianceTerritoryInfo__) */
