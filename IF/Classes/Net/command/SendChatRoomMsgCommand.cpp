//
//  SendChatRoomMsgCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#include "SendChatRoomMsgCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool SendChatRoomMsgCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_SEND_MSG) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
    
//    params->setObject(CCString::create(CC_ITOA(CHAT_ROOM)), "type");
//    double t = GlobalData::shared()->getWorldTime();
//    t *= 1000;
//    
//    t = GlobalData::shared()->renewTime(t);
//    params->setObject(CCString::create(CC_ITOA(t)), "createTime");
//    params->setObject(CCString::create(CC_ITOA(0)), "save");
//    params->setObject(CCString::create(m_msg), "contents");
//    params->setObject(CCInteger::create(0), "tipFlag");
//    params->setObject(CCString::create(GlobalData::shared()->playerInfo.uid), "fromUid");
//    params->setObject(CCString::create(GlobalData::shared()->playerInfo.name), "fromName");
//
//    auto search = GlobalData::shared()->mailList.find(m_groupId);
//    bool isExistMail=(search != GlobalData::shared()->mailList.end() && m_groupId != "");
//    if(isExistMail)
//    {
//        MailController::getInstance()->addOneDialogToMailEnd(params,m_groupId,CHANNEL_TYPE_CHATROOM);
//    }

    return true;
}