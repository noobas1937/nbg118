//
//  KickChatRoomMemberCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#ifndef __IF__KickChatRoomMemberCommand__
#define __IF__KickChatRoomMemberCommand__

#include "CommandBase.h"

#define CHAT_ROOM_KICK "chat.room.member.kick"

class KickChatRoomMemberCommand:public CommandBase
{
public:
    KickChatRoomMemberCommand(std::string memberName,cocos2d::CCArray* memberUidArray,std::string groupId) :
    m_memberName(memberName)
    ,m_groupId(groupId)
    ,m_memberUidArray(memberUidArray)
    ,CommandBase(CHAT_ROOM_KICK)
    {
        CCLOGFUNCF("memberName %s  m_groupId %s",memberName.c_str(),m_groupId.c_str());
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
    std::string  m_groupId;
};

#endif /* defined(__IF__KickChatRoomMemberCommand__) */
