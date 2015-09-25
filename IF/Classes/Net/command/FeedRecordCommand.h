//
//  FeedRecordCommand.h
//  IF
//
//  Created by ganxiaohua on 15/2/5.
//
//

#ifndef __IF__FeedRecordCommand__
#define __IF__FeedRecordCommand__

#include "CommandBase.h"
#define Feed_RECORD "feed.record"

class FeedRecordCommand: public CommandBase {
public:
    FeedRecordCommand(string key) : CommandBase(Feed_RECORD){
        putParam("key", CCString::create(key));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__FeedRecordCommand__) */
