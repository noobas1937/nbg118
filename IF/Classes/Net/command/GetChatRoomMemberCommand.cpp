//
//  GetChatRoomMemberCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-6-11.
//
//

#include "GetChatRoomMemberCommand.h"
#include "ChatServiceCocos2dx.h"
#include "MailController.h"

bool GetChatRoomMemberCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_MEMBERS) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    string founder=params->valueForKey("founder")->getCString();
    CCLOGFUNCF("founder %s",founder.c_str());
    
    CCArray* memberArray=dynamic_cast<CCArray*>(params->objectForKey("members"));
    if (memberArray!=NULL && memberArray->count()>0) {
        string memberStr="";
        for (int i=0; i<memberArray->count(); i++) {
            CCDictionary* dic=dynamic_cast<CCDictionary*>(memberArray->objectAtIndex(i));
            if (dic) {
                string uidStr=dic->valueForKey("uid")->getCString();
                if (memberStr!="" && uidStr!="") {
                    memberStr.append("_").append(uidStr);
                }
                else
                {
                    memberStr.append(uidStr);
                }
            }
        }
        
        string chatRoomName=params->valueForKey("name")->getCString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable()  &&memberStr!="" ) {
            ChatServiceCocos2dx::setChannelMemberArray(CHANNEL_TYPE_CHATROOM,m_groupId,memberStr,chatRoomName);
        }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        int channelType , string channelID , string uidArr , string roomOwner , string customName
        
        string groupId=params->valueForKey("id")->getCString();
        if (ChatServiceCocos2dx::enableChatRoom && MailController::getInstance()->getChatRoomEnable()  &&memberStr!="" ) {
            ChatServiceCocos2dx::updateChatChannelRoom(CHANNEL_TYPE_CHATROOM , groupId , memberStr , founder , chatRoomName);
        }
#endif
    }
    
    return true;
}