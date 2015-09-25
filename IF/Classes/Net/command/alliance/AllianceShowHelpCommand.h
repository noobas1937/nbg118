//
//  AllianceShowHelpCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-16.
//
//

#ifndef __IF__AllianceShowHelpCommand__
#define __IF__AllianceShowHelpCommand__
#include "CommandBase.h"

#define ALLIANCE_AL_SHOWHELP "al.showhelp"

class AllianceShowHelpCommand: public CommandBase {
public:
    AllianceShowHelpCommand() : CommandBase(ALLIANCE_AL_SHOWHELP){
        
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceShowHelpCommand__) */
