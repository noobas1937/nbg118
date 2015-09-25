//
//  WinPointsUseCommand.h
//  IF
//
//  Created by ganxiaohua on 15/8/17.
//
//

#ifndef __IF__WinPointsUseCommand__
#define __IF__WinPointsUseCommand__

#include "CommandBase.h"

#define WIN_POINTS_USE "winpoints.use"

class WinPointsUseCommand: public CommandBase {
public:
    WinPointsUseCommand(std::string skillId) : CommandBase(WIN_POINTS_USE){
        putParam("skillId", CCString::create(skillId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__WinPointsUseCommand__) */
