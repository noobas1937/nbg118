//
//  GetNewMailMsgCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-7-15.
//
//

#include "GetNewMailMsgCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include <spine/Json.h>
#include "ChatMailInfo.h"
#include "ChatController.h"

GetNewMailMsgCommand::~GetNewMailMsgCommand()
{
    CC_SAFE_RELEASE_NULL(m_paramDic);
}

bool GetNewMailMsgCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CHAT_GET_NEW_MAIL_MSG) != 0)
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
    if (params->objectForKey("updatemail")) {
        string data=params->valueForKey("updatemail")->getCString();
        CCLOGFUNCF("data : %s",data.c_str());
        if (data!="") {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CCLOGFUNCF("data2 : %s",data.c_str());
            ChatServiceCocos2dx::postMailUpdate(data);
#endif
        }
    }

    MailController::getInstance()->getChatRoomIdArray()->removeAllObjects();
    CCArray* roomArray = dynamic_cast<CCArray*>(params->objectForKey("rooms")) ;
    if (roomArray!=NULL && roomArray->count()>0) {
        CCLOGFUNCF("roomArray count %d",roomArray->count());
        
        string newMsgInfo="";
        CCDictionary *item = NULL;
        CCArray* chatInfoArr=CCArray::create();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CCArray* chatInfoArr_IOSMailList=CCArray::create();
#endif
        
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
                    
                    CCLOGFUNCF("MSG SIZE:%d",msgArr->count());
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
                        CCLOGFUNCF("roomName:%s",rommName.c_str());
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
                                        chatInfoArr_IOSMailList->addObject(info);
                                    }
                                }
                            }
                            if (chatMailArr->count()>0) {
                                ChatController::getInstance()->notifyMsgToIOS(chatMailArr,CHANNEL_TYPE_CHATROOM,id);
                                if (founderUid!="") {
                                    ChatServiceCocos2dx::setChatRoomFounder(id,founderUid);
                                }
                            }
                            ChatServiceCocos2dx::updateChatChannelRoom(CHANNEL_TYPE_CHATROOM, id, memberUids, "", "");
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
                    else if(type==CHANNEL_TYPE_USER)
                    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        if (MailController::getInstance()->getIsNewMailListEnable()) {
                            int minCreateTime = -1;
                            int minModCreateTime = -1;
                            for (int i=0; i<msgArr->count(); i++) {
                                item = _dict(msgArr->objectAtIndex(i));
                                MailInfo* mailInfo=MailInfo::create();
                                mailInfo->parse(item);
                                string fromUid=mailInfo->fromUid;
                                id=fromUid;
                                CCArray* mailInfoArr=CCArray::create();
                                CCArray* modMailInfoArr=CCArray::create();
                                CCObject* obj;
                                CCARRAY_FOREACH(mailInfo->dialogs, obj){
                                    MailDialogInfo* tempDialogInfo = dynamic_cast<MailDialogInfo*>(obj);
                                    bool isNew  = true;
                                    if(tempDialogInfo->status==1)
                                        isNew = false;
                                    ChatMailInfo* info=ChatMailInfo::create(mailInfo,tempDialogInfo,isNew);
                                    
                                    if(info->post == 0)
                                    {
                                        if(minCreateTime==-1)
                                            minCreateTime=info->createTime;
                                        else if(info->createTime<minCreateTime)
                                            minCreateTime=info->createTime;
                                        mailInfoArr->addObject(info);
                                    }
                                    else if(info->post == 200)
                                    {
                                        if(minModCreateTime==-1)
                                            minModCreateTime=info->createTime;
                                        else if(info->createTime<minModCreateTime)
                                            minModCreateTime=info->createTime;
                                        modMailInfoArr->addObject(info);
                                    }
                                }
                                CCLOGFUNCF("CHANNEL_TYPE_USER mail: fromUid:%s  fromName:%s",mailInfo->fromUid.c_str(),mailInfo->fromName.c_str());
                                if (mailInfo->fromUid!="" && mailInfoArr->count()>0) {
                                    ChatController::getInstance()->notifyMsgToAndroid(mailInfoArr,CHANNEL_TYPE_USER,mailInfo->fromUid,mailInfo->fromName);
                                }
                                if (mailInfo->fromUid!="" && modMailInfoArr->count()>0) {
                                    ChatController::getInstance()->notifyMsgToAndroid(modMailInfoArr,CHANNEL_TYPE_USER,mailInfo->fromUid,mailInfo->fromName,true);
                                }
                            }
                            
                            string tempStr=CC_ITOA(type);
                            if(minCreateTime!=-1)
                                tempStr.append("|").append(id).append("|").append(CC_ITOA(minCreateTime)).append("|-1");
                            if(minModCreateTime!=-1)
                                tempStr.append("|").append(id).append("@mod|").append(CC_ITOA(minModCreateTime)).append("|-1");
                            if (newMsgInfo=="") {
                                newMsgInfo.append(tempStr);
                            }
                            else
                            {
                                newMsgInfo.append(",").append(tempStr);
                            }
                        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        int minCreateTime=-1;
                        for (int i=0; i<msgArr->count(); i++) {
                            item = _dict(msgArr->objectAtIndex(i));
                            MailInfo* mailInfo=MailInfo::create();
                            mailInfo->parse(item);
                            string fromUid=mailInfo->fromUid;
                            id=fromUid;
                            CCArray* mailInfoArr=CCArray::create();
                            CCObject* obj;
                            CCARRAY_FOREACH(mailInfo->dialogs, obj){
                                MailDialogInfo* tempDialogInfo = dynamic_cast<MailDialogInfo*>(obj);
                                ChatMailInfo* info=ChatMailInfo::create(mailInfo,tempDialogInfo,true);
                                if(minCreateTime==-1)
                                    minCreateTime=info->createTime;
                                else if(info->createTime<minCreateTime)
                                    minCreateTime=info->createTime;
                                mailInfoArr->addObject(info);
                                chatInfoArr_IOSMailList->addObject(info);
                            }
                            CCLOGFUNCF("CHANNEL_TYPE_USER mail: fromUid:%s  fromName:%s",mailInfo->fromUid.c_str(),mailInfo->fromName.c_str());
                            if (mailInfo->fromUid!="" && mailInfoArr->count()>0) {
                                ChatController::getInstance()->notifyMsgToIOS(mailInfoArr,CHANNEL_TYPE_USER,mailInfo->fromUid);
                            }
                        }
                        
                        string tempStr=CC_ITOA(type);
                        if(firstSeqId=="")
                            firstSeqId=minCreateTime;
                        if(lastSeqId=="")
                            lastSeqId="-1";
                        tempStr.append("|").append(id).append("|").append(CC_ITOA(minCreateTime)).append("|").append(lastSeqId);
                        if (newMsgInfo=="") {
                            newMsgInfo.append(tempStr);
                        }
                        else
                        {
                            newMsgInfo.append(",").append(tempStr);
                        }
#endif
                    }
                    else if(type==CHANNEL_TYPE_OFFICIAL)
                    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        if (MailController::getInstance()->getIsNewMailListEnable()) {
                            CCArray* mailDataArr=CCArray::create();
                            int minCreateTime[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1};
                            for (int i=0; i<msgArr->count(); i++) {
                                item = _dict(msgArr->objectAtIndex(i));
                                
                                if(item->objectForKey("contentsArr"))
                                {
                                    CCArray* contentsArr = dynamic_cast<CCArray*>(item->objectForKey("contentsArr"));
                                    if(contentsArr)
                                    {
                                        std::string actualContent = "";
                                        CCLOGFUNCF("contentsArr size %d",contentsArr->count());
                                        for (int i=0; i<contentsArr->count(); i++) {
                                            CCString* content = dynamic_cast<CCString*>(contentsArr->objectAtIndex(i));
                                            if (content) {
                                                CCLOGFUNCF("content I :%s",content->getCString());
                                                actualContent.append(content->getCString());
                                            }
                                        }
                                        CCLOGFUNCF("actualContent :%s",actualContent.c_str());
                                        item->setObject(CCString::create(actualContent), "contents");
                                    }
                                }
                                
                                MailInfo* mailInfo=MailInfo::create();
                                mailInfo->parse(item);
                                CCLOGFUNCF("type : %d uid:%s",mailInfo->type,mailInfo->uid.c_str());
                                int tabIndex = mailInfo->tabType;
                                if(mailInfo->type == MAIL_RESOURCE)
                                    tabIndex = 6;
                                else if(mailInfo->type == MAIL_ATTACKMONSTER)
                                    tabIndex = 7;
                                else if(mailInfo->type == MAIL_RESOURCE_HELP)
                                    tabIndex = 8;
                                
                                if(minCreateTime[tabIndex]==-1)
                                    minCreateTime[tabIndex]=mailInfo->createTime;
                                else if(mailInfo->createTime<minCreateTime[tabIndex])
                                    minCreateTime[tabIndex]=mailInfo->createTime;
                                
                                if (item!=NULL) {
                                    mailDataArr->addObject(item);
                                }
                            }
                            
                            for (int i=1; i<=8; i++) {
                                if(minCreateTime[i]!=-1)
                                {
                                    //                            CCLOGFUNCF("minCreateTime[%d]:%d",i,minCreateTime[i]);
                                    string channelId="";
                                    switch (i) {
                                        case 1:
                                            channelId = "system";
                                            break;
                                        case 2:
                                            channelId ="notice";
                                            break;
                                        case 3:
                                            channelId = "studio";
                                            break;
                                        case 4:
                                            channelId = "fight";
                                            break;
                                        case 5:
                                            channelId = "mod";
                                            break;
                                        case 6:
                                            channelId = "resource";
                                            break;
                                        case 7:
                                            channelId = "monster";
                                            break;
                                        case 8:
                                            channelId = "resourcehelp";
                                            break;
                                        default:
                                            break;
                                    }
                                    if(channelId!="" && newMsgInfo.find(channelId)==string::npos)
                                    {
                                        string tempStr="4";
                                        tempStr.append("|").append(channelId).append("|").append(CC_ITOA(minCreateTime[i])).append("|-1");
                                        if (newMsgInfo=="") {
                                            newMsgInfo.append(tempStr);
                                        }
                                        else
                                        {
                                            newMsgInfo.append(",").append(tempStr);
                                        }
                                    }
                                    
                                }
                            }
                            
                            CCLOGFUNCF("mailDataArr siez:%d",mailDataArr->count());
                            if (mailDataArr->count()>0) {
                                if(mailDataArr->count()>20)
                                {
                                    
                                    CCArray* mailArr=CCArray::create();
                                    int count=0;
                                    for (int i=0; i<mailDataArr->count(); i++) {
                                        CCLOGFUNCF("mailArr size:%d",mailArr->count());
                                        if(mailArr->count()==20)
                                        {
                                            int index=MailController::getInstance()->getCurNotifyMailDataIndex();
                                            MailController::getInstance()->m_mailDataDic->setObject(mailArr, index);
                                            ChatServiceCocos2dx::notifyMailDataIndex(index, true);
                                            MailController::getInstance()->setCurNotifyMailDataIndex(index+1);
                                            mailArr=CCArray::create();
                                        }
                                        
                                        mailArr->addObject(mailDataArr->objectAtIndex(i));
                                    }
                                    
                                    if(mailArr->count()>0)
                                    {
                                        int index=MailController::getInstance()->getCurNotifyMailDataIndex();
                                        MailController::getInstance()->m_mailDataDic->setObject(mailArr, index);
                                        ChatServiceCocos2dx::notifyMailDataIndex(index, true);
                                        MailController::getInstance()->setCurNotifyMailDataIndex(index+1);
                                    }
                                    
                                }
                                else
                                {
                                    int index=MailController::getInstance()->getCurNotifyMailDataIndex();
                                    MailController::getInstance()->m_mailDataDic->setObject(mailDataArr, index);
                                    ChatServiceCocos2dx::notifyMailDataIndex(index, true);
                                    MailController::getInstance()->setCurNotifyMailDataIndex(index+1);
                                }
                            }
                        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
                            CCLog("iOS端原生邮件列表数据");
                            CCArray* mailInfoArray=CCArray::create();
                            CCDictionary *item = NULL;
                            for (int i=0; i<msgArr->count(); i++) {
                                item = _dict(msgArr->objectAtIndex(i));
                                MailInfo* mailInfo=MailInfo::create();
                                mailInfo->parse(item);
                                if (mailInfo!=NULL) {
                                    mailInfoArray->addObject(mailInfo);
                                }else {
                                    CCLog("system Mail 对象不存在" );
                                }
                            }
                            ChatServiceCocos2dx::pushSystemMailMsgWithMsgArray(mailInfoArray);
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
        if (newMsgInfo!="") {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ChatServiceCocos2dx::handleGetNewMailMsg(newMsgInfo);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
        ChatServiceCocos2dx::pushChatMailMsgWithArray(chatInfoArr_IOSMailList);
    }
#endif
        }
    }

    return true;
}