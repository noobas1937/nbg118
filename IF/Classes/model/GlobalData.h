//
//  GlobalData.h
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//
//

#ifndef __IF__GlobalData__
#define __IF__GlobalData__

#include <string>
#include "cocos2d.h"
#include "TypeDefinition.h"
#include "PlayerInfo.h"
#include "ResourceInfo.h"
#include "GeneralInfo.h"
#include "ServerInfo.h"
#include "UserLvAboutInfo.h"
#include "MailInfo.h"
#include "LordInfo.h"
#include "FunBuildInfo.h"
#include "DragonInfo.h"
#include "ScienceInfo.h"
#include "GeneralConfig.h"
#include "LoginRewardInfo.h"
#include "GoldExchangeItem.h"
#include "AchievementInfo.h"
#include "MailConfig.h"
#include "ChatShieldInfo.h"
#include "QueueInfo.h"
#include "ArmyInfo.h"
#include "TreatInfo.h"
#include "WorldConfig.h"
#include "TrainInfo.h"
#include "TalentInfo.h"
#include "AllianceInfo.h"
#include "MailCountObj.h"
#include "AllianceDonateInfo.h"
#include "RankListInfo.h"
#include "FirstPayInfo.h"
#include "SacrificeInfo.h"
#include "PushSettingInfo.h"
#include "TitanInfo.h"//fusheng 泰坦info

#define FULL_WIN_WIDTH    768
#define FULL_WIN_HEIGHT   1364

#define ARMY_LOAD_EFFECT 78
#define GENERAL_LOAD_EFFECT 133
#define FORCE_PRODUCT_SPEED_EFFECT 66
#define FORT_PRODUCT_SPEED_EFFECT 67
#define TREAT_NUM_MAX_EFFECT 57
#define TREAT_NUM_MAX_EFFECT_ADD 73
#define TRAP_NUM_MAX_EFFECT 88

#define BU_PRODUCT_SPEED_EFFECT 113
#define QI_PRODUCT_SPEED_EFFECT 114
#define GONG_PRODUCT_SPEED_EFFECT 115
#define CHE_PRODUCT_SPEED_EFFECT 116
#define PRODUCT_TRAIN_NUM_EFFECT 131
#define WORLD_STAMINE_INTERVAL 106
#define WORLD_FIELD_MONSTER_TIME 59
#define WORLD_UNIN_EFFECT 75
#define WORLD_YUAN_EFFECT 58
#define WORLD_MARCH_NUM 55
#define WORLD_MARCH_SPEED 60
#define ALLIANCE_TRADE_RATE_EFFECT 139
#define ALLIANCE_TRADE_MAX_EFFECT 140
#define ALLIANCE_TRADE_SPEED_TIME_EFFECT 74
#define ALLIANCE_MEMBER_MAX_FACTOR 500
#define ALLIANCE_TEAM_MEMBER_MAX 143

#define LORD_SKILL_PROTECTED 501051
#define PLAYER_PROTECTED_TIME1 500000
#define PLAYER_PROTECTED_TIME2 500001
#define PLAYER_PROTECTED_TIME3 500002
#define PLAYER_PROTECTED_TIME4 500003
#define PLAYER_PROTECTED_TIME5 500004
#define PLAYER_PROTECTED_TIME6 500005 //7天保护

#define NEW_PLAYER_PROTECTED 500009

#define KINGDOM_KING_ID "216000"

#define COMMAND_EFFECT 8
#define SCARECROW_EFFECT 63

#define TEXT_COLOR_WHITE {173,175,172}
#define TEXT_COLOR_GREEN {83,151,33}
#define TEXT_COLOR_BLUE {71,132,225}
#define TEXT_COLOR_PURPLE {112,72,194}
#define TEXT_COLOR_ORANGE {193,129,72}
#define TEXT_COLOR_YELLOW {205,183,63}

enum WorldResourceType {
    Wood,
    Stone,
    Iron,
    Food,
    Silver,//钢矿
    Gold,
    Chip,//铜矿
    Diamond,//龙矿
    LongJing,//龙晶
    WorldResource_Max,
    
    DragonFood=20,
    DragonGold
};

enum SOILDERTYPE{
    BU = 1,
    QI,
    GONG,
    CHE,
    TITAN,//fusheng 原本是SENG 现在改成TITAN
};

enum SpecialSteps{
    STEP1 = 1,
    STEP2,
    STEP3,
    STEP4,
    STEP5,
    STEP6,
    STEP7,
    STEP8,
};

enum SysBtn{
    SYSTEM_HOME = 1,
    SYSTEM_TIME,
    SYSTEM_HERO,
    SYSTEM_ITEM,
    SYSTEM_ALLIANCE,
    SYSTEM_MORE,
    SYSTEM_QUEST,
    SYSTEM_WORLD,
};

enum Color {
    WHITE,
    GREEN,
    BLUE,
    PURPLE,
    ORANGE,
    GOLDEN,
    RED,
};

enum SceneType {
    MOUNTAIN,
    WATER,
    PLAIN,
    CITY,
};

enum RewardType
{
    R_WOOD
    ,R_STONE
    ,R_IRON
    ,R_FOOD
    ,R_SILVER
    ,R_GOLD
    ,R_EXP
    ,R_GOODS
    ,R_GENERAL
    ,R_POWER
    ,R_HONOR
    ,R_ALLIANCE_POINT
    ,R_CHIP
    ,R_DIAMOND
    ,R_EQUIP
    ,R_DRAGONFOOD
    ,R_DRAGONGOLD
    ,R_EFFECT
    ,R_WIN_POINT
    ,R_CRYSTAL//水晶 19
};

enum ItemTypes {
    NORMAL = 1,
    WEAPON,
    HORSE,
    ARMOR,
    SHIELD,
    FLAG,
    HORN,
    DOCORATION
};

enum Directions{
    UP = 1,
    DOWN,
    LEFT,
    RIGHT,
    RIGHT_UP,
    RIGHT_DOWN,
    LEFT_DOWN,
    LEFT_UP,
};

enum NpcState{
    STAND_STATE = 1,
    WALKING_STATE,
};

enum EFFECT{
    GENERAL_ATTACK = 1,
    GENERAL_DEFENCE,
    GENERAL_SOLDIER,
    GENERAL_COMMAND,
    GENERAL_INTELLIGENCE,
};

enum FormationType{
    PVE_FORMATION,
    PVP_FORMATION,
};

enum BattleType
{
    PVE
    ,PVP
};

enum GridType {
    NEUTRALLY
    , STOP
    , SPACE
};

enum ArmEnumType{
    ARM_BU = 1,
    ARM_QIANG,
    ARM_RIDE,
    ARM_RIDE_SHE,
    ARM_GONG,
    ARM_NU,
    ARM_TOU_SHI_BING,
    ARM_CHE,
};

typedef enum
{
    STORE_TYPE_STORE,
    STORE_TYPE_BAG,
    
    STORE_TYPE_GOTO_STORE
} StoreType;

typedef enum
{
    STORE_RESOURCE_TYPE_WAR = 1,
    STORE_RESOURCE_TYPE_SPEED_UP,
    STORE_RESOURCE_TYPE_RESOURCE,
    STORE_RESOURCE_TYPE_OTHER,
} StoreResourceType;

typedef enum
{
    FLY_HINT_LOGIN,
    FLY_HINT_KING,
    FLY_HINT_SYSTEM,
    FLY_HINT_ALLIANCE_INVITE,
} FlyHintType;

enum ChatType{
    CHAT_TYPE_COUNTRY,
    CHAT_TYPE_SELF,
    CHAT_TYPE_ALLIANCE,
    CHAT_TYPE_INVITE,
    CHAT_TYPE_BATTLE_SHARE,
    CHAT_TYPE_DETECT_SHARE,
    CHAT_TYPE_NOTICE,
    CHAT_TYPE_EQUIP_SHARE
};

class TimeStamp {
public:
    TimeStampType type;
    time_t finishTime;
};

typedef std::map<int,TimeStamp> TimeStampMap;

class stateEffect {
public:
    int value;
    int effectId;
    int stateId;
};

class GlobalData: public cocos2d::CCObject {
public:
    static GlobalData* shared();
    static void purgeData();
    void initRand();
    bool isUiInti;
    bool isTodayFirstLogin; //是否今日第一次登陆
    bool ifShowNews;        //是否弹出新闻框
    bool isLoginFlag;       //登陆login：true
    bool isInitFlag;
    bool isInitFinish;
    bool isPause;
    bool isBind;//绑定状态
    bool isPayBind;//绑定状态
    bool historyPurchaseChecked;//历史订单每次开游戏只检查一次
    bool isXMLInitFlag;
    bool isFirstLoginGame;
    bool translation;//是否开启自动翻译
    bool mail_translation;//是否开启邮件的自动翻译
    bool isShowCDkey;//是否开启CDkey
    bool isGoogle();
    bool needLogin();
    bool needLogout();
    bool cantSwtichAccount();
    bool isWeiboEnabled();
    bool isVKEnabled();
    bool isPressedHomeKey;// 是否按了home键
    bool isChinaPlatForm();
    bool isXiaoMiPlatForm();
    
    std::string phoneDevice;
    std::string downloadUrl;
    int updateType;//0: 不更新；1:建议更新；2：强制更新。
    string version;//前台版本号
    string xmlVersion;//当前最新xml版本号
    string serverVersion;//当前客户端版本号
    string lang;//语言
    string uuid;// uuid
    string platform;//系统平台 ios ad
    string platformUID;//平台UID
    string platformToken;//平台登陆验证
    string analyticID;//发布平台 91 google 360.
    string gcmRegisterId;//GCM注册ID
    string parseRegisterId;//Parse注册ID
    string gaid;//adjust 发奖依据
    string gaidCache;//android读取到gaid以后缓存起来
    string referrer;//广告来源数据
    string fromCountry;//国家
    time_t lastAcceptEnergyTime;
    time_t pauseTime;//暂停时间
    string connectMode;//sfs连接模式
    int bagGoldBuyNum;//金币购买武将背包数
    string inviteFriendsInfo;//邀请的朋友数据
    string installFriendsInfo;//安装了的朋友数据
    string medalid;//玩家选择的勋章id;

    map<std::string, ArmyInfo> armyList;
    map<std::string, ArmyInfo> fortList;
    map<std::string, TreatInfo> treatList;
    map<string, string> historyPurchaseList;

    LoginRewardInfo loginReward;
    LordInfo lordInfo;
    LoginServerInfo loginServerInfo;
    MailConfig mailConfig;
    GeneralConfig generalConfig;
    TitanInfo titanInfo;

    PlayerInfo playerInfo;
    ChatShieldInfo chatShieldInfo;//屏蔽玩家信息
    
    FirstPayInfo payRewardInfo;
    SacrificeInfo sacrificeInfo;
    WorldConfig worldConfig;
    unsigned int cityDefenseVal;
    double cityFireStamp;
    double cityStartFireStamp;
    double cityLastUpdateDefenseStamp;
    unsigned int cityTileCountry;
    unsigned int cityUpdateDefenseDelayMin;
    time_t goldExchageRefreshStamp;

    //武将
    map<string,GeneralInfo> generals;
    //将魂
    map<string, GeneralInfo> souls;
    //已获得武将的id
    map<string, int>generalAlreadyGet;
    
    map<string,GeneralInfo> unHiredGenerals;
    
    // science effect map
    map<string,int> scienceEffect;
    map<int, float> effectValues;
    TimeStampMap timeStampMap;
    
    ScienceInfo scienceInfo;
    //资源信息
    ResourceInfo resourceInfo;
    //祭祀相关信息
    MailCountObj mailCountObj;
    
    map<int, QueueInfo> allQueuesInfo;
    map<int, FunBuildInfo> imperialInfo;
    map<int, FunBuildInfo> UpgradeBuildsInfo;
    //龙建筑
    map<int, FunBuildInfo> dragonBuildingInfo;
    map<int, FunBuildInfo> UpDragonBuildsInfo;
    //龙建筑
    map<int, DragonInfo> dragonInfoMap;
    
    map<int, int> corpCreateInfo;
    map<int, int> woodCreateInfo;
    map<int, int> ironCreateInfo;
    map<int, int> barracksCreateInfo;
    map<int, int> marketCreateInfo;
    //用户级别相关信息
    UserLvAboutInfo userLvAboutInfo;
    
    map<int,ServerInfo> serverList;
    //邮件列表
    map<string, MailInfo*> mailList;
    //仓库物品列表
    map<string, EquipInfo*> inventoryList;
    
    map<int, CCArray*> achList;
    
    
    vector<AllianceDonateInfo*> allianceDonateAllList;
    vector<AllianceDonateInfo*> allianceDonateTodayList;
    vector<AllianceDonateInfo*> allianceDonateWeekList;
    
    RankListInfo rankListInfo;

    //金币兑换
    map<string, GoldExchangeItem*> goldExchangeList;
    map<string, MonthCardInfo*> monthCardInfoList;
    double tomorrow_time;
    //成就
    map<string, AchievementInfo*> achievementList;
    //训练的英雄
    map<string, TrainInfo*> trainList;
    //推送设置
    vector<int> cd_time;
    //cd时间
    vector<PushSettingInfo*> pushSettingVector;
    vector<int> cd_cost;
    map<string, float> cd_gold;

    
    //res_cost
    map<string, float> res1_num;
    map<string, float> res2_num;
    map<string, float> res3_num;
    map<string, float> res_cost;
    
    //状态 和 作用值 相关
    map<int, double> statusMap;
    map<int, vector<stateEffect> > effectStateMap;
    map<int, int> vipEffectMap;
    map<int, CCDictionary*> vipListMap;
    time_t getWorldTime();
    time_t getTimeStamp();
    time_t changeTime(time_t t);
    double renewTime(double t);
    int getTimeZone();
    void setWorldTime(time_t t, int tz);
    int getRand(int min=0,int max=std::numeric_limits<int>::max());
    
    int essenceK1;
    int essenceK2;
    int essenceK3;
    int flushFreeTimes;
    int flushGoldTimes;
    int flushCurrentTime; //洗炼当前刷新的时间
    int recruitedGeneralCount;//可招武将最大数
    
    int quickEndLevel;//能立即结束战斗的等级
    int contryResType;
    float speedFactor;//加速因子
    int focusBuildId;//
    int freeSpdT;
    
    int MaxPlayerLv;
    int MaxHeroLv;
    int MaxBuildLv;
    
    int shield_base;
    int cd_goldK1;
    int cd_goldK2;
    int alliance_cost_k1;
    int alliance_cost_k2;
    int alliance_cost_k3;
    int alliance_cost_k5;
    int alliance_cost_k6;
    int alliance_cost_k7;
    int alliance_cost_k8;
    
    int alliance_science_k1;
    int alliance_science_k2;
    int alliance_science_k3;
    int alliance_science_k4;
    int alliance_science_k5;
    int alliance_science_k6;
    int alliance_science_k7;
    int alliance_science_k8;
    int alliance_science_k9;
    int alliance_science_k10;
    int alliance_science_k11;
    int alliance_science_k12;
    
    int resources_weight_1;
    int resources_weight_2;
    int resources_weight_3;
    int resources_weight_4;
    int resources_weight_5;
    int resources_weight_6;
    int resources_weight_7;

    int world_alliance_war_k1;
    int world_alliance_war_k2;
    
    int world_rally_k1;
    int world_rally_k2;
    int world_rally_k3;
    int world_rally_k4;
    int world_rally_k5;
    int world_rally_k6;
    int world_rally_k7;
    int world_rally_k8;
    
    int glodMine_fb_k1;
    int glodMine_fb_k2;
    int glodMine_fb_k3;
    int glodMine_fb_k4;
    
    int invite_cost_k1;
    
    int alliance_gift_k2;
    long alliance_gift_k3;
    int alliance_fun_on3_k5;
    int replayTime;
    int cnReplayFlag;
    int sceneResourceStepLoad;
//    int world_detect_2;
//    int world_detect_3;
    
    std::string m_trainGeneralId;
    std::string m_trainId;
    std::string m_speedTrainId;
    std::string m_harvestInfo;//丰收信息
    std::string m_skillFortInfo;//技能造陷阱信息
    float battleSpeedFactor;
    int facebook_gift_k7;//facebook礼包开关
    bool m_isFirstOpenDailyView;
    bool m_isFirstAskFor;//从facebook askfor进来的
    int android_native_chat;
    bool isOpenForum;//论坛 0关闭入口，1显示入口
    int alliance_territory_tower_switch;//联盟领地箭塔 1显示入口，0关闭入口
    int hd_switch;//是否使用hd资源 1使用，0关闭
    string hd_switch_version;
    int alliance_territory_banner_switch;//联盟领地国旗 1显示入口，0关闭入口
    int alliance_territory_positive_effect;//联盟领地增益(除去采集)
    int alliance_territory_positive_effect_gather;//联盟领地增益(世界采集速度)
    int alliance_territory_negative_effect;//联盟领地减益
    int alliance_territory_supermine_limit;//联盟采集超级矿等级限制
    int alliance_territory_index;
    unsigned long territory_warehouse_dayStorage;
    unsigned long territory_warehouse_totalStorage;
    int march_formation_switch;//编队功能开头
    bool isInviteOpen;//邀请码
    bool isOpenEvaluate;
    //活动面板弹出的次数
    int popupTimes;
    //当前设备创建的新账号的数量
    int nowGameCnt;
    //游戏所允许的在一台设备上创建账号的数量
    int maxGameCnt;
    //防止玩家通过小号速成
    int cnt_forbidden_swith;//开关 1开 0关
    int cnt_forbidden_level;//小号等级限制
    double cnt_forbidden_playingtime;//小号游戏时间限制
    int trans_res_forbidden_swith;//开关 1开 0关
    long trans_res_limit;//小号跨服迁城可带资源量
    map<string, CCDictionary*> shareFbmap;//fb分享的数据
    CCDictionary* pushData = NULL;//push数据临时存储
    bool isInDataParsing;
    bool isUploadPic;//是否在上传照片
    bool isOpenTaiwanFlag;//大陆看台湾国旗开关1开0关
    bool isOpenAnniversary;//周年庆开关1开0关
    bool isAnniSharePhoto;//周年庆分享照片
    int anniSharePhoto;//周年庆分享照片
    int shakeAllianceFlag;
    int picUploadFlag;
    bool isBranchOn;//branch
    string branchOnStr;//branch
    string dragonglass_control;//branch
    string equipShare_control;//装备分享开关
    int shakeFlag;
    int shakeGuideFlag;
    int shakeCmdDelayTime;
    string shakePicStr;
    bool isCrossService;//是否是跨服
    bool firstEnterCrossService;//是否是第一次进跨服
    int wb_fire_k1;//跨服燃烧值，k1表示每小时的燃烧城防值
    int serverMax;
    map<string, int> testSwitchMap;//留存功能测试开关
    int chinaSwitchFlag;//大陆显示国旗开关 0不变 1为将五星红旗显示为纯红色旗子 2为不显示国旗
private:
    GlobalData() = default;
    time_t worldTime;
    time_t localTime;
    int timeZone;
};

#endif /* defined(__IF__GlobalData__) */
