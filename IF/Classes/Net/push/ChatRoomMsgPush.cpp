//
//  ChatRoomMsgPush.cpp
//  IF
//
//  Created by xupengzhan on 15-5-7.
//
//

#include "ChatRoomMsgPush.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include "ChatMailInfo.h"

void ChatRoomMsgPush::handleResponse(CCDictionary *dict)
{
    CCLOGFUNC("");
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    string groupId=params->valueForKey("roomId")->getCString();
    string groupName=params->valueForKey("name")->getCString();
    CCLOGFUNCF(" groupId %s",groupId.c_str());
    if (groupId=="") {
        return;
    }
    
    if(!MailController::getInstance()->getChatRoomEnable())
        return;
    
    int type=-1;
    CCDictionary* bodyDic=dynamic_cast<CCDictionary*>(params->objectForKey("body"));
    if (bodyDic) {
        type=bodyDic->valueForKey("type")->intValue();
    }
    
    CCLOGFUNCF(" type %d",type);
    
    string operatorUid=params->valueForKey("senderUid")->getCString();
    
    if(type==CHAT && GlobalData::shared()->chatShieldInfo.isShield(operatorUid))
        return;
    
    string playerUid=GlobalData::shared()->playerInfo.uid;
    bool isOperator=(operatorUid==playerUid);
    
    CCArray* mailInfoArr=CCArray::create();
    MailDialogInfo* dialogInfo=MailController::getInstance()->parseChatRoomDic(params,true,false,true);
    if (dialogInfo) {
        auto search = GlobalData::shared()->mailList.find(groupId);
        bool isExistMail=(search != GlobalData::shared()->mailList.end());
        if (isExistMail) {
            MailInfo* mailInfo=dynamic_cast<MailInfo*>(search->second);
            if (mailInfo) {
                ChatMailInfo* info=ChatMailInfo::create(mailInfo,dialogInfo,true,CHANNEL_TYPE_CHATROOM);
                CCLOGFUNCF("info type:%d  msg:%s",dialogInfo->type,info->msg.c_str());
                mailInfoArr->addObject(info);
            }
        }
    }
    
    if (type==MK) {
        MailController::getInstance()->getChatRoomIdArray()->addObject(CCString::create(groupId));
    }
    
    CCArray* arr=dynamic_cast<CCArray*>(params->objectForKey("invalid"));
    if (arr!=NULL) {
        for (int i=0; i<arr->count(); i++) {
            CCString* str=dynamic_cast<CCString*>(arr->objectAtIndex(i));
            CCLOGFUNCF("invalid uid: %s",str->getCString());
        }
    }
    
   string founderUid=params->valueForKey("founder")->getCString();
    CCLOGFUNCF("founderUid %s",founderUid.c_str());
    CCLOGFUNCF("groupId %s mailInfoArr count %d",groupId.c_str(),mailInfoArr->count());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable() && mailInfoArr->count()>0)
    {
//        if (type==MK && MailController::getInstance()->getTempChatRoomMsgDic()!=NULL) {
//            if (isO) {
//                statements
//            }
//             CCLOGFUNC("type==MK");
//            CCArray* arr=dynamic_cast<CCArray*>(MailController::getInstance()->getTempChatRoomMsgDic()->objectForKey(groupId));
//            if (arr!=NULL || arr->count()>0) {
//                MailController::getInstance()->getTempChatRoomMsgDic()->removeObjectForKey(groupId);
//            }
//            MailController::getInstance()->getTempChatRoomMsgDic()->setObject(mailInfoArr, groupId);
//        }
//        else
//        {
//            CCLOGFUNC("type!=MK");
            MailController::getInstance()->notifyMailMsgToAndroid(mailInfoArr,groupId,groupName);
//        }
        
        if (type==NAME) {
            string modifyName=bodyDic->valueForKey("msg")->getCString();
            CCLOGFUNCF("modify name:%s",modifyName.c_str());
            ChatServiceCocos2dx::notifyChatRoomNameChanged(modifyName);
        }
        if (founderUid!="") {
            ChatServiceCocos2dx::setChatRoomFounder(groupId,founderUid);
        }
    }
    
    if (!ChatServiceCocos2dx::isChatShowing) {
        if(type==MK)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SEND_SUCCESS);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
    }
    
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChatServiceCocos2dx::enableChatRoom && mailInfoArr->count()>0)
    {
        //        if (type==MK && MailController::getInstance()->getTempChatRoomMsgDic()!=NULL) {
        //            if (isO) {
        //                statements
        //            }
        //             CCLOGFUNC("type==MK");
        //            CCArray* arr=dynamic_cast<CCArray*>(MailController::getInstance()->getTempChatRoomMsgDic()->objectForKey(groupId));
        //            if (arr!=NULL || arr->count()>0) {
        //                MailController::getInstance()->getTempChatRoomMsgDic()->removeObjectForKey(groupId);
        //            }
        //            MailController::getInstance()->getTempChatRoomMsgDic()->setObject(mailInfoArr, groupId);
        //        }
        //        else
        //        {
        //            CCLOGFUNC("type!=MK");
        MailController::getInstance()->notifyMailMsgToIOS(mailInfoArr,CHANNEL_TYPE_CHATROOM,groupId);
        //        }
        
        if (type==NAME) {
            string modifyName=bodyDic->valueForKey("msg")->getCString();
            ChatServiceCocos2dx::notifyChatRoomNameChanged(modifyName);
        }
        if (founderUid!="") {
            ChatServiceCocos2dx::setChatRoomFounder(groupId,founderUid);
        }
    }
    
    
    if (!ChatServiceCocos2dx::isChatShowing_fun()) {
        if(type==MK)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SEND_SUCCESS);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
    }
#else
    
#endif
}

