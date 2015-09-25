//
//  CreateChatRoomCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-5-5.
//
//

#include "CreateChatRoomCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool CreateChatRoomCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_CREATE) != 0)
        return false;

    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }

    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
    string groupIdStr = params->valueForKey("id")->getCString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    
    auto interator = GlobalData::shared()->mailList.find(groupIdStr);
    
    if (interator!=GlobalData::shared()->mailList.end()) {
        MailInfo* mailInfo=dynamic_cast<MailInfo*>(interator->second);
        if (mailInfo) {
            if(!MailController::getInstance()->getIsNewMailListEnable()){
                ChatServiceCocos2dx::setMailInfo(groupIdStr.c_str(),mailInfo->uid.c_str(),mailInfo->fromName.c_str(),CHAT_ROOM);
                
                if(!ChatServiceCocos2dx::isChatShowing){
                    CCLOGFUNC("!ChatServiceCocos2dx::isChatShowing");
                }
                else
                {
                    CCLOGFUNC("ChatServiceCocos2dx::isChatShowing");
                    ChatServiceCocos2dx::exitChatActivityFrom2dx();
                }
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_CHATROOM;
                ChatServiceCocos2dx::m_isNoticItemUsed=false;
                MailController::getInstance()->setOldOpenMailInfo(mailInfo);
                ChatServiceCocos2dx::showChatActivityFrom2dx();
            }else{
                
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_CHATROOM;
                ChatServiceCocos2dx::m_isNoticItemUsed=false;
                MailController::getInstance()->setOldOpenMailInfo(mailInfo);
                ChatServiceCocos2dx::setMailInfo(groupIdStr.c_str(),mailInfo->uid.c_str(),mailInfo->fromName.c_str(),CHAT_ROOM);
                ChatServiceCocos2dx::onCreateChatroomSuccess();
            }
        }
    }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto interator = GlobalData::shared()->mailList.find(groupIdStr);
    
    if (interator!=GlobalData::shared()->mailList.end()) {
        MailInfo* mailInfo=dynamic_cast<MailInfo*>(interator->second);
        if (mailInfo) {
            ChatServiceCocos2dx::setMailInfo(groupIdStr.c_str(),mailInfo->uid.c_str(),mailInfo->fromName.c_str(),CHAT_ROOM);
            
            if(!ChatServiceCocos2dx::isChatShowing_fun()){
                CCLOGFUNC("!ChatServiceCocos2dx::isChatShowing");
            }
            else
            {
                CCLOGFUNC("ChatServiceCocos2dx::isChatShowing");
                //ChatServiceCocos2dx::exitChatActivityFrom2dx();
            }
            ChatServiceCocos2dx::settingGroupChatMailVCChatChannel();
            
            ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_CHATROOM;
            ChatServiceCocos2dx::m_isNoticItemUsed=false;
            MailController::getInstance()->setOldOpenMailInfo(mailInfo);
            ChatServiceCocos2dx::showChatIOSFrom2dx();
        }
    }
#endif
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SEND_SUCCESS);
    return true;
}
