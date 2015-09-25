//
//  KickChatRoomMemberCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-5-6.
//
//

#include "KickChatRoomMemberCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool KickChatRoomMemberCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_KICK) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        CCLOGFUNC("22");
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOGFUNC("33");
        return false;
    }
    
//    CCLOGFUNCF("m_groupId %s",m_groupId.c_str());
//    auto search = GlobalData::shared()->mailList.find(m_groupId);
//    bool isExistMail=(search != GlobalData::shared()->mailList.end() && m_groupId != "");
//    
//    params->setObject(CCString::create(CC_ITOA(CHAT_ROOM)), "type");
//    double t = GlobalData::shared()->getWorldTime();
//    t *= 1000;
//    
//    t = GlobalData::shared()->renewTime(t);
//    params->setObject(CCString::create(CC_ITOA(t)), "createTime");
//    params->setObject(CCString::create(CC_ITOA(0)), "save");
//    string opStr=_lang("105347");
//    params->setObject(CCString::create(_lang_2("105338", opStr.c_str(),m_memberName.c_str())), "contents");
//    params->setObject(CCInteger::create(1), "tipFlag");
//    params->setObject(CCString::create(GlobalData::shared()->playerInfo.uid), "fromUid");
//    params->setObject(CCString::create(GlobalData::shared()->playerInfo.name), "fromName");
//    
//    if(isExistMail)
//    {
//        CCLOGFUNC("isExistMail");
//        MailInfo* mailInfo=dynamic_cast<MailInfo*>(search->second);
//        CCArray* memberUidArray=CCArray::create();
//        
//        if (mailInfo->crMemberUidArray) {
//            for(int i=0;i<mailInfo->crMemberUidArray->count();i++)
//            {
//                string uid=dynamic_cast<CCString*>(mailInfo->crMemberUidArray->objectAtIndex(i))->getCString();
//                for(int k=0;k<m_memberUidArray->count();k++)
//                {
//                    string uid2=dynamic_cast<CCString*>(m_memberUidArray->objectAtIndex(k))->getCString();
//                    CCLOGFUNCF("uid:%s   uid2:%s",uid.c_str(),uid2.c_str());
//                    if(uid!=uid2)
//                    {
//                        CCLOGFUNC("uid!=uid2");
//                        memberUidArray->addObject(dynamic_cast<CCString*>(mailInfo->crMemberUidArray->objectAtIndex(i)));
//                    }
//                }
//            }
//            mailInfo->crMemberUidArray->removeAllObjects();
//            mailInfo->crMemberUidArray->addObjectsFromArray(memberUidArray);
//            CCLOGFUNCF("mailInfo->crMemberUidArray %d",mailInfo->crMemberUidArray->count());
//        }
//        if (mailInfo->crMemberHeadPicArray) {
//            mailInfo->crMemberHeadPicArray->removeObjectsInArray(m_memberHeadPicArray);
//        }
//        if (mailInfo->crMemberHeaderVerArray) {
//            mailInfo->crMemberHeaderVerArray->removeObjectsInArray(m_memberHeadPicVerArray);
//        }
//        
//         GlobalData::shared()->mailList[m_groupId]=mailInfo;
//        MailController::getInstance()->addOneDialogToMailEnd(params,m_groupId,CHANNEL_TYPE_CHATROOM);
//        
//        string memberUids="";
//        for (int j=0; j<memberUidArray->count(); j++) {
//            CCString* uid=dynamic_cast<CCString*>(memberUidArray->objectAtIndex(j));
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
//        CCLOGFUNCF("memberUids %s",memberUids.c_str());
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