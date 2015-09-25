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

class QuestRewardCommand: public CommandBase {
public:
    QuestRewardCommand(std::string itemId, bool isMain=false) : CommandBase(QUEST_REWARD_GET){
        m_id = itemId;
        m_isMain = isMain;
        putParam("id", CCString::create(itemId));
        CCLOG("%s", m_id.c_str());
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;
    bool m_isMain;
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

#endif /* defined(__IF__QuestRewardCommand__) */
