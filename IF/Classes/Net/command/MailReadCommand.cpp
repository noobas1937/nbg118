//
//  MailReadCommand.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-28.
//
//

#include "MailReadCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include "MailInfo.h"
#include "ChatMailInfo.h"

bool MailReadCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_READ) != 0)
        return false;
    GameController::getInstance()->removeWaitInterface();
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        
        if (m_isFromChat) {
            callSuccess(NetResult::create(Error_OK, (CCObject*)params));
            return true;
        }
        
        CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("result"));
        if(array!=NULL){
            int num = array->count();
            for (int i=0; i<num; i++) {
                auto dic = _dict(array->objectAtIndex(i));
                std::string id = dic->valueForKey("u")->getCString();
                auto oneMail = GlobalData::shared()->mailList.find(id);
                if(oneMail!=GlobalData::shared()->mailList.end()){
                    oneMail->second->isReadContent = true;
                    int oneType = oneMail->second->type;
                    if(oneType == MAIL_BATTLE_REPORT){
                        MailController::getInstance()->refreshMailContent(id, dic);
                    }else if(oneType == MAIL_RESOURCE){
                        MailController::getInstance()->refreshMailContent(id, dic);
                    }else if(oneType == MAIL_RESOURCE_HELP){
    
                    }else if(oneType == MAIL_GENERAL_TRAIN){
                        MailController::getInstance()->refreshGeneralTrainConten(id, dic);
                    }else if(oneType == MAIL_DETECT_REPORT){
                        MailController::getInstance()->refreshDetectContent(id, dic);
                    }else if(oneType == MAIL_ENCAMP){
                        MailController::getInstance()->refreshOcupyContnet(id, dic);
                    }else if(oneType == MAIL_DIGONG){
                        MailController::getInstance()->refreshDiGongContnet(id, dic);
                    }
                    else if(oneType == WORLD_NEW_EXPLORE){
                        MailController::getInstance()->refreshExploreContnet(id, dic);
                    }
                    else if(oneType == MAIL_ATTACKMONSTER){
//                        MailController::getInstance()->refreshAttackMonsterContnet(id, dic);
                    }else if(oneType==MAIL_ALLIANCEINVITE){
                        MailController::getInstance()->refreshAllianceInviteContnet(id,dic);
                    } else if (oneType == MAIL_REFUSE_ALL_APPLY) {
                        MailController::getInstance()->refreshRefuseAllApplyContent(id, dic);
                    } else if (oneType == MAIL_ALLIANCE_KICKOUT) {
                        MailController::getInstance()->refreshAllianceKickOutContent(id, dic);
                    } else if (oneType == MAIL_INVITE_TELEPORT) {
                        MailController::getInstance()->refreshInviteTeleportContent(id, dic);
                    }else if(oneType==MAIL_USER||oneType==MAIL_Alliance_ALL){
                        MailController::getInstance()->refreshMailDialogContent(id, dic);
                    }else if(oneType==MAIL_ALLIANCEAPPLY){
                        MailController::getInstance()->refreshAllianceApplyContnet(id,dic);
                    }else if(oneType==MAIL_WORLD_BOSS){
                        MailController::getInstance()->refreshWorldBossConten(id,dic);
                        MailController::getInstance()->refreshMailContent(id, dic->valueForKey("contents")->getCString());
                    }else{
                        MailController::getInstance()->refreshMailContent(id, dic->valueForKey("contents")->getCString());
                        if (dic->objectForKey("like")) {
                            int like = dic->valueForKey("like")->intValue();
                            oneMail->second->like = like;
                        }
                    }
                }
            }
        }
        callSuccess(NetResult::create(Error_OK, (CCObject*)params));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);

    }
    return true;
}

void MailReadCommand::setFromChat() {
    m_isFromChat = true;
}

bool MailReadStatusCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_READ_STATUS) != 0)
        return false;
    return true;
}
bool MailReadBatchCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_READ_BATCH) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    CCArray* mailArr = dynamic_cast<CCArray*>(params->objectForKey("result"));
    if(mailArr){
        MailResourceCellInfo* mailResourceCellInfo = NULL;
        MailMonsterCellInfo* mailMonsterCellInfo = NULL;
        MailResourceHelpCellInfo* mailResourceHelpCellInfo = NULL;
        if(m_type==5){
            map<string, MailInfo*>::iterator it;
            for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
                if(it->second->type==MAIL_RESOURCE){
                    mailResourceCellInfo = dynamic_cast<MailResourceCellInfo*>(it->second);
                    break;
                }
            }
        }else if(m_type==7){
            map<string, MailInfo*>::iterator it;
            for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
                if(it->second->type==MAIL_ATTACKMONSTER){
                    mailMonsterCellInfo = dynamic_cast<MailMonsterCellInfo*>(it->second);
                    break;
                }
            }
        }else if(m_type==8){
            map<string, MailInfo*>::iterator it;
            for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
                if(it->second->type==MAIL_RESOURCE_HELP){
                    mailResourceHelpCellInfo = dynamic_cast<MailResourceHelpCellInfo*>(it->second);
                    break;
                }
            }
        }
        int length = mailArr->count();
        CCDictionary* info = NULL;
        for(int i = 0; i < length; i++){
            info = _dict(mailArr->objectAtIndex(i));
            int type = info->valueForKey("type")->intValue();
            if(type==MAIL_RESOURCE_HELP&&m_type==8){
                MailController::getInstance()->addOneResourceHelpToMail(info, mailResourceHelpCellInfo);
            }else if(type==MAIL_RESOURCE&&m_type==5){
                MailController::getInstance()->addOneResourceToMail(info, mailResourceCellInfo);
            }else if(type==MAIL_ATTACKMONSTER&&m_type==7){
                MailController::getInstance()->addOneMonsterToMail(info, mailMonsterCellInfo);
            }else{
                MailController::getInstance()->addMail(info,true);
            }
        }
        if(length>0){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD
                                                                                   , CCInteger::create(length));
        }
    }
    
    
    return true;
}

//邮件历史数据接收函数
bool MailDialogReadBatchCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DIALOG_READ_BATCH) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        auto it = GlobalData::shared()->mailList.find(m_mailUid);
        if(it == GlobalData::shared()->mailList.end()){
            return true;
        }
        CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("result"));
        
        if(array==NULL)
        {
            CCLOG("MailDialogReadBatchCommand array==NULL");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            ChatServiceCocos2dx::postNoMoreMessage(3);
#endif
            return false;
        }
        
        if(array!=NULL){
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
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CCArray* mailInfoArr=CCArray::create();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            CCArray* mailInfoArr=CCArray::create();
#endif
            
            for (int i=0; i<num; i++) {
                auto dic = _dict(array->objectAtIndex(i));
                
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if(ChatServiceCocos2dx::isChatShowing && ChatServiceCocos2dx::enableNativeMail)
                {
                    MailDialogInfo* dialogInfo = MailDialogInfo::create();
                    dialogInfo->parse(dic);
//                    dynamic_cast<MailInfo*>(it->second)->dialogs->insertObject(dialogInfo, 0);
                    ChatMailInfo* info=ChatMailInfo::create(dynamic_cast<MailInfo*>(it->second),dialogInfo,false);
                    mailInfoArr->addObject(info);
                }
                else
                    MailController::getInstance()->addOneDialogToMail(it->second,dic,false);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                if(ChatServiceCocos2dx::isChatShowing_fun() && ChatServiceCocos2dx::enableNativeChat)
                {
                    MailDialogInfo* dialogInfo = MailDialogInfo::create();
                    dialogInfo->parse(dic);
                    //                    dynamic_cast<MailInfo*>(it->second)->dialogs->insertObject(dialogInfo, 0);
                    ChatMailInfo* info=ChatMailInfo::create(dynamic_cast<MailInfo*>(it->second),dialogInfo,false);
                    mailInfoArr->addObject(info);
                }
                else
                    MailController::getInstance()->addOneDialogToMail(it->second,dic,false);
#endif
            }

            if(num>0){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if(ChatServiceCocos2dx::enableNativeMail)
                {
                    CCLOG("ChatServiceCocos2dx::m_curSendMailIndex %d",ChatServiceCocos2dx::m_curSendMailIndex);
                    MailController::getInstance()->notifyMailMsgToAndroid(mailInfoArr,dynamic_cast<MailInfo*>(it->second)->fromUid,dynamic_cast<MailInfo*>(it->second)->fromName);
                }
                else
                {
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD
                                                                                           , CCInteger::create(num));
                }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                if(ChatServiceCocos2dx::enableNativeChat)
                {
                    MailController::getInstance()->notifyMailMsgToIOS(mailInfoArr,CHANNEL_TYPE_USER,dynamic_cast<MailInfo*>(it->second)->fromUid);
                }
                else
                {
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD
                                                                                           , CCInteger::create(num));
                }
                
#endif
                
            }
            else
            {
                
            }
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(!ChatServiceCocos2dx::isChatShowing)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        }
#else
        if (!ChatServiceCocos2dx::isChatShowing_fun()) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        }
#endif
        
    }
    return true;
}
bool MailDialogTranslateCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DIALOG_TRANSLATE) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
//        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("result"));
        MailController::getInstance()->backInitTranslateMails(array,m_refUIDs);
        callSuccess(NetResult::create(Error_OK, (CCObject*)params));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        
    }
    return true;
}