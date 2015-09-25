//
//  ActPopTimesCommand.h
//  IF
//
//  Created by xxrdsg on 15-7-9.
//
//

#ifndef __IF__ActPopTimesCommand__
#define __IF__ActPopTimesCommand__

#include "CommandBase.h"

#define ACT_POPUP "act.popup"

class ActPopTimesCommand: public CommandBase
{
public:
    ActPopTimesCommand(int times): CommandBase(ACT_POPUP)
    {
        putParam("times", CCInteger::create(times));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ActPopTimesCommand__) */
