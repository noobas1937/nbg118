//
//  AllianceInfo.h
//  IF
//
//  Created by ganxiaohua on 14-3-24.
//
//

#ifndef __IF__AllianceInfo__
#define __IF__AllianceInfo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class AllianceInfo : public cocos2d::CCObject {
public:
    AllianceInfo():
    name(std::string()),
    shortName(std::string()),
    leader(std::string()),
    currentNum(0),
    maxNum(0),
    level(0),
    language(std::string()),
    totalForce(0),
    kills(0),
    isApplied(false),
    join(false),
    rank(0),
    currentGiftExp(0),
    currentGiftLevel(0),
    giftNextExp(1000),
    intro(std::string()),
    announce(std::string()),
    point(0),
    accPoint(0),
    donateAvailable(1),
    donateCDTime(0),
    joinTime(0),
    alliancepoint(0),
    abbrRename(0),
    strRank1(std::string()),
    strRank2(std::string()),
    strRank3(std::string()),
    strRank4(std::string()),
    strRank5(std::string()),
    reicon(0),
    militaryNum(0),
    applyNum(0),
    uid(std::string())
    ,createTime(0),
    serverId(-1),
    powerRestriction(0),
    castleRestriction(0)
    {};
    ~AllianceInfo(){};
    std::string name;//联盟名
    std::string shortName;//联盟简称
    int currentNum;//联盟当前人数
    int maxNum;//联盟最大人数
    int level;
    std::string language;
    unsigned long totalForce;//联盟总战斗力
    int kills;//联盟杀敌数
    bool isApplied;
    unsigned long fightpower;
    int giftexp1;
    int giftexp2;
    int giftexp3;
    int giftexp4;
    int giftexp5;
    int giftexp6;
    std::string uid;
    std::string icon;//旗帜
    std::string country;
    int recruit;//联盟是否 1公开 0 不公开
    std::string leaderUid;
    std::string leader;//盟主
    bool join;
    int rank;//表示他在联盟中的级别
    int giftlevel;
    int currentGiftExp;
    int reicon;//是否改过旗帜 0 没有改过，1为改过
    int currentGiftLevel;
    int helpcount;
    int giftNextExp;
    std::string intro;//宣言
    long powerRestriction;
    int castleRestriction;
    std::string announce;//公告
    std::string strRank1;
    std::string strRank2;
    std::string strRank3;
    std::string strRank4;
    std::string strRank5;
    map<int, std::pair<std::string, std::string>> territoryNameMap;//int:领地默认名字的index pair->first:领地的uid pair->second:领地的名字
//    std::string strTName1;
//    std::string strTName2;
//    std::string strTName3;
//    std::string strTName4;
//    std::string strTName5;
    int militaryNum;//联盟军情数
    int serverId;
    int applyNum;//联盟申请数量，只有R4R5能看到，其他人均为0
    
    int point;
    int accPoint;//联盟荣誉
    int donateAvailable;
    int donateCDTime;
    int joinTime;
    int alliancepoint;//联盟积分
    int abbrRename;
    time_t createTime;
    
    int leaderPoint;
    int rallyPoint;
    
    int getSelfAllianceNum();
    void updateAllianceInfo(CCDictionary* dict);
    void reset();
    std::string getAllianceIcon();
};

#endif /* defined(__IF__AllianceInfo__) */
