//
//  KingdomOfficersCommand.h
//  IF
//
//  Created by ganxiaohua on 14-12-11.
//
//

#ifndef __IF__KingdomOfficersCommand__
#define __IF__KingdomOfficersCommand__

#include "CommandBase.h"
#define KINGDOM_OFFICERS "kingdom.officers"

class KingdomOfficersCommand: public CommandBase {
public:
    KingdomOfficersCommand() : CommandBase(KINGDOM_OFFICERS){
        //putParam("lastEventTime", CCString::create(lastEventTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__KingdomOfficersCommand__) */
