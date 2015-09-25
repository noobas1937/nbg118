//
//  GetActivityDetailCommand.h
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#ifndef __IF__GetActivityDetailCommand__
#define __IF__GetActivityDetailCommand__

#include "CommandBase.h"

#define GET_ACTIVITY_DETAIL "score.info"

class GetActivityDetailCommand: public CommandBase {
public:
    GetActivityDetailCommand() : CommandBase(GET_ACTIVITY_DETAIL){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__GetActivityDetailCommand__) */
