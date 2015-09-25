//
//  DismissAllianceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-9.
//
//

#ifndef __IF__DismissAllianceCommand__
#define __IF__DismissAllianceCommand__

#include "CommandBase.h"
#define ALLIANCE_AL_DISMISS "al.dismiss"

class DismissAllianceCommand: public CommandBase {
public:
    DismissAllianceCommand() : CommandBase(ALLIANCE_AL_DISMISS){
        
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__DismissAllianceCommand__) */
