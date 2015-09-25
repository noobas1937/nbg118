//
//  QuitChatRoomCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#ifndef __IF__QuitChatRoomCommand__
#define __IF__QuitChatRoomCommand__

#include "CommandBase.h"

#define CHAT_ROOM_QUIT "chat.room.member.quit"

class QuitChatRoomCommand:public CommandBase
{
public:
    QuitChatRoomCommand(std::string groupId):
    m_groupId(groupId)
    ,CommandBase(CHAT_ROOM_QUIT)
    {
        putParam("id", CCString::create(groupId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_groupId;
};

#endif /* defined(__IF__QuitChatRoomCommand__) */
