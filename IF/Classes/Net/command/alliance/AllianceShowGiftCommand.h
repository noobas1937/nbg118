//
//  AllianceShowGiftCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-10.
//
//

#ifndef __IF__AllianceShowGiftCommand__
#define __IF__AllianceShowGiftCommand__

#include "CommandBase.h"
#define SHOW_GIFT "al.showgift"

class AllianceShowGiftCommand: public CommandBase {
public:
    AllianceShowGiftCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__AllianceShowGiftCommand__) */
