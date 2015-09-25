//
//  GetPowerInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 14-5-20.
//
//

#ifndef __IF__GetPowerInfoCommand__
#define __IF__GetPowerInfoCommand__

#include "CommandBase.h"
#define GET_POWER_INFO "get.power.info"

class GetPowerInfoCommand: public CommandBase {
public:
    GetPowerInfoCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__GetPowerInfoCommand__) */
