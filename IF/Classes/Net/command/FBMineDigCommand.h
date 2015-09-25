//
//  FBMineDigCommand.h
//  IF
//
//  Created by ganxiaohua on 14-10-22.
//
//

#ifndef __IF__FBMineDigCommand__
#define __IF__FBMineDigCommand__

#include "CommandBase.h"

#define FB_MINE_DIG_COMMAND "mine.dig"

class FBMineDigCommand: public CommandBase {
public:
    FBMineDigCommand() : CommandBase(FB_MINE_DIG_COMMAND){
       
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__FBMineDigCommand__) */
