//
//  QuestSubmitCommand.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-5.
//
//

#include "QuestSubmitCommand.h"

QuestSubmitCommand::QuestSubmitCommand(CCString* id)
: CommandBase(QUEST_COMMAND_SUBMIT_KEY),m_id(id)
{
    putParam("taskId", id);
}

bool QuestSubmitCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEST_COMMAND_SUBMIT_KEY) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    return true;
}