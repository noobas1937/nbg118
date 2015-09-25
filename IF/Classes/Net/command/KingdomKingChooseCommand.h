//
//  KingdomKingChooseCommand.h
//  IF
//
//  Created by ganxiaohua on 14-12-24.
//
//

#ifndef __IF__KingdomKingChooseCommand__
#define __IF__KingdomKingChooseCommand__

#include "CommandBase.h"
#define KINGDOM_KING_CHOOSE "kingdom.king.choose"

class KingdomKingChooseCommand: public CommandBase {
public:
    KingdomKingChooseCommand(string uid) : CommandBase(KINGDOM_KING_CHOOSE){
        putParam("uid", CCString::create(uid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__KingdomKingChooseCommand__) */
