//
//  AllianceHelpAllCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-18.
//
//

#ifndef __IF__AllianceHelpAllCommand__
#define __IF__AllianceHelpAllCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_HELP_ALL "al.helpall"

class AllianceHelpAllCommand: public CommandBase {
public:
    AllianceHelpAllCommand() : CommandBase(ALLIANCE_AL_HELP_ALL){
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:

};
#endif /* defined(__IF__AllianceHelpAllCommand__) */
