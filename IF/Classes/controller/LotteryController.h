//
//  LotteryController.h
//  IF
//
//  Created by chenfubi on 15/1/23.
//
//

#ifndef __IF__LotteryController__
#define __IF__LotteryController__

#include "CommonInclude.h"

#define LOTTERYACTVIEWCMD "LOTTERYACTVIEWCMD"
#define LOTTERYACTVIEWCMD2 "LOTTERYACTVIEWCMD2"
#define LOTTERYACTVIEW_SEND "LOTTERYACTVIEW_SEND"
#define LOTTERYACTVIEW_COM "LOTTERYACTVIEW_COM"
#define LOTTERYACTVIEW_GET "LOTTERYACTVIEW_GET"
#define LOTTERYACTVIEW_UPDATE "LOTTERYACTVIEW_UPDATE"
#define LOTTERYROTATEVIEW "LOTTERYROTATEVIEW"
#define LOTTERYACT2VIEW_NOTIFY "LOTTERYACT2VIEW_NOTIFY"


#define LOTTERY_REWARD_INFO "lottery.reward.info"
#define LotteryAct2ShowView_PREVIEW_REWARD "lotteryAct2Show_preview_reward"
#define LotteryAct2ShowView_PREVIEW_REWARD0 "lotteryAct2Show_preview_reward0"
#define LOTTERYACT2_PREVIEW_REWARD "lotteryact_preview"

class LotteryActInfo : public CCObject
{
public:
    virtual bool init(){return true;};
    CREATE_FUNC(LotteryActInfo);
    LotteryActInfo():superMode(0),type(1),lotteryCnt(0),hasResetReward(true){};
    virtual ~LotteryActInfo(){};
public:
    int superMode;                      //是否超级模式 0：非超级模式，1：超级模式
    int type;                           //处于第几层抽奖模式 1：第一层；2：第二层
    int lotteryCnt;                     //外层抽奖次数
    int lotteryCnt2;                    //内层抽奖次数
    
    //第二层相关数据
    map<int, vector<string>> reward2Map;        //第二层9个奖品的位置及开启信息
    map<int, vector<string>> rewardInfo;        //第二层奖品信息
    vector<string> rewardSort;                  //第二层奖品顺序信息
    bool hasResetReward;
    
};

class LotteryController : public CCObject
{
public:
    static void addCountInfo(CCNode* pNode,std::string itemid);
	static LotteryController * shared();
	void purge();
    
    void parseLotteryData(CCDictionary* dict);
    void parseLotteryData2(CCDictionary* dict);
    int getStepByPos(int position);
    bool getStep(string itemId,std::string value, vector<int> &pos);
    void setReward2Map(int position, vector<string> reward);
    void getReward2Map(int position, vector<string> &reward);
    void setRewardInfo(int position, vector<string> reward);
    void getRewardInfo(int position, vector<string> &reward);
    int getLotteryCost2();
    int getLotteryCost1();
    CCArray* getLotteryReward();
    string getLotteryIcon(int itemId);
    CCLabelIFBMFont* getLabelBMfont(int itemId);
    void getItemDes(int position, vector<string> &vecStr, bool desAniInfo = false);
    string getLotteryName(int itemId, int itemCnt);
    bool isMultiReward(string rewardId);
    void lotteryActCmdBack(CCObject* pObj);
    void lotteryAct2CmdBack(CCObject* pObj);
    
    CC_SYNTHESIZE_READONLY(float, m_rate, Rate);
    CC_SYNTHESIZE_READONLY(float, m_rate2, Rate2);
    CC_SYNTHESIZE(int, m_openFlag, OpenFlag);
    CC_SYNTHESIZE(int, m_openFlag2, OpenFlag2);
    CC_SYNTHESIZE(int, m_openFlag_ko, OpenFlag2_ko);
    CC_SYNTHESIZE(double, m_updateTime, UpdateTime);
    
    CC_SYNTHESIZE(bool, m_bSendCMD, SendCMD);
private:
	LotteryController();
	virtual ~LotteryController();
	LotteryController(const LotteryController &);
    LotteryController &operator=(const LotteryController &);
    bool isFirstTime();
    string getIcon(string itemId);
public:
    LotteryActInfo lotteryInfo;
    
private:
    CCSafeObject<CCArray> m_nLotteryReward;
    CCSafeObject<CCArray> m_sLotteryReward;
    
    map<int, int> m_lotteryCost2;//第二层花费
    
    int m_nLotteryCost;
    int m_sLotteryCost;
};

#endif /* defined(__IF__LotteryController__) */
