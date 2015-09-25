//
//  QuestSubmitCommand.h
//  IF
//
//  Created by ganxiaohua on 13-11-5.
//
//

#ifndef __IF__QuestSubmitCommand__
#define __IF__QuestSubmitCommand__

#include "CommandBase.h"
#define QUEST_COMMAND_SUBMIT_KEY "task.submit"

class QuestSubmitCommand: public CommandBase {
public:
    QuestSubmitCommand(CCString* m_id);
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    CCString* m_id;
};
#endif /* defined(__IF__QuestSubmitCommand__) */
