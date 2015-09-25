//
//  QueueController.h
//  IF
//
//  Created by fubin on 14-2-19.
//
//

#ifndef IF_QueueController_h
#define IF_QueueController_h

#include "CommonInclude.h"
#include "QueueInfo.h"
#include "UIComponent.h"

#define BUILD_QUEUE_1ST 1101
#define BUILD_QUEUE_2ND 1102
#define BUILD_QUEUE_3RD 1103

class QueueController : public CCObject{
public:
    static QueueController* getInstance();
    static void purgeData();
    QueueController();
    ~QueueController();
    
    void initQueueData(CCDictionary* dict);
    void initMarchQueueData();
    
    int getQueueQidByKey(string key);
    int getQueueQidByUuid(string uuid);
    double getFinishTimeByKey(string key);
    
    void updateQueueInfo(int qid, double finishTime, int key);
    string addQueueInfo(CCDictionary* dict, int key=0);
    
    unsigned int getMarchQueueQid(const string& uuid);
    void updateMarchQueueInfo(const string& uuid);
    void addMarchQueueInfo(const string& uuid);
    
    void startOpenQueue(string uuid);
    void endOpenQueue(CCDictionary* dict);
    
    void startCancelQueue(int qid);
    void endCancelQueue(CCDictionary* dict);
    
    void openQueue(CCDictionary* dict);
    
    void rentQueue(int qid, int time, bool oneCmdQueue = false);
    
    void startCCDQueue(int qid, string itemUuid, bool isFinish=false, int isGold=0,string mark="",int from=0, string guideStep="",int itmCnt = 0);//from 建筑，练兵，恢复伤兵，科研 锻造
    void endCCDQueue(CCDictionary* dict);
    
    void startFinishQueue(int qid, bool isSend = true);
    void removeQueue(int qid);
    void removeQueueByType(int type);
    void endFinishQueue(CCDictionary* dict);
    void onEnterFrame(float dt);
    void collectSolider(int qid,int buildingKey);
    
    int canMakeItemByType(int type, int time_need);
    int getMinTimeQidByType(int type, string key="");
    int getCanRecQidByType(int type);
    std::string getKey(int itemId, int indx);
    int getItemId(string key);
    void pushQueueNotify(int type, int time, std::string body="");
    int getQueueNumByType(int type);
    bool checkMarchStatus();

    int getQID(int type);
    
    void autoHavestEquip(int qid, int buildingKey);
    void autoHavestMateTool(int qid, int buildingKey);
    void refreshMateQueue(int buildingKey);
    void endRefreshMateQueue(CCDictionary* dict);
    
    void ResetNoticeParam(int open, int sound);
    
    void initLoginFinishQueues(CCDictionary* dict);
    string getLoginFinishQueueUuId(int qType);
    map<string, string> loginFinishQueues;
    
    int m_buildType;
    WaitInterface* m_waitInterface;
    
    map<int, QueueInfo> m_buildQueueInfo;
    
    int ShowRecAllianceTime;
};

#endif
