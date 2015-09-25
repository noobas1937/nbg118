//
//  ActivityScoreHistoryCommand.h
//  IF
//
//  Created by ganxiaohua on 14-9-3.
//
//

#ifndef __IF__ActivityScoreHistoryCommand__
#define __IF__ActivityScoreHistoryCommand__

#include "CommandBase.h"

#define GET_ACTIVITY_SCORE_HISTORY "score.history"
#define GET_ACTIVITY_LAST_HISTORY "score.top.history"

class ActivityScoreHistoryCommand: public CommandBase {
public:
    ActivityScoreHistoryCommand() : CommandBase(GET_ACTIVITY_SCORE_HISTORY){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

class ActivityLastHistoryCommand: public CommandBase {
public:
    ActivityLastHistoryCommand() : CommandBase(GET_ACTIVITY_LAST_HISTORY){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__ActivityScoreHistoryCommand__) */
