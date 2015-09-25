//
//  MineHelpInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 14-11-19.
//
//

#ifndef __IF__MineHelpInfoCommand__
#define __IF__MineHelpInfoCommand__

#include "CommandBase.h"

#define FB_MINE_HELP_COMMAND "mine.help.info"

class MineHelpInfoCommand: public CommandBase {
public:
    MineHelpInfoCommand() : CommandBase(FB_MINE_HELP_COMMAND){
        //putParam("lastEventTime", CCString::create(lastEventTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__MineHelpInfoCommand__) */
