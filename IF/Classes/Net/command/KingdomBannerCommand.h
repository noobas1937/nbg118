//
//  KingdomBannerCommand.h
//  IF
//
//  Created by ganxiaohua on 14-12-24.
//
//

#ifndef __IF__KingdomBannerCommand__
#define __IF__KingdomBannerCommand__

#include "CommandBase.h"
#define KINGDOM_KING_BANNER "kingdom.banner"

class KingdomBannerCommand: public CommandBase {
public:
    KingdomBannerCommand() : CommandBase(KINGDOM_KING_BANNER){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__KingdomBannerCommand__) */
