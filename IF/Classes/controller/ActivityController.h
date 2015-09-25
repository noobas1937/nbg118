//
//  ActivityController.h
//  IF
//
//  Created by 李锐奇 on 14-4-3.
//
//

#ifndef __IF__ActivityController__
#define __IF__ActivityController__

#include "CommonInclude.h"
#include "ActivityInfo.h"
#include "ActivityEventInfo.h"
#include "AllianceActivityInfo.h"
#include "ActivityEventObj.h"

#define ACTIVITY_NUM_CHANGE "activityNumChange"
#define ACTIVITY_SCORE_CHANGE "activityScoreChange"

#define CHRISTMAS_SCOKS_EXCHANGE "christmas.scoks.exchange"
#define CHRISTMAS_LOGINRD_END "christmas.loginRd.end"
#define CHRISTMAS_SNOW_END "christmas.snow.end"
#define CHRISTMAS_DATA_INIT "christmas.data.init"

#define PORT_VIP_LOGINRD_END "port.vip.loginRd.end"
#define PORT_LOGINRD_END "port.loginRd.end"
#define PORT_TIME_EXCHANGE "port.time.exchange"
#define PORT_DATA_INIT "port.data.init"
#define MSG_NEW_TIME_RWD_END "msg.new.time.rwd.end"
#define MSG_REPAY_INFO_INIT "repay.info.init"
#define MSG_NEW_PLAYER_RD "msg.new.player.rd"

#define MSG_GET_ACT6_DATA_SUCCESS "msg.get.act6.data.success"
#define MSG_GET_ACT6_DATA_FAIL "msg.get.act6.data.fail"

#define MSG_ACT6_EXC_SUCCESS "msg.act6.exc.success"
#define MSG_ACT6_EXC_FAIL "msg.act6.exc.fail"

#define MSG_ACT6_START_REFRESH "msg.act6.start.refresh"
#define MSG_ACT6_REFRESH_SUCCESS "msg.act6.refresh.success"
#define MSG_ACT6_REFRESH_FAIL "msg.act6.refresh.fail"

#define MSG_ACT6_SHOWRWD_END "msg.act6.showrwd.end"

#define MSG_GET_ATK_DATA "msg.get.atk.data"
#define MSG_TRIAL_START_SUC "msg.trial.start.suc"
#define MSG_TRIAL_FINISH_SUC "msg.trial.finish.suc"
#define MSG_TRIAL_BUY_SUC "msg.trial.buy.suc"

enum ActivityType{
    ACTIVITY_SOLO = 1,
    ACTIVITY_ALLIANCE,
};

enum ActivityGetScoreType{
    KILL_ARMY,
    TRAINING_ARMY,
    DESTROY_TRAP,
    PRODUCT_TRAP,
};

class ActExc2Info
{
public:
    void update(CCDictionary*);
    ActExc2Info(CCDictionary*);
    ActExc2Info(): m_type(0), m_multiple(0), m_reward(nullptr){};
    int m_type;
    int m_multiple;
    vector<string> m_ids;
    CCSafeObject<CCArray> m_reward;
    vector<string> m_rwdIds;
};

class Act2RefrshInfo
{
public:
    vector<string> m_excIds;
    int m_refresh;
    int m_freeNum;
    int m_refreshCost;
    int m_refreshTime;
};

const int prepareTime = 30 * 60;

class ActivityController : public CCObject{
public:
    static ActivityController *getInstance();
    ActivityController();
    virtual ~ActivityController();
    void addActivity(CCArray *arr);
    void addActivity(CCDictionary *dict);
    void removeActivity(ActivityInfo *info);
    void removeActivity(int type);
    
    bool hasActivity(int type);
    
    bool isActivityTimeOver(ActivityInfo *info);
    void doWhenTimeOver(float t = 0.0);
    void updateCurrentScore(int type, int score);
    void updateCurrentEvent(CCDictionary *dict);
    void openActivityView();
    void loginEventData();
    void paraseEventObj(CCArray *arr);
    void enterGameOpenView(int type = 0);
    
    bool activityIsOpen();
    void activityRecord(int type);
    void rewardRecordHandle();
    std::string UrlDecode(const std::string& szToDecode);
    void facebookPostMethod(string key,CCArray* param=NULL);
    void openCrossServerActivity();
    void leaveCKFServer(CCDictionary *dict);
    bool checkShengDianOpen();
    bool checkLianShengOpen();
    bool checkOpenVersion(string version);
    bool checkOpenDragonBattle();
    bool checkUseCKFMassTime();
    
    ActivityEventObj* getActObj(string itemId);
    
    map<int, ActivityInfo*> activity;
    
    ActivityEventInfo eventInfo;
    CCSafeObject<CCArray> activityArr;//ActivityEventObj
    CCSafeObject<CCArray> speActivityArr;//ActivityEventObj
    CCSafeObject<CCArray> allianceKill;
    CCSafeObject<CCArray> gole;
    CCSafeObject<CCArray> memberKill;
    AllianceActivityInfo  aActivityInfo;//联盟活动信息
    CCSafeObject<CCDictionary> ckfServerInfo;//ckf离开的服务信息
    CCSafeObject<CCArray> baozangHelpArr;//宝藏可帮好友的信息
    
    string getActivityInfo();
    void sortActivityArr();
    
    //type = 6的活动
    void createRwdViewByActId(string actId);
    
    bool checkIsToday(time_t time);
    bool checkCanExchange2(string actId, int idx);
    
    void startGetAct6Data();
    void endGetAct6Data(CCDictionary*);
    void startAct6Exchange(string, int);
    void endAct6Exchange(CCDictionary*);
    void startAct6Refresh(string);
    void endAct6Refresh(CCDictionary*);
    
    void createGoodsIconWithGray(CCNode* parent, string itemId, float size);
    void createGoodsIcon(CCNode* parent, string itemId, float size);
    string createRandomGoodsId(string actId, int type);//type == 2  是碎片活动 ； type == 1 是普通的兑换活动
    
    bool isHasAct(string actId);
    
    //进游戏时活动弹窗相关
    string getRunningPopEvent();
    string getRandomPopActId();
    void showRandomActPop();
    void parseActConfig(CCDictionary* dict);//包括试炼场相关配置
    
    //每天活动弹窗的最大次数
    int popCntLimit;
    
    //type = 6 的碎片兑换相关数据
    map<string, Act2RefrshInfo> m_exc2RefInfo;
    map<string, map<int, ActExc2Info> > m_exc2Data;

    int wb_condition_k1;//跨服控制进入条件 玩家大本等级
    int wb_condition_k2;//跨服控制进入条件 需要玩家道具的id
    int wb_condition_k3;//跨服控制进入条件 表示所需道具的数量
    string wb_info_k2;//开放功能的版本
    int wb_info_k6;//总时间
    int wb_info_k7;//表示活动每个阶段的时间，以小时为单位，当前值=3
    int wb_info_k9;//跨服伤兵速度加倍那个
    float wb_info_k10;//跨服伤兵速度加倍,乘方因子
    int wb_info_k11;//跨服是从多少连胜开始显示
    
    int wb_timing_k1;//活动结束前，开始进入下一个阶段的提示
    int wb_timing_k2;//每间一段时间提示一次
    int ckfNeedLeave;//已经时间到，要离开中立服，但还没有离开的标志
    double ckfProtectTime;//跨服保护时间
    int wb_switch_k1;//跨服连胜的开关
    string wb_switch_k2;//跨服连胜开放的版本
    int wb_switch_k3;//跨服技能的开关
    string wb_switch_k4;//跨服技能开放的版本
    
    int wb_switch_k6;//集结时间的开关
    string wb_switch_k7;//集结时间开放的版本
    
    double m_userScore;//跨服用户积分
    int m_userRank;//跨服用户排名
    double m_kingdomScore;//跨服王国积分
    int m_kingdomRank;//跨服王国排名
    
    
    map<string, string> wbScoreMap;//跨服获得积分计算
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_ckfMemberReward, CkfMemberReward);//跨服个人奖励
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_ckfKingdomReward, CkfKingdomReward);//跨服王国奖励

    //进入游戏的弹窗
    void initViewConfig(CCDictionary *ccDict);
    void showEnterView();
    void showDailyRwd(CCObject* ccObj);
    
    //-------试炼场活动-------start-------
    void parseTrialInfo(CCDictionary* );
    void endAtkCmd(CCDictionary*);
    void endStartTrialCmd(CCDictionary*);
    void endTrialFinishCmd(CCDictionary*);
    void endTrialBuyCmd(CCDictionary*);
    void showTrainingView();
    int getMyTrialState();//得到当前的活动进行程度  1 进行全新一轮的试炼 2 继续进行上一轮的攻击 3 攻击完要领奖但没有打完所有的怪物 4 打完所有的怪物要领奖 5 攻击完无法领奖
    int getKilledMonsterCnt();
    int getAllMonsterCnt();
    
    int m_freeAtk;//free攻击次数
    vector<int> m_screen;//0 1 2三种攻击在16个格子中的位置
    int m_blood;//怪物当前血量
    string m_monster;//怪物ID
    int m_maxBlood;//怪物最大血量
    int m_freeTrial;//免费挑战次数
    int m_maxAtkCnt;//每局玩家可攻击怪兽的次数
    vector<int> m_atkHarm;//三种攻击对怪物的伤害
    int m_isTrialOpen;//试炼场活动开关
    int m_bigStrengthGold;//蓄力攻击花费金币数量
    int m_moreAtkGold;//增加攻击次数花费金币数量
    int m_addedAtkCnt;//花金币增加的攻击次数数量
    int m_newTrialGold;//新挑战花费的金币数量
    bool m_isInAtkCmdSending;//正在发送攻击命令
    int m_bigStrenMul; // 蓄力时的攻击倍数
    vector<int> m_monsterDiv; //怪物的级别划分
    time_t m_trialRefTime;//数据刷新时间
    //--------试炼场活动--------end-------
private:
    void sortSpeActivityArr();
    
    //进入游戏的弹窗
    vector<string> mOpenViewConfig;
    void showEnterViewByType(string popup);
};

class SocksExInfo;
class ChristmasActController : public CCObject{
public:
    static ChristmasActController *getInstance();
    static void purgeData();
    ChristmasActController();
    ~ChristmasActController();
    
    //袜子兑换数据
    int m_exchangeTime;//兑换刷新时间
    int m_level;//当前已经兑换了第几档
    string m_socksMsg;//获得袜子的信息
    map<int, SocksExInfo> m_socExMap;
    //end
    
    //14天登陆奖励数据
    int m_loginDay;
    int m_isRdLoginDay;//0 今天没领 1今天已经领了
    map<int, CCSafeObject<CCArray>> m_loginDayMap;
    int m_Today;
    bool m_isNewDay;
    //end
    
    //雪球数据
    double m_refreshTime;//恢复一个雪球的时间戳
    int m_maxThrowCount;//一天最多扔多少雪球
    int m_todayThrowCount;//当天已经扔过多少雪球
    int m_curLevel;//当前雪球等级
    int m_goldCostRecover;//恢复全部雪球需要的金币数量。
    int m_expArr[5];//资源扔雪球获得的相应的经验值
    int m_goldThrowMultiply;//用金币扔雪球获得的经验倍数
    int m_goldCostThrow;//花费金币扔雪球消耗的金币数
    int m_resourceType;//扔雪球所耗费的资源种类
    int m_resourceNum;//扔雪球所耗费的资源的数量
    long m_remainGold;//玩家金币数量
    int m_exp;//玩家当前经验值
    int m_isInit;
    int m_free;//1代表免费  0代表不免费
    //end 雪球数据
    
    map<string, int> m_actTime;
    
    int checkActState();
    int getStartTime();//获取活动开始时间
    int getSnowTime();//获取打雪人活动开始时间
    int getEndTime();//获取活动结束时间
    
    void startGetActTime();
    void endGetActTime(CCDictionary* param);

    void startGetExchangeData(int type=0);//初始化数据
    void endGetExchangeData(CCDictionary *dict, int type=0);
    
    void startExchange(int eid);//兑换
    void endExchange(CCDictionary *dict);
    
    void pushSocksMsg(CCDictionary *dict);//袜子信息更新
    
    void startGetLogin14Day();
    void endGetLogin14Day(CCDictionary *dict);
    void startGetLogin14RD();
    void endGetLogin14RD(CCDictionary *dict);
    
    void startGetSnowActData(int isInit=0);
    void endGetSnowActData(CCDictionary* param);
};

class SocksExInfo : public cocos2d::CCObject {
public:
    SocksExInfo(CCDictionary* dict);
    SocksExInfo():
    eId(0)
    ,needNum(0)
    ,rewardId("")
    ,rewardArr(NULL)
    {};
    
    int eId;
    int needNum;
    string rewardId;
    CCSafeObject<CCArray> rewardArr;
};

class TimeExInfo: public cocos2d::CCObject{
public:
    TimeExInfo(CCArray* arr, int i);
    TimeExInfo():
    eId(0)
    , rewardArr(NULL)
    {};
    
    int eId;
    CCSafeObject<CCArray> rewardArr;
    
};

class DailyRwdInfo: public cocos2d::CCObject{
public:
    DailyRwdInfo(CCDictionary* dict);
    DailyRwdInfo():
    day(0)
    , showDay(0)
    , multiple(0)
    , vipMultiple(0)
    , vipReward(NULL)
    , reward(NULL)
    {};
    
    int day;
    int showDay;
    int multiple;
    int vipMultiple;
    CCSafeObject<CCArray> reward;
    CCSafeObject<CCArray> vipReward;
};

class PortActController: public CCObject{
public:
    static PortActController *getInstance();
    static void purgeData();
    PortActController();
    ~PortActController();
    
    //在线时长兑换数据
    //等级的划分时间
    vector<int> m_levelTime;
    //上一次领奖的级别的时长最大值
    long m_lastRwdTime;
    //更新时大本等级
    int m_castleLevel;
    //当前在线时长
    long m_timeOnline;
    //前台开始计时时间
    long m_startTime;
    map<int, TimeExInfo> m_timeExMap;
    //end
    
    //新在线时长数据
    //下次可领奖时间
    time_t m_nextRewardTime;
    //奖励信息
    string m_rewardInfo;
    //开关
    bool m_isNewTimeRwd;
    
    //每日签到
    int m_loginDay;
    int m_isRdLoginDay;//0 今天没领 1今天已经领了
    int m_isVipRdLoginDay;//vip 0 今天没领 1今天已经领了
    int m_Today;
    bool m_isNewDay;
    int m_inCnt;//进页面次数
//    map<int, CCSafeObject<CCArray>> m_loginDayMap;
    map<int, DailyRwdInfo> m_loginDayMap;
    //end
    
    //新手7日累计签到
    int m_newLoginDay;//累计天数
    int m_isGetNewRD;//0 今天没领 1今天已经领了
    map<int, CCSafeObject<CCArray>> m_newPalyerRDMap;
    
    //根据时间获取等级
    int getLvByTime(long time);
    //获取当前最大可兑换等级
    int getMaxRwdLv();
    //判断是否是新的一天
    bool isNewDay();
    //解析奖励的时间点
    void parseTimeDiv(CCDictionary* dict);
    
    //初始化数据
    void startGetExchangeData(int type = 0);
    void endGetExchangeData(CCDictionary *dict, int type = 0);
    
    //兑换
    void startExchange(int eId);
    void endExchange(CCDictionary *dict);
    
    //每日签到初始化
    void startGetDailyRwdData();
    void endGetDailyRwdData(CCDictionary* dict);
    
    //新手7日签到初始化
    void initNewPlayerDailyRwdData(CCDictionary* dict);
    void startGetNewPlayerRD(string key="");
    void endGetNewPlayerRD(CCDictionary* dict);
    
    //签到
    void startGetCheckInRwd(bool isVip,int num=0,int newNum=0,int oldNum=0);
    void endGetCheckInRwd(CCDictionary* dict, bool isVip);
    
    //奖励领取特效
    void flyReward(CCArray* arr, bool isVip);
    void flyToolReward(CCArray* arr, bool isVip);
    void aniComplete(CCObject* node);
    void removeLayerColor(CCObject* layerColor);
    void showRwdParticle();
    
    //全部奖励领取完是否第一次进当前页面
    bool isLastDayFirstIn();
    
    //新的领取在线时长奖励方法
    void startTimeRwd();
    void endTimeRwd(CCDictionary*);
    //判断是否可以领取奖励
    bool isCanRwd();
    //增加推送
    void addPush();
    
    bool canPopNewRDView();
};



//
//累计充值回馈
//
struct RepayRewardStruct1{
    int type;
    std::string itemid;
    int count;
    int orderid;
};
struct RepayRewardStruct{
    int point;
    vector<RepayRewardStruct1> rewards;
};
class RepayController:public CCObject{
public:
    RepayController();
    ~RepayController();
    static RepayController* getInstance();
    static bool sortRepayItem(const RepayRewardStruct& itemId1,const RepayRewardStruct& itemId2);
    void testInfoInit();
    void initRepayInfo(CCDictionary *ccDict);
    vector<RepayRewardStruct> repayRewards;
    int getLVIndexByPoint(int point);
    bool isBegin();
    void addPoint(int addPoint);
    void resetChangeLV();
private:
    CC_SYNTHESIZE_READONLY(int, m_oldPayPoint, OldPayPoint);
    CC_SYNTHESIZE_READONLY(int, m_newPayPoint, NewPayPoint);
    CC_SYNTHESIZE_READONLY(double, m_startTime, StartTime);
    CC_SYNTHESIZE_READONLY(double, m_endTime, EndTime);
    CC_SYNTHESIZE_READONLY(bool, m_changeLV, ChangeLV);
};

#endif /* defined(__IF__ActivityController__) */
