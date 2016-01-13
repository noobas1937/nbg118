//
//  QuestInfo.h
//  IF
//
//  Created by ganxiaohua on 13-11-4.
//
//

#ifndef __IF__QuestInfo__
#define __IF__QuestInfo__

#include "CommonInclude.h"

class QuestInfo : public cocos2d::CCObject {
public:
    CREATE_FUNC(QuestInfo);
    ~QuestInfo(){if(reward)reward->release();};
    virtual bool init(){
        reward = NULL;
        currPhase = 0;
        maxPhase = 0;
        return true;
    };
    void parse(CCDictionary *dict);
    void parseServerDic(CCDictionary *dict);
    std::string itemId;
    int state;
    cocos2d::CCArray *reward;
    int recomment;
    std::string name;
    std::string description;
    std::string target;
    std::string guide;
    std::string icon1;
    std::string icon2;
    std::string rewardshow;
    int type;//  3为跨服任务
    int type1;
    int maxtype;//用来解析max字段
    int level;
    int order;
    int order1;
    int order2;
    int maxStar;
    int curValue;
    int maxValue;
    int currPhase;
    int maxPhase;
    bool haveOpen;
    int go;
    int gotype;
    int isShow;
};

#endif /* defined(__IF__QuestInfo__) */
