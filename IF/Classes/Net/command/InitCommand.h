//
//  InitCommand.h
//  IF
//
//  Created by zhaohf on 13-12-11.
//
//

#ifndef __IF__InitCommand__
#define __IF__InitCommand__

#include "cocos2d.h"
#include "CommandBase.h"

#define INIT_COMMAND "login.init"

/// 初始化数据下发
class InitCommand:public CommandBase {
    
public:
    InitCommand() : CommandBase(INIT_COMMAND){
        }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__InitCommand__) */
