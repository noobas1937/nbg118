//
//  ArmyCancelCommand.h
//  IF
//
//  Created by 李锐奇 on 14-3-25.
//
//

#ifndef __IF__ArmyCancelCommand__
#define __IF__ArmyCancelCommand__


#include "CommandBase.h"
#define ARMY_CANCEL_COMMAND "army.cancel"

class ArmyCancelCommand: public CommandBase {
public:
    ArmyCancelCommand(std::string itemId, std::string uid):
    CommandBase(ARMY_CANCEL_COMMAND)
    {
        putParam("id", CCString::create(itemId));
        putParam("uuid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    void parseResult(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ArmyCancelCommand__) */
