//
//  QuitChatRoomArrayCommand.h
//  IF
//
//  Created by xupengzhan on 15-6-9.
//
//

#ifndef __IF__QuitChatRoomArrayCommand__
#define __IF__QuitChatRoomArrayCommand__

#include "CommandBase.h"

#define CHAT_ROOM_ARRAY_QUIT "chat.room.member.quitmult"

class QuitChatRoomArrayCommand:public CommandBase
{
public:
    QuitChatRoomArrayCommand(CCArray* chatRoomIdArray):
    CommandBase(CHAT_ROOM_ARRAY_QUIT)
    {
        if(chatRoomIdArray!=NULL)
        putParam("roomids", chatRoomIdArray);
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__QuitChatRoomArrayCommand__) */
