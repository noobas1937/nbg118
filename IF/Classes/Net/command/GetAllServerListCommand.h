//
//  GetAllServerListCommand.h
//  IF
//
//  Created by 李锐奇 on 14-10-13.
//
//

#ifndef __IF__GetAllServerListCommand__
#define __IF__GetAllServerListCommand__

#include "CommandBase.h"
#define CROSS_SERVER_LS "cross.server.ls"

class GetAllServerListCommand: public CommandBase {
public:
    GetAllServerListCommand():
    CommandBase(CROSS_SERVER_LS){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__GetAllServerListCommand__) */
