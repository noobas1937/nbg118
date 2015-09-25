//
//  GetChatRoomMemberCommand.h
//  IF
//
//  Created by xupengzhan on 15-6-11.
//
//

#ifndef __IF__GetChatRoomMemberCommand__
#define __IF__GetChatRoomMemberCommand__


#include "CommandBase.h"

#define CHAT_ROOM_MEMBERS "chat.room.members"
class GetChatRoomMemberCommand: public CommandBase {
public:
    GetChatRoomMemberCommand(std::string groupId):
    CommandBase(CHAT_ROOM_MEMBERS),m_groupId(groupId)
    {
        putParam("id", CCString::create(groupId));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_groupId;
};

#endif /* defined(__IF__GetChatRoomMemberCommand__) */
