//
//  CSALeaveCommand.h
//  IF
//
//  Created by ganxiaohua on 15/6/10.
//
//

#ifndef __IF__CSALeaveCommand__
#define __IF__CSALeaveCommand__

#include "CommandBase.h"

#define CROSS_SERVER_LEAVE "ckf.leave"

class CSALeaveCommand: public CommandBase {
public:
    CSALeaveCommand() : CommandBase(CROSS_SERVER_LEAVE){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
#endif /* defined(__IF__CSALeaveCommand__) */
