//
//  InviteChatRoomMemberCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#include "InviteChatRoomMemberCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool InviteChatRoomMemberCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_INVITE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
//    auto search = GlobalData::shared()->mailList.find(m_groupId);
//    bool isExistMail=(search != GlobalData::shared()->mailList.end() && m_groupId != "");
//    
//    
//
//    params->setObject(CCString::create(CC_ITOA(CHAT_ROOM)), "type");
//    double t = GlobalData::shared()->getWorldTime();
//    t *= 1000;
//    
//    t = GlobalData::shared()->renewTime(t);
//    params->setObject(CCString::create(CC_ITOA(t)), "createTime");
//    params->setObject(CCString::create(CC_ITOA(0)), "save");
//    string opStr=_lang("105347");
//    params->setObject(CCString::create(_lang_2("105337", opStr.c_str(),m_memberName.c_str())), "contents");
//    params->setObject(CCInteger::create(1), "tipFlag");
//    params->setObject(CCString::create(GlobalData::shared()->playerInfo.uid), "fromUid");
//    params->setObject(CCString::create(GlobalData::shared()->playerInfo.name), "fromName");
//    
//    if(isExistMail)
//    {
//        CCLOGFUNC("isExistMail");
//        MailInfo* mailInfo=dynamic_cast<MailInfo*>(search->second);
//        if (mailInfo->crMemberUidArray) {
//            CCLOGFUNCF("mailInfo->crMemberUidArray count:%d",mailInfo->crMemberUidArray->count());
//            mailInfo->crMemberUidArray->addObjectsFromArray(m_memberUidArray);
//        }
//        if (mailInfo->crMemberHeadPicArray) {
//             CCLOGFUNCF("mailInfo->crMemberHeadPicArray count:%d",mailInfo->crMemberHeadPicArray->count());
//            mailInfo->crMemberHeadPicArray->addObjectsFromArray(m_memberHeadPicArray);
//        }
//        if (mailInfo->crMemberHeaderVerArray) {
//            CCLOGFUNCF("mailInfo->crMemberHeaderVerArray count:%d",mailInfo->crMemberHeaderVerArray->count());
//            mailInfo->crMemberHeaderVerArray->addObjectsFromArray(m_memberHeadPicVerArray);
//        }
//        
//        CCLOGFUNC("isExistMail 22");
//        GlobalData::shared()->mailList[m_groupId]=mailInfo;
//        MailController::getInstance()->addOneDialogToMailEnd(params,m_groupId,CHANNEL_TYPE_CHATROOM);
//        
//        string memberUids="";
//        for (int j=0; j<mailInfo->crMemberUidArray->count(); j++) {
//            CCString* uid=dynamic_cast<CCString*>(mailInfo->crMemberUidArray->objectAtIndex(j));
//            if (uid) {
//                string uidStr=uid->getCString();
//                if (memberUids!="" && uidStr!="") {
//                    memberUids.append("_").append(uidStr);
//                }
//                else
//                {
//                    memberUids.append(uidStr);
//                }
//            }
//        }
//        CCLOGFUNCF("m_groupId:%s  memberUids:%s",m_groupId.c_str(),memberUids.c_str());
//        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        if(ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable())
//        {
//            if (memberUids!="") {
//                ChatServiceCocos2dx::setChannelMemberArray(3,m_groupId,memberUids);
//            }
//        }
//#endif
//    }
    return true;
}