//
//  ChatIsInAllanceCommand.h
//  IF
//
//  Created by liweiyu on 14-8-20.
//
//

#ifndef __IF__ChatIsInAllanceCommand__
#define __IF__ChatIsInAllanceCommand__

#include "CommandBase.h"
#include "cocos2d.h"

#define ALLIANCE_CHAT_ISINALLIANCE "al.check"

class ChatIsInAllanceCommand: public CommandBase {
public:
    ChatIsInAllanceCommand(std::string uid);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_uid;
};

#endif /* defined(__IF__ChatIsInAllanceCommand__) */
