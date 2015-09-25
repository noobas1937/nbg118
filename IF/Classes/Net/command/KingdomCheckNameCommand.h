//
//  KingdomCheckNameCommand.h
//  IF
//
//  Created by ganxiaohua on 14-12-11.
//
//

#ifndef __IF__KingdomCheckNameCommand__
#define __IF__KingdomCheckNameCommand__

#include "CommandBase.h"
#define KINGDOM_CHECK_NAME "kingdom.check.name"

class KingdomCheckNameCommand: public CommandBase {
public:
    KingdomCheckNameCommand() : CommandBase(KINGDOM_CHECK_NAME){
        //putParam("lastEventTime", CCString::create(lastEventTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__KingdomCheckNameCommand__) */
