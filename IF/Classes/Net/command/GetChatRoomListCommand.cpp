//
//  GetChatRoomListCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-5-12.
//
//

#include "GetChatRoomListCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include <spine/Json.h>
#include "ChatMailInfo.h"

bool GetChatRoomListCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_GET_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        CCLOGFUNC("params==NULL");
        return false;
    }
    
    CCArray* roomArray = dynamic_cast<CCArray*>(params->objectForKey("rooms")) ;
    if (roomArray==NULL || roomArray->count()<=0) {
        return false;
    }
    CCLOGFUNCF("roomArray count %d",roomArray->count());
    MailController::getInstance()->getChatRoomIdArray()->removeAllObjects();
    string groupId="";
    for (int i=0;i<roomArray->count(); i++) {
        CCArray* mailInfoArr=CCArray::create();
        CCDictionary* info=dynamic_cast<CCDictionary*>(roomArray->objectAtIndex(i));
        if (info) {
            
            groupId=info->valueForKey("id")->getCString();
            if (groupId!="") {
                MailController::getInstance()->getChatRoomIdArray()->addObject(CCString::create(groupId));
            }
            string roomName=info->valueForKey("name")->getCString();
            CCArray* members=dynamic_cast<CCArray*>(info->objectForKey("members"));
            if (members==NULL || members->count()<=0) {
                return false;
            }
            
            CCArray* memberUidArray=CCArray::create();
            memberUidArray->retain();
            
            for (int j=0; j<members->count(); j++) {
                CCDictionary* dic=dynamic_cast<CCDictionary*>(members->objectAtIndex(j));
                if (dic) {
                    string uid=dic->valueForKey("uid")->getCString();
                    memberUidArray->addObject(CCString::create(uid));
                }
            }
            
            auto search = GlobalData::shared()->mailList.find(groupId);
            bool isExistMail=(search != GlobalData::shared()->mailList.end() && groupId != "");
            
            CCString* jsonMsg=dynamic_cast<CCString*>(info->objectForKey("msg"));

            if (jsonMsg) {
                MailInfo* mailInfo=NULL;
                if (!isExistMail) {
                    mailInfo=MailInfo::create();
                    mailInfo->crGroupId=groupId;
//                    mailInfo->retain();
//                    mailInfo->crMemberUidArray =memberUidArray;
                    GlobalData::shared()->mailList[groupId]=mailInfo;
                }
                else
                {
                    mailInfo=dynamic_cast<MailInfo*>(search->second);
                }

                
                string jsonStr=jsonMsg->getCString();
                CCLOGFUNCF("jsonStr %s",jsonStr.c_str());
                Json* jsonObj = Json_create(jsonStr.c_str());
                Json* jsonBody=Json_getItem(jsonObj, "body");
                CCDictionary* dic=MailController::getInstance()->parseChatRoomJsonMsg(jsonObj,jsonBody);
                Json_dispose(jsonObj);
                if(dic!=NULL)
                {
                    MailDialogInfo* dialogInfo=MailController::getInstance()->parseChatRoomDic(dic,true,true);
                    if (dialogInfo) {
                        ChatMailInfo* info=ChatMailInfo::create(mailInfo,dialogInfo,false,CHANNEL_TYPE_CHATROOM);
                        mailInfoArr->addObject(info);
                    }
                }
                CCLOGFUNCF("roomName %s,mailInfoArr count %d",roomName.c_str(),mailInfoArr->count());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if(ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable() && mailInfoArr->count()>0)
                {
                    ChatServiceCocos2dx::postIsChatRoomMemberFlag(groupId,true);
                    MailController::getInstance()->notifyMailMsgToAndroid(mailInfoArr,groupId);
                }
#endif
            }
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (!ChatServiceCocos2dx::isChatShowing) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
    }
#else
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
#endif
    
    return true;
}