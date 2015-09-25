//
//  PlayerInfo.h
//  IF
//
//  Created by 邹 程 on 13-9-13.
//
//

#ifndef IF_PlayerInfo_h
#define IF_PlayerInfo_h

#include "cocos2d.h"
#include "AllianceInfo.h"
#include "UserPlatformInfo.h"

USING_NS_CC;
using namespace std;

class PlayerInfo : public cocos2d::CCObject{
public:
    PlayerInfo():
    level(0),name(string("")),nickName(string("")),uid(string("")),pic(string("")),country(0),roomName(string("")),gold(0),exp(0),maxExp(0)
    ,openedSystem(std::string("")),gmFlag(0),renameTime(0),m_allianceId(""),invited(false),sciencePower(0),battleLose(0),armyDead(0),
    fortPower(0),armyCure(0),questpower(0),playerPower(0),armyKill(0),buildingPower(0), equipPower(0),defenseWin(0),heroPower(0),attackLose(0),
    detectNum(0),cityDes(0),attackWin(0),power(0),battleWin(0),m_generals(NULL),mainCityLv(0),lastBattlePower(0),battlePower(0),addPower(0),armyPower(0),isChatView(false),selfServerId(-1),currentServerId(-1)
    ,officer(std::string("")),regCountry(""),picVer(0),lastUpdateTime(0),registerTime(0),crossFightSrcServerId(-1),srcServerPointId(-1),winPoint(0),pic_banTime(0),pic_nextUpdateTime(0),pic_banCount(0)
    {}
    int level;
    string name;
    string nickName;
    string uid;
    string pic;
    int country;
    string roomName;
    
    int allianceRank;
    long gold;
    long longJing;//龙晶
    long payTotal;
    long exp;
    long maxExp;
    std::string openedSystem;
    int gmFlag;//0:非GM 1:GM  2:mod 3:游戏内GM 4:超级mod 5:实习mod
    int renameTime;
    double protectTimeStamp;
    double resourceProtectTimeStamp;
    long vipPoints;
    long vipEndTime;
    long nextDayLoginVipReward;
    int consecutiveLoginDays;
    int isfirstJoin;        //首次加入联盟标志，0:非首次，1:首次。只有退盟的时候后台才会将其改为0.请前台不要再私自更改其值为0.一定要在退盟时改为0，否则。。。。.
    double registerTime;//玩家的注册时间戳
    double sciencePower;
    int battleLose;
    int armyDead;//军队损失数
    double armyPower;
    double fortPower;
    long armyCure;
    double questpower;
    double playerPower;
    int armyKill;//击杀军队数
    double buildingPower;
    double equipPower;//装备的战斗力加成
    int defenseWin;
    double heroPower;
    int attackLose;
    int detectNum;//侦查数
    int cityDes; //城市摧毁数
    int attackWin;
    int battleWin;
    double power;//别的玩家的total power
    bool isApplied;
    bool invited;//是否邀请过
    bool bindFlag;//是否绑定过
    AllianceInfo allianceInfo;
    int rank;
    int pointId;
    int mainCityLv;//大本等级
    string officer;//官职
    bool relinquishFlag;//是否禅让过，1是，0否
    double relinquishEndTime;//禅让截止时间
    double pic_banTime;
    double pic_nextUpdateTime;
    int pic_banCount;
    
    void updateInfo(CCDictionary* dict);
    void parseGenerals(CCArray* array);
    void updateVipEffect(bool isXml=false);
    bool isInAlliance();
    double lastBattlePower;
    double battlePower;
    double addPower;
    double getTotalPower();
    string getPic();
    string getBodyPic();
    bool isChatView;//是否进入聊天面板
    int selfServerId;
    int currentServerId;
    int crossFightSrcServerId;//表示玩家是从哪个服跨服过来的。 -1 表示没有跨服， >=0表示现在处于跨服状态
    std::string serverName;
    std::string regCountry;//国家或地区
    int srcServerPointId;//表示原服城点的坐标ID
    CC_SYNTHESIZE(string, m_allianceId, AllianceId);
    CC_SYNTHESIZE(int, m_allianceRank, AllianceRank);
    CC_SYNTHESIZE_SAFEOBJECT(cocos2d::CCArray, m_generals, Generals);
    bool isInSelfServer();
    bool isMOD();
    std::string inviCode;//邀请码
    //picVer 0:允许上传；1-99999：使用上传头像；1000001-2000000：切回系统头像，可上传；2000001-3000000：正在审核中，可上传；3000001-：审核未通过
    int picVer;
    string getCustomPicUrl();
    bool isUseCustomPic();
    int lastUpdateTime;
    
    int getAttributeByKey(string key);
    //不包括facebook，gamecenter，googleplay
    std::map<int, UserPlatformInfo> m_platFormBindInfo;
    std::vector<int> m_equip;
    
    bool showEquip;//是否装备别人可见
    int materialBackup;
    long winPoint;
};

struct LoginServerInfo{
    std::string serverIp;
    int country;
    int recommandCountry;
};


#endif
