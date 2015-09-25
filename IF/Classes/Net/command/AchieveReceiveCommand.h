//
//  AchieveReceiveCommand.h
//  IF
//
//  Created by ganxiaohua on 13-12-23.
//
//

#ifndef __IF__AchieveReceiveCommand__
#define __IF__AchieveReceiveCommand__

#include "CommandBase.h"
#define ACHIEVE_RECEIVE "achieve.receive"

class AchieveReceiveCommand: public CommandBase {
public:
    AchieveReceiveCommand(std::string id);
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_id;
};
#endif /* defined(__IF__AchieveReceiveCommand__) */
