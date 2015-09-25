//
//  TestFeedBackCommand.h
//  IF
//
//  Created by chenfubi on 15/2/10.
//
//

#ifndef __IF__TestFeedBackCommand__
#define __IF__TestFeedBackCommand__

#include "CommandBase.h"

#define TESTFEEDBACK_GET_CMD "suggestion.get"
#define TESTFEEDBACK_SAVE_CMD "suggestion.save"

class TestFeedBackCommand : public CommandBase
{
public:
    TestFeedBackCommand():CommandBase(TESTFEEDBACK_GET_CMD)
    {
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class TestFeedBackSaveCommand : public CommandBase
{
public:
    TestFeedBackSaveCommand(string version, int index, string suggestion, string name, int serverId):CommandBase(TESTFEEDBACK_SAVE_CMD)
    {
        putParam("version", ccs(version));
        putParam("index", CCInteger::create(index));
        putParam("suggestion", ccs(suggestion));
        putParam("name", ccs(name));
        putParam("serverId", CCInteger::create(serverId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__TestFeedBackCommand__) */
