//
//  GetUserRankInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/4/10.
//
//

#ifndef __IF__GetUserRankInfoCommand__
#define __IF__GetUserRankInfoCommand__

#include "CommandBase.h"

#define MS_GET_USER_TOP_INFO "ms.get.user.top.info"

class GetUserRankInfoCommand: public CommandBase {
public:
    GetUserRankInfoCommand() : CommandBase(MS_GET_USER_TOP_INFO){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__GetUserRankInfoCommand__) */
