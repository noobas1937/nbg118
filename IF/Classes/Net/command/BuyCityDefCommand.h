//
//  BuyCityDefCommand.h
//  IF
//
//  Created by ganxiaohua on 14-12-8.
//
//

#ifndef __IF__BuyCityDefCommand__
#define __IF__BuyCityDefCommand__

#include "CommandBase.h"

#define BUY_CITY_DEF "buy.citydef"

class BuyCityDefCommand: public CommandBase {
public:
    BuyCityDefCommand() : CommandBase(BUY_CITY_DEF){
        //putParam("lastEventTime", CCString::create(lastEventTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__BuyCityDefCommand__) */
