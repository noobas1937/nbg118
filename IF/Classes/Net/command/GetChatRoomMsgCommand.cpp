//
//  GetChatRoomMsgCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-5-7.
//
//

#include "GetChatRoomMsgCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include "ChatMailInfo.h"
#include "MailDialogView.h"
#include "PopupViewController.h"

bool GetChatRoomMsgCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_GET_MSG) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    if(!MailController::getInstance()->getChatRoomEnable())
        return false;
    
    auto search = GlobalData::shared()->mailList.find(m_groupId);
    bool isExistMail=(search != GlobalData::shared()->mailList.end() && m_groupId != "");
    if (!isExistMail) {
        return false;
    }
    
    
    CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("msg"));

    if(array==NULL)
    {
        CCLOGFUNC("array==NULL");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::postNoMoreMessage(3);
#endif
        return false;
    }

    int num = array->count();
    
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(num<20)
    {
        ChatServiceCocos2dx::postChannelNoMoreData(3,true);
        if(num==0)
        {
            ChatServiceCocos2dx::postNoMoreMessage(3);
            return false;
        }
    }
#endif
    
    CCArray* mailInfoArr=CCArray::create();
    for (int i=num-1; i>=0; i--) {
        CCString* jsonMsg=dynamic_cast<CCString*>(array->objectAtIndex(i));
        if (jsonMsg) {
            string jsonStr=jsonMsg->getCString();
            CCLOGFUNCF("jsonStr %s",jsonStr.c_str());
            Json* jsonObj = Json_create(jsonStr.c_str());
            Json* jsonBody=Json_getItem(jsonObj, "body");
            CCDictionary* dic=MailController::getInstance()->parseChatRoomJsonMsg(jsonObj,jsonBody);
            Json_dispose(jsonObj);
            if(dic!=NULL)
            {
                MailDialogInfo* dialogInfo=MailController::getInstance()->parseChatRoomDic(dic,false,true);
                if (dialogInfo) {
                    ChatMailInfo* info=ChatMailInfo::create(dynamic_cast<MailInfo*>(search->second),dialogInfo,false,CHANNEL_TYPE_CHATROOM);
                    mailInfoArr->addObject(info);
                }
            }
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable() && mailInfoArr->count()>0)
    {
        CCLOG("ChatServiceCocos2dx::m_curSendMailIndex %d mailInfoArr->count():%d",ChatServiceCocos2dx::m_curSendMailIndex,mailInfoArr->count());
        MailController::getInstance()->notifyMailMsgToAndroid(mailInfoArr,m_groupId);
    }
    if (!ChatServiceCocos2dx::isChatShowing) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD, CCInteger::create(num));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChatServiceCocos2dx::enableChatRoom && mailInfoArr->count()>0)
    {
        CCLOG("ChatServiceCocos2dx::m_curSendMailIndex %d mailInfoArr->count():%d",ChatServiceCocos2dx::m_curSendMailIndex,mailInfoArr->count());
        MailController::getInstance()->notifyMailMsgToIOS(mailInfoArr,CHANNEL_TYPE_CHATROOM,m_groupId);
    }
    if (!ChatServiceCocos2dx::isChatShowing_fun()) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD, CCInteger::create(num));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
#endif
    return true;
}