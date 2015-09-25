//
//  GetChatRoomMsgCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-7.
//
//

#ifndef __IF__GetChatRoomMsgCommand__
#define __IF__GetChatRoomMsgCommand__

#include "CommandBase.h"

#define CHAT_ROOM_GET_MSG "chat.room.msg"

class GetChatRoomMsgCommand:public CommandBase
{
public:
    GetChatRoomMsgCommand(bool isShowing,int start,int count,std::string groupId):
    m_groupId(groupId)
    ,m_start(start)
    ,m_count(count)
    ,m_isDialogShowing(isShowing)
    ,CommandBase(CHAT_ROOM_GET_MSG)
    {
        putParam("id", CCString::create(groupId));
        putParam("seqId", CCString::create(CC_ITOA(start)));
        putParam("count", CCInteger::create(count));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_start;
    int m_count;
    std::string m_groupId;
    bool m_isDialogShowing;
};

#endif /* defined(__IF__GetChatRoomMsgCommand__) */
