//
//  QuestRewardCommand.h
//  IF
//
//  Created by ganxiaohua on 13-11-4.
//
//

#ifndef __IF__QuestRewardCommand__
#define __IF__QuestRewardCommand__
#include "CommandBase.h"
#define QUEST_REWARD_GET "task.reward.get"
#define QUEST_FS_TASK "praise.receive"
#define QUEST_STAGE_RD_COMMAND "quest.stage.rd"
#define GET_RD_INFO_COMMAND "get.rd.info"

class QuestRewardCommand: public CommandBase {
public:
    QuestRewardCommand(std::string itemId, bool isMain=false, bool showTip = true) : CommandBase(QUEST_REWARD_GET){
        m_id = itemId;
        m_isMain = isMain;
        m_showTip = showTip;
        putParam("id", CCString::create(itemId));
        CCLOG("%s", m_id.c_str());
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;
    bool m_isMain;
    bool m_showTip;
};

class FSTaskCommand: public CommandBase {
public:
    FSTaskCommand(int value):
    CommandBase(QUEST_FS_TASK){
        putParam("platforom", CCInteger::create(value));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

//领取阶段任务奖励
class QuestStageRDCommand: public CommandBase {
public:
    QuestStageRDCommand():
    CommandBase(QUEST_STAGE_RD_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

//获取奖励信息
class GetRdCommand: public CommandBase {
public:
    GetRdCommand(int rewardId):
    CommandBase(GET_RD_INFO_COMMAND){
        putParam("rewardId", CCInteger::create(rewardId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__QuestRewardCommand__) */
