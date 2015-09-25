//
//  KingdomExecCommand.h
//  IF
//
//  Created by ganxiaohua on 14-12-11.
//
//

#ifndef __IF__KingdomExecCommand__
#define __IF__KingdomExecCommand__

#include "CommandBase.h"
#define KINGDOM_KING_EXEC "kingdom.king.exec"

enum RightsType{
    APPOINT,
    FIRE,
    CHANGE_COUNTRY_FLAG,
    CHANGE_KINGDOM_NAME,
    CHANGE_RESOURCE_FLUSH,
    CHANGE_PALACE,
};

class KingdomExecCommand: public CommandBase {
public:
    KingdomExecCommand() : CommandBase(KINGDOM_KING_EXEC){
        //putParam("lastEventTime", CCString::create(lastEventTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__KingdomExecCommand__) */
