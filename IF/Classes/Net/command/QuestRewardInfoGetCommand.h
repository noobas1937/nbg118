//
//  QuestRewardInfoGetCommand.h
//  IF
//
//  Created by 李锐奇 on 14-3-25.
//
//

#ifndef __IF__QuestRewardInfoGetCommand__
#define __IF__QuestRewardInfoGetCommand__

#include "CommandBase.h"

#define QUEST_REWRD_INFO_GET_COMMAND "task.reward.info"
#define QUEST_REWARD_DATA_BACK "questRewardDataBack"

class QuestRewardInfoGetCommand: public CommandBase {
public:
    QuestRewardInfoGetCommand(std::string questId) : CommandBase(QUEST_REWRD_INFO_GET_COMMAND){
        putParam("id", CCString::create(questId));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__QuestRewardInfoGetCommand__) */
