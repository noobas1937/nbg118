//
//  ChatRoomMsgPush.h
//  IF
//
//  Created by xupengzhan on 15-5-7.
//
//

#ifndef __IF__ChatRoomMsgPush__
#define __IF__ChatRoomMsgPush__

#include "CommonInclude.h"

class ChatRoomMsgPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
    
private:
    enum RoomChatType{
        MK,             //创建
        INVITE,     //邀请
        KICK,         //踢人
        QUIT,           //退出
        CHAT,          //消息
        NAME        //改名
    };
};

#endif /* defined(__IF__ChatRoomMsgPush__) */
