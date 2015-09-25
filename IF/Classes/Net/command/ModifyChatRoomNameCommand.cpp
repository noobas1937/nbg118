//
//  ModifyChatRoomNameCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#include "ModifyChatRoomNameCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool ModifyChatRoomNameCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_MODIFY_NAME) != 0)
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
//    string opStr=_lang("105347");
//    params->setObject(CCString::create(_lang_2("105346",opStr.c_str(),m_roomName.c_str())), "contents");
//    params->setObject(CCInteger::create(1), "tipFlag");
//    params->setObject(CCString::create(GlobalData::shared()->playerInfo.uid), "fromUid");
//    params->setObject(CCString::create(GlobalData::shared()->playerInfo.name), "fromName");
//    params->setObject(CCString::create(m_roomName), "oldName");
//    
//    auto search = GlobalData::shared()->mailList.find(m_groupId);
//    bool isExistMail=(search != GlobalData::shared()->mailList.end() && m_groupId != "");
//    if(isExistMail)
//    {
//        search->second->fromName=m_roomName;
//        MailController::getInstance()->addOneDialogToMailEnd(params,m_groupId,CHANNEL_TYPE_CHATROOM);
//    }
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    if(ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable())
//    {
//        ChatServiceCocos2dx::notifyChatRoomNameChanged(m_roomName);
//    }
//#endif
    return true;
}