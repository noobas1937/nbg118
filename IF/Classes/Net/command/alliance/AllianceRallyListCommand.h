//
//  AllianceRallyListCommand.h
//  IF
//
//  Created by ganxiaohua on 14-8-13.
//
//

#ifndef __IF__AllianceRallyListCommand__
#define __IF__AllianceRallyListCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_REF_LIST "al.ref.list"

class AllianceRallyListCommand: public CommandBase {
public:
    AllianceRallyListCommand() : CommandBase(ALLIANCE_AL_REF_LIST){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceRallyListCommand__) */
