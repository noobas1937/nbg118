//
//  ActivityEventObj.h
//  IF
//
//  Created by ganxiaohua on 15-1-5.
//
//

#ifndef __IF__ActivityEventObj__
#define __IF__ActivityEventObj__

#include "CommonInclude.h"

enum ActivityEventType{
    SUPER_LORD = 1,
    MONTH_CARD,
};

class ActivityEventObj : public CCObject{
public:
    CREATE_FUNC(ActivityEventObj);
    void parse(CCDictionary *dict);
    CCDictionary* toDic();
    ActivityEventObj():id(string()),startTime(0),endTime(0),name(string()),desc(string()),type(0),sort(0),desc_info(string()),activityid(string()),Advertise_pic(string()), exchange(string()), story(string()), num(string()), popup(-1){};
    ~ActivityEventObj(){};
    time_t startTime;
    time_t endTime;
    std::string id;
    std::string name;
    std::string desc;
    std::string descDia;
    std::string desc_info;
    std::string activityid;
    std::string Advertise_pic;
    std::string exchange;
    std::string story;
    std::string num;
    int popup;// 用于标识登录游戏时是否弹该活动的ActivityPopupView: 0 不弹面板；-1 可以弹面板
    int type;// 1 最强领主、2 月卡 3、联盟活动 4 boss  5 、兑换 、 6、兑换lua 7、 跨服活动 8、巨龙战役
    int sort;
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_reward, Reward);//奖励
private:
    virtual bool init(){return true;};
};
#endif /* defined(__IF__ActivityEventObj__) */
