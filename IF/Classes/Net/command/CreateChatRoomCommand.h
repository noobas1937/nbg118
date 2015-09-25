//
//  CreateChatRoomCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-5.
//
//

#ifndef __IF__CreateChatRoomCommand__
#define __IF__CreateChatRoomCommand__

#include "CommandBase.h"

#define CHAT_ROOM_CREATE "chat.room.mk"

class CreateChatRoomCommand : public CommandBase{
public:
    CreateChatRoomCommand(std::string memberName,cocos2d::CCArray* memberUidArray, std::string roomName,std::string content) :
    m_memberName(memberName)
    ,m_memberUidArray(memberUidArray)
    , m_roomName(roomName)
    ,m_content(content)
    , CommandBase(CHAT_ROOM_CREATE){
        putParam("name", CCString::create(roomName));
        if(content!="")
        {
            putParam("msg", CCString::create(content));
        }
        if(m_memberUidArray!=NULL)
        {
            putParam("members", m_memberUidArray);
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    cocos2d::CCArray* m_memberUidArray;
    std::string m_roomName;
    std::string m_content;
    std::string m_memberName;
    int m_type;
};

#endif /* defined(__IF__CreateChatRoomCommand__) */
