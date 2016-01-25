//
//  GCMRewardController.h
//  IF
//
//  Created by fubin on 13-12-25.
//
//

#ifndef IF_GCMRewardController_h
#define IF_GCMRewardController_h

#include "CommonInclude.h"
#include "GCMRewardInfo.h"

class GCMRewardController : public CCObject{
public:
    static GCMRewardController* getInstance();
    static void purgeData();
    GCMRewardController();
    ~GCMRewardController();
    
    void initGCMRewardData(CCDictionary* dict);
    bool sendReward(string actId);
    CCArray* retReward(CCDictionary* dict);
    CCArray* retReward2(CCArray* arr, bool flyAni = false);
    void flyReward(CCArray* arr);
    void flyToolReward(CCArray* arr);
    CCArray* getFlyArr(CCArray* arr);
    std::string getPicByType(std::string type, int value);
    std::vector<GCMRewardInfo> m_rewardPool;
    bool isFirstOpen;
    
    //fusheng begin
    void retReward3(Array* arr,float time = 0,bool move = false, Point fromPt = ccp(0, 0));//获取奖励move为是否从某点放大移动到startPt
    void flyRewardFromPointToPoint(Array* arr,float time = 0,bool move = false, Point fromPt = ccp(0, 0));//奖励从屏幕上飞到相应的位置
    
    void flyResAni(CCObject *obj);
    //fusheng end
    
private:
    void aniComplete(CCObject* node);
};
#endif
