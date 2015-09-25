//
//  AllianceActivityInfo.h
//  IF
//
//  Created by ganxiaohua on 15/4/1.
//
//

#ifndef __IF__AllianceActivityInfo__
#define __IF__AllianceActivityInfo__

#include "CommonInclude.h"

class AllianceActivityInfo:public CCObject
{
public:
    AllianceActivityInfo():activityST(0),activityET(0),siegeST(0),siegeET(0),round(0),posX(0),posY(0),nextRoundTime(0),monsterId(0),count(0),m_monsterInfo(NULL){};
    double activityST; //活动开始时间
    double activityET; //活动结束时间
    double siegeST; //开始迎战时间
    double siegeET; //刷怪结束时间
    int round;//当前是第几波
    int posX;//x坐标
    int posY;//y坐标
    double nextRoundTime;//下波怪的刷新时间
    int monsterId;//怪物ID
    int count;//怪物数量
    int userKill;//
    int userRank;//
    int allKill;//
    int allRank;//
    int state;// 活动状态 enum AllianceActivityState
    double rewardTime;//发奖的截止时间
    int userState;// 0 //本期未参加过活动  1 //本期在本盟已参加活动（不区分活动是否结束，只要参加了就是1）2 //本期在别的盟已经参加过活动了（不区分之前那个盟的活动是否结束，只要参加了就是2）
    
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_monsterInfo, MonsterInfo);//怪物信息
    
    void parse(CCDictionary* dict);
};
#endif /* defined(__IF__AllianceActivityInfo__) */
