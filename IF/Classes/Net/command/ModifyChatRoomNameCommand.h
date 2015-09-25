//
//  ModifyChatRoomNameCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#ifndef __IF__ModifyChatRoomNameCommand__
#define __IF__ModifyChatRoomNameCommand__

#include "CommandBase.h"

#define CHAT_ROOM_MODIFY_NAME "chat.room.name"

class ModifyChatRoomNameCommand:public CommandBase
{
public:
    ModifyChatRoomNameCommand(std::string name,std::string  groupId):
    m_groupId(groupId)
    ,m_roomName(name)
    ,CommandBase(CHAT_ROOM_MODIFY_NAME)
    {
        putParam("id", CCString::create(groupId));
        putParam("name",CCString::create(name));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_groupId;
    std::string m_roomName;
};


#endif /* defined(__IF__ModifyChatRoomNameCommand__) */
