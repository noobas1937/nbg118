//
//  ClearAllianceGiftCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-10.
//
//

#ifndef __IF__ClearAllianceGiftCommand__
#define __IF__ClearAllianceGiftCommand__

#include "CommandBase.h"

#define CLEAR_GIFT "al.cleargift"

class ClearAllianceGiftCommand: public CommandBase {
public:
    ClearAllianceGiftCommand(std::string giftId);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_giftId;
};
#endif /* defined(__IF__ClearAllianceGiftCommand__) */
