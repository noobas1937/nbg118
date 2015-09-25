//
//  AchievementInfo.h
//  IF
//
//  Created by ganxiaohua on 13-12-23.
//
//

#ifndef __IF__AchievementInfo__
#define __IF__AchievementInfo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"

enum AchievementState{
    ACHIEVEMENT_NORMAL,
    ACHIEVEMENT_COMPELETE,
    ACHIEVEMENT_REWARD_GET,
};

class AchievementInfo{
public:
    AchievementInfo() :
    groupId("")
    , currentItemId("")
    , totalLevel(1)
    , currentLevel(0)
    , totalNum(1)
    , currentNum(0)
    , state(ACHIEVEMENT_NORMAL)
    , isVisible(false)
    , reward(NULL)
    , medalId("")
    {};
    ~AchievementInfo(){
        if(reward){
            reward->removeAllObjects();
            reward->release();
        }
    };
    std::string groupId;//系列的id
    std::string currentItemId;//当前等级下的id

    int state;
    int totalLevel;//最大星级
    int currentLevel;//当前星级
    double totalNum;//完成当前星级需要的数目
    double currentNum;//当前完成的数目
    int showLevel;//显示等级
    int showOrder;//显示顺序，低的先显示
    int series;//成就类别
    std::string medalId;//对应勋章id
    
    std::string name;
    std::string description;
    std::string iconMain;//
    std::string iconAdditional;//
    cocos2d::CCArray *reward;
    bool isVisible;
    double start;
    double end;
    void initData(std::string itemId);
    void initData(cocos2d::CCDictionary *dict);
    void updateData(cocos2d::CCDictionary *dict);//刷新整个数据。
    void setXmlData(std::string itemId);
    void setXmlData(cocos2d::CCDictionary *dict);
    void updateNumData(cocos2d::CCDictionary *dict);//刷新当前数目
    bool refreshVisibleFlag();
};

#endif /* defined(__IF__AchievementInfo__) */
