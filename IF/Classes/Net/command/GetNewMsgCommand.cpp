//
//  GetNewMsgCommand.cpp
//  IF
//
//  Created by 周雨 on 15-5-28.
//
//

#include "GetNewMsgCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include <spine/Json.h>
#include "ChatMailInfo.h"
#include "ChatController.h"

GetNewMsgCommand::~GetNewMsgCommand()
{
    CC_SAFE_RELEASE_NULL(m_paramDic);
}

bool GetNewMsgCommand::handleRecieve(cocos2d::CCDictionary *dict){
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_GET_NEW_MSG) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        CCLOGFUNC("params==NULL");
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        return false;
    }
    
    MailController::getInstance()->getChatRoomIdArray()->removeAllObjects();
    CCArray* roomArray = dynamic_cast<CCArray*>(params->objectForKey("rooms")) ;
    if (roomArray==NULL || roomArray->count()<=0) {
        return false;
    }
    CCLOGFUNCF("roomArray count %d",roomArray->count());
    
    string newMsgInfo="";
    CCDictionary *item = NULL;
    CCArray* chatInfoArr=CCArray::create();
    
    vector<std::string> *uids = new vector<std::string>();

    for(int i=0;i<roomArray->count();i++)
    {
        CCDictionary* roomDic=dynamic_cast<CCDictionary*>(roomArray->objectAtIndex(i));
        int type=-1;
        if (roomDic) {
            type=roomDic->valueForKey("type")->intValue();
            string firstSeqId="";
            string lastSeqId="";
            string id="";
            int requestSeqId=0;
            if(type!=CHANNEL_TYPE_USER && type!=CHANNEL_TYPE_OFFICIAL)
            {
                firstSeqId=roomDic->valueForKey("firstSeqId")->getCString();
                lastSeqId=roomDic->valueForKey("lastSeqId")->getCString();
                id=roomDic->valueForKey("id")->getCString();
                requestSeqId=m_paramDic->valueForKey(id)->intValue();
                CCLOGFUNCF("id:%s  requestSeqId:%d",id.c_str(),requestSeqId);
                string tempStr=CC_ITOA(type);
                if(firstSeqId=="")
                    firstSeqId="-1";
                if(lastSeqId=="")
                    lastSeqId="-1";
                tempStr.append("|").append(id).append("|").append(firstSeqId).append("|").append(lastSeqId);
                if (newMsgInfo=="") {
                    newMsgInfo.append(tempStr);
                }
                else
                {
                    newMsgInfo.append(",").append(tempStr);
                }

            }
            
            CCArray* msgArr=dynamic_cast<CCArray*>(roomDic->objectForKey("msg"));
            
            if (msgArr && msgArr->count()>0) {
                if (type==CHANNEL_TYPE_COUNTRY || type==CHANNEL_TYPE_ALLIANCE) {
                    CCArray* chatMailArr=CCArray::create();
                    for (int i=0; i<msgArr->count(); i++) {
                        item = _dict(msgArr->objectAtIndex(i));
                        if (type==CHANNEL_TYPE_COUNTRY)
                        {
                            if(ChatInfo(item).sequenceId>ChatController::getInstance()->m_latestCountryMsg.sequenceId)
                                ChatController::getInstance()->m_latestCountryMsg=ChatInfo(item);
                            ChatController::getInstance()->m_chatCountryPool.insert(ChatController::getInstance()->m_chatCountryPool.begin(), ChatInfo(item));
                        }
                        else if (type==CHANNEL_TYPE_ALLIANCE)
                        {
                            if(ChatInfo(item).sequenceId>ChatController::getInstance()->m_latestAllianceMsg.sequenceId)
                                ChatController::getInstance()->m_latestAllianceMsg=ChatInfo(item);
                            ChatController::getInstance()->m_chatAlliancePool.insert(ChatController::getInstance()->m_chatAlliancePool.begin(), ChatInfo(item));
                        }
                        ChatMailInfo* info=ChatMailInfo::create(ChatInfo(item),false);
                        if (requestSeqId<info->sequenceId) {
                            chatMailArr->addObject(info);
                        }
                    }
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
                    ChatController::getInstance()->notifyMsgToIOS(chatMailArr, type, id);
#else
                    ChatController::getInstance()->notifyMsgToAndroid(chatMailArr,type,id);
#endif
                }
                else if(type==CHANNEL_TYPE_CHATROOM && MailController::getInstance()->getChatRoomEnable())
                {
                    CCArray* members=dynamic_cast<CCArray*>(roomDic->objectForKey("members"));
                    string founderUid=roomDic->valueForKey("founder")->getCString();
                    string rommName=roomDic->valueForKey("roomName")->getCString();
                    if (members==NULL || members->count()<=0) {
                        continue;
                    }
                    
                    if (id!="") {
                        MailController::getInstance()->getChatRoomIdArray()->addObject(CCString::create(id));
                    }
                    
                    CCArray* memberUidArray=CCArray::create();
                    memberUidArray->retain();
                    
                    string memberUids="";
                    for (int j=0; j<members->count(); j++) {
                        CCDictionary* dic=dynamic_cast<CCDictionary*>(members->objectAtIndex(j));
                        if (dic) {
                            string uid=dic->valueForKey("uid")->getCString();
//                            CCLOGFUNCF("chatroom uid:%s",uid.c_str());
                            if (memberUids!="" && uid!="") {
                                memberUids.append("_").append(uid);
                            }
                            else
                            {
                                memberUids.append(uid);
                            }
                            memberUidArray->addObject(CCString::create(uid));
                        }
                    }
//                    CCLOGFUNCF("chatroom memberUids:%s",memberUids.c_str());
                    
                    auto search = GlobalData::shared()->mailList.find(id);
                    bool isExistMail=(search != GlobalData::shared()->mailList.end() && id != "");
                    
                    MailInfo* mailInfo=NULL;
                    if (!isExistMail) {
                        mailInfo=MailInfo::create();
                        mailInfo->crGroupId=id;
                        mailInfo->retain();
                        GlobalData::shared()->mailList[id]=mailInfo;
                    }
                    else
                    {
                        mailInfo=dynamic_cast<MailInfo*>(search->second);
                    }
                    
                    CCArray* chatMailArr=CCArray::create();
                    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    if(ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable())
                    {
                        for (int i=0; i<msgArr->count(); i++) {
                            item = _dict(msgArr->objectAtIndex(i));
                            MailDialogInfo* dialogInfo=MailController::getInstance()->parseChatRoomDic(item,true,true);
                            if (dialogInfo) {
                                ChatMailInfo* info=ChatMailInfo::create(mailInfo,dialogInfo,true,CHANNEL_TYPE_CHATROOM);
                                if (ChatController::getInstance()->m_chatUserInfoDic->objectForKey(dialogInfo->uid)==NULL) {
                                    uids->push_back(dialogInfo->uid);
                                }
                                if (requestSeqId<info->sequenceId) {
                                    chatMailArr->addObject(info);
                                }
                            }
                        }
                        
                        ChatServiceCocos2dx::setChannelMemberArray(CHANNEL_TYPE_CHATROOM,id,memberUids,rommName);
                        
                        if( chatMailArr->count()>0)
                        {
                            ChatServiceCocos2dx::postIsChatRoomMemberFlag(id,true);
                            ChatController::getInstance()->notifyMsgToAndroid(chatMailArr,CHANNEL_TYPE_CHATROOM,id,rommName);
                            if (founderUid!="") {
                                ChatServiceCocos2dx::setChatRoomFounder(id,founderUid);
                            }
                        }
                        
                        
                    }
                    else
                    {
                        for (int i=msgArr->count()-1; i>=0; i--) {
                            item = _dict(msgArr->objectAtIndex(i));
                            MailController::getInstance()->parseChatRoomDic(item,true,true);
                        }
                    }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    if(ChatServiceCocos2dx::enableChatRoom)
                    {
                        for (int i=0; i<msgArr->count(); i++) {
                            item = _dict(msgArr->objectAtIndex(i));
                            MailDialogInfo* dialogInfo=MailController::getInstance()->parseChatRoomDic(item,true,true);
                            if (dialogInfo) {
                                ChatMailInfo* info=ChatMailInfo::create(mailInfo,dialogInfo,true,CHANNEL_TYPE_CHATROOM);
                                if (ChatController::getInstance()->m_chatUserInfoDic->objectForKey(dialogInfo->uid)==NULL) {
                                    uids->push_back(dialogInfo->uid);
                                }
                                if (requestSeqId<info->sequenceId) {
                                    chatMailArr->addObject(info);
                                }
                            }
                        }
                        if (chatMailArr->count()>0) {
                            ChatController::getInstance()->notifyMsgToIOS(chatMailArr,CHANNEL_TYPE_CHATROOM,id);
                            if (founderUid!="") {
                                ChatServiceCocos2dx::setChatRoomFounder(id,founderUid);
                            }
                        }
                        ChatServiceCocos2dx::updateChatChannelRoom(CHANNEL_TYPE_CHATROOM,id,memberUids,"","" );
                    }
                    else
                    {
                        for (int i=msgArr->count()-1; i>=0; i--) {
                            item = _dict(msgArr->objectAtIndex(i));
                            MailController::getInstance()->parseChatRoomDic(item,true,true);
                        }
                    }
#endif
                }
            }
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //
#else
//    if (uids->size()>0) {
//        ChatController::getInstance()->getMultiUserInfo(uids);
//    }
#endif
    
    delete uids;
    
    CCLOGFUNCF("newMsgInfo %s",newMsgInfo.c_str());
    if (newMsgInfo=="") {
        return false;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::postChannelInfo(newMsgInfo);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::updateChatChannelInfo(newMsgInfo);
#endif
    
    return true;
}