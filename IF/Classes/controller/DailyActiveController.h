//
//  DailyActiveController.h
//  IF
//
//  Created by chenfubi on 15/4/10.
//
//

#ifndef __IF__DailyActiveController__
#define __IF__DailyActiveController__

#include "CommonInclude.h"
#include "DailyActiveBoxInfo.h"
#include "DailyActiveItemInfo.h"

#define DAILYQEUST_INIT "DAILYQEUST_INIT"
#define DAILYQEUST_BOX_INIT "DAILYQEUST_BOX_INIT"
#define DAILYQEUST_REWARD "DAILYQEUST_REWARD"
class DailyActiveController : public CCObject
{
public:
	static DailyActiveController * shared();
	void purge();

private:
	DailyActiveController();
	virtual ~DailyActiveController();
	DailyActiveController(const DailyActiveController &);
    DailyActiveController &operator=(const DailyActiveController &);
    
public:
    void getDataFormServer();
    void init(CCDictionary* dict);
    void initItemInfo(CCArray* arr);
    void initBoxInfo(CCArray* arr);
    void updateItemInfo(string itemId, CCDictionary* dict, bool setIsNew = false);
    void updateBoxInfo(CCDictionary* dict);
    void updateActivePoint();
    void getBoxReward(string itemId);
    CCArray* getSortItemArr();
    void goActTarget(string itemId);
    void resetAllData();
    bool canGetReward();
    bool needShow();
    int getTotalPoint();
    void updateCurReward();
    
private:
    void sortArray(CCArray* arr);
    
public:
    map<string, DailyActiveItemInfo> m_itemInfo;
//    CCSafeObject<DailyActiveBoxInfo> m_boxInfo;
    map<int, DailyActiveBoxInfo> m_boxInfo;
    
    CC_SYNTHESIZE(int, m_activePoints, ActivePoints);
    CC_SYNTHESIZE(double, m_tomorrowTime, TomorrowTime);
    CC_SYNTHESIZE(int, m_dailyActiveOpenFlag, DailyActiveOpenFlag);
    CC_SYNTHESIZE(bool, m_hasUnlockItem, HasUnlockItem);
    CC_SYNTHESIZE_READONLY(bool, m_hasInit, HasInit);
    CC_SYNTHESIZE(int, m_preActPoint, PreActPoint);
    
private:
    int m_perReward;
    int m_curReward;
    
};

//CLASS DailyActiveRewardCmd
#include "CommandBase.h"
#define DAILYACTIVEREWARDCMD "daily.quest.reward"
class DailyActiveRewardCmd : public CommandBase
{
public:
    DailyActiveRewardCmd():CommandBase(DAILYACTIVEREWARDCMD)
    {
        
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//CLASS DailyAcitveInitCmd
#define DAILYACTIVEINITCMD "daily.quest.ls"
class DailyAcitveInitCmd : public CommandBase
{
public:
    DailyAcitveInitCmd():CommandBase(DAILYACTIVEINITCMD)
    {
        
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//CLASS DailyActiveFirstCmd
#define DAILYACTIVEFIRST "questReward.first"
class DailyActiveFirstCmd : public CommandBase
{
public:
    DailyActiveFirstCmd():CommandBase(DAILYACTIVEFIRST)
    {
        
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__DailyActiveController__) */
