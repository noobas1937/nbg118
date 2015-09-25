//
//  QuitChatRoomArrayCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-6-9.
//
//

#include "QuitChatRoomArrayCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool QuitChatRoomArrayCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_ARRAY_QUIT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    MailController::getInstance()->getChatRoomIdArray()->removeAllObjects();
    return true;
}