//
//  GetChatRoomListCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-12.
//
//

#ifndef __IF__GetChatRoomListCommand__
#define __IF__GetChatRoomListCommand__

#include "CommandBase.h"

#define CHAT_ROOM_GET_LIST "chat.room.user.rooms"

class GetChatRoomListCommand:public CommandBase
{
public:
    GetChatRoomListCommand():
    CommandBase(CHAT_ROOM_GET_LIST)
    {
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#endif /* defined(__IF__GetChatRoomListCommand__) */
