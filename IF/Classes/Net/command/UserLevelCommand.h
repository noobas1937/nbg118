//
//  UserLevelCommand.h
//  IF
//
//  Created by wangzhenlei on 14-1-2.
//
//

#ifndef __IF__UserLevelCommand__
#define __IF__UserLevelCommand__

#include "CommandBase.h"
#define User_LV_COMMAND "user.lv"
#define User_LV_REWARD_COMMAND "levelUp.reward"
/*
 *用于矫正用户等级和经验，请求后台刷新
 */
class UserLevelCommand: public CommandBase {
public:
    UserLevelCommand():
    CommandBase(User_LV_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class UserLevelRewardCommand: public CommandBase {
public:
    UserLevelRewardCommand(int level):
    CommandBase(User_LV_REWARD_COMMAND){
        m_level = level;
        putParam("level", CCInteger::create(level));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_level;
};

#endif /* defined(__IF__UserLevelCommand__) */
