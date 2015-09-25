//
//  AllianceLeaderReplaceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-12-4.
//
//

#ifndef __IF__AllianceLeaderReplaceCommand__
#define __IF__AllianceLeaderReplaceCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_REPLACE "al.leader.replace"

class AllianceLeaderReplaceCommand: public CommandBase {
public:
    AllianceLeaderReplaceCommand() : CommandBase(ALLIANCE_AL_REPLACE){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceLeaderReplaceCommand__) */
