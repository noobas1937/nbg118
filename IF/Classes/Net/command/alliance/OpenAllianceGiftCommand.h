//
//  OpenAllianceGiftCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-10.
//
//

#ifndef __IF__OpenAllianceGiftCommand__
#define __IF__OpenAllianceGiftCommand__

#include "CommandBase.h"

#define OPEN_GIFT "al.opengift"

class OpenAllianceGiftCommand: public CommandBase {
public:
    OpenAllianceGiftCommand(std::string giftId);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_giftId;
};
#endif /* defined(__IF__OpenAllianceGiftCommand__) */
