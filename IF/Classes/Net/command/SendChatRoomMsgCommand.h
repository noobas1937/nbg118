//
//  SendChatRoomMsgCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#ifndef __IF__SendChatRoomMsgCommand__
#define __IF__SendChatRoomMsgCommand__

#include "CommandBase.h"

#define CHAT_ROOM_SEND_MSG "chat.room.send"

class SendChatRoomMsgCommand:public CommandBase
{
public:
    SendChatRoomMsgCommand(std::string msg, std::string groupId, std::string sendLocalTime):
    m_groupId(groupId)
    ,m_msg(msg)
    ,m_sendLocalTime(sendLocalTime)
    ,CommandBase(CHAT_ROOM_SEND_MSG)
    {
        if(groupId!="")
        {
            putParam("id", CCString::create(groupId));
        }
        if(msg!="")
        {
            putParam("msg", CCString::create(msg));
        }
        if(sendLocalTime!="")
        {
            putParam("sendLocalTime", CCString::create(sendLocalTime));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_msg;
    std::string m_groupId;
    std::string m_sendLocalTime;
};

#endif /* defined(__IF__SendChatRoomMsgCommand__) */
