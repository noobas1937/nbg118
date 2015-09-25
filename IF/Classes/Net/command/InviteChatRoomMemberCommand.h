//
//  InviteChatRoomMemberCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#ifndef __IF__InviteChatRoomMemberCommand__
#define __IF__InviteChatRoomMemberCommand__

#include "CommandBase.h"

#define CHAT_ROOM_INVITE "chat.room.member.invite"

class InviteChatRoomMemberCommand : public CommandBase{
public:
    InviteChatRoomMemberCommand(std::string memberName,cocos2d::CCArray* memberUidArray,std::string groupId) :
    m_memberName(memberName)
    ,m_groupId(groupId)
    ,m_memberUidArray(memberUidArray)
    ,CommandBase(CHAT_ROOM_INVITE){
        putParam("id", CCString::create(groupId));
        if(memberUidArray!=NULL)
        {
            putParam("members", memberUidArray);
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    cocos2d::CCArray* m_memberUidArray;
    std::string m_memberName;
    std::string m_groupId;
};

#endif /* defined(__IF__InviteChatRoomMemberCommand__) */
