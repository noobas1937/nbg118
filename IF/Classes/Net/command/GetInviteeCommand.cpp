//
//  GetInviteeCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-5-20.
//
//

#include "GetInviteeCommand.h"
#include "NetController.h"
#include "MailController.h"
#include "HFUrlImageCache.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"

static string COMMAND_CHAT_ROOM_INVITEE = "chat.room.invitee";

GetInviteeCommand::GetInviteeCommand()
: CommandBase(COMMAND_CHAT_ROOM_INVITEE)
{
    
}

bool GetInviteeCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_CHAT_ROOM_INVITEE.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        onGetAllianceInfoSuccess(params);
    }
    return true;
}

void GetInviteeCommand::onGetAllianceInfoSuccess(CCObject* pObj)
{
    CCLOGFUNC("");
    MailController::getInstance()->m_chatRoomMemberArr->removeAllObjects();
    CCDictionary* params = dynamic_cast<CCDictionary*>(pObj);
    if (params==NULL || ChatController::getInstance()->m_chatUserInfoDic==NULL) {
        return;
    }
    CCArray* members = dynamic_cast<CCArray*>(params->objectForKey("invitee"));
    string uidStr="";
    string lastUpdateTimeStr="";
    if(members)
    {
        vector<std::string> *uids = new vector<std::string>();
        CCLOGFUNCF("members->count():%d",members->count());
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = (CCDictionary*)members->objectAtIndex(i);
            
//            CCDictionary* userInfoDic=CCDictionary::create();
//            userInfoDic->setObject(member->objectForKey("rank"), "allianceRank");
//            userInfoDic->setObject(member->objectForKey("uid"), "uid");
//            userInfoDic->setObject(member->objectForKey("name"), "userName");
//            userInfoDic->setObject(member->objectForKey("pic"), "headPic");
//            userInfoDic->setObject(member->objectForKey("picVer"), "headPicVer");
//            userInfoDic->setObject(member->objectForKey("gmFlag"), "mGmod");
            string uid=member->valueForKey("uid")->getCString();
            if(uid!="")
            {
                if (ChatController::getInstance()->m_chatUserInfoDic->objectForKey(uid)==NULL) {
                    uids->push_back(uid);
                }
                string time=member->valueForKey("lastUpdateTime")->getCString();
                if (time=="") {
                    time="0";
                }
                
                if(uidStr!="")
                {
                    uidStr.append("_").append(uid);
                    lastUpdateTimeStr.append("_").append(time);
                }
                else
                {
                    uidStr=uid;
                    lastUpdateTimeStr=time;
                }
            }
//            userInfoDic->setObject(member->objectForKey("lastUpdateTime"), "lastUpdateTime");
//            userInfoDic->setObject(member->objectForKey("level"), "vipLevel");
//            userInfoDic->setObject(member->objectForKey("abbr"), "asn");
//            userInfoDic->setObject(member->objectForKey("vipEndTime"), "vipEndTime");
//            MailController::getInstance()->m_chatRoomMemberArr->addObject(userInfoDic);
        }
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        //
//#else
//        if (uids->size()>0) {
//            ChatController::getInstance()->getMultiUserInfo(uids);
//        }
//#endif
        
    }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    
    if (uidStr!="") {
        ChatServiceCocos2dx::notifyUserUids(uidStr,lastUpdateTimeStr);
    }
    
//    MailController::getInstance()->m_allianceMemberInfoDic->removeAllObjects();
//    ChatServiceCocos2dx::m_curAllianceMemberIndex=0;
//    
//    CCArray* memberArr=MailController::getInstance()->m_chatRoomMemberArr;
//    if (memberArr->count()>0) {
//        if(memberArr->count()>20)
//        {
//            CCArray* userArr=CCArray::create();
//            int count=0;
//            for (int i=0; i<memberArr->count(); i++) {
//                if(count==20)
//                {
//                    MailController::getInstance()->m_allianceMemberInfoDic->setObject(userArr, ChatServiceCocos2dx::m_curAllianceMemberIndex);
//                    ChatServiceCocos2dx::notifyUserInfo(true,ChatServiceCocos2dx::m_curAllianceMemberIndex);
//                    ChatServiceCocos2dx::m_curAllianceMemberIndex++;
//                    userArr->removeAllObjects();
//                    count=0;
//                }
//                userArr->addObject(memberArr->objectAtIndex(i));
//                count++;
//            }
//            
//            if(userArr->count()>0)
//            {
//                MailController::getInstance()->m_allianceMemberInfoDic->setObject(userArr, ChatServiceCocos2dx::m_curAllianceMemberIndex);
//                ChatServiceCocos2dx::notifyUserInfo(true,ChatServiceCocos2dx::m_curAllianceMemberIndex);
//                ChatServiceCocos2dx::m_curAllianceMemberIndex++;
//            }
//            
//        }
//        else
//        {
//            CCLOG("memberArr->count():%d",memberArr->count());
//            MailController::getInstance()->m_allianceMemberInfoDic->setObject(memberArr, ChatServiceCocos2dx::m_curAllianceMemberIndex);
//            ChatServiceCocos2dx::notifyUserInfo(true,ChatServiceCocos2dx::m_curAllianceMemberIndex);
//            ChatServiceCocos2dx::m_curAllianceMemberIndex++;
//        }
//    }
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if (uidStr!="") {
        ChatServiceCocos2dx::notifyUserUids(uidStr,lastUpdateTimeStr);
    }
#endif
}