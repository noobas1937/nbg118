//
//  FBGetGlodMineCommand.h
//  IF
//
//  Created by ganxiaohua on 14-10-21.
//
//

#ifndef __IF__FBGetGlodMineCommand__
#define __IF__FBGetGlodMineCommand__

#include "CommandBase.h"

#define FB_GET_GOLD_COMMAND "mine.bind"

class FBGetGlodMineCommand: public CommandBase {
public:
    FBGetGlodMineCommand() : CommandBase(FB_GET_GOLD_COMMAND){
        //putParam("lastEventTime", CCString::create(lastEventTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__FBGetGlodMineCommand__) */
