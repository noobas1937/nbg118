//
//  ChatIsInAllanceCommand.cpp
//  IF
//
//  Created by liweiyu on 14-8-20.
//
//

#include "ChatIsInAllanceCommand.h"
#include "ChatController.h"

ChatIsInAllanceCommand::ChatIsInAllanceCommand(std::string uid)
: CommandBase(ALLIANCE_CHAT_ISINALLIANCE),m_uid(uid)
{
    putParam("uid", CCString::create(m_uid.c_str()));
}

bool ChatIsInAllanceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_CHAT_ISINALLIANCE) != 0)
        return false;
    
    std::string allianceId =
    _dict(_dict(dict->objectForKey("params")))->valueForKey("allianceId")->getCString();
    
    ChatController::getInstance()->newChaterAllianceId = allianceId;
    callSuccess(NetResult::create());
    return true;
}