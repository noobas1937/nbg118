//
//  MailController.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-28.
//
//

#include "MailController.h"
#include "CCSafeNotificationCenter.h"
#include "MailDeleteCommand.h"
#include "MailReadCommand.h"
#include "MailSendCommand.h"
#include "MailDeleteAllCommand.h"
#include "MailPopUpView.h"
#include "LocalController.h"
#include "WorldController.h"
#include "RewardController.h"
#include "MailDialogInfo.h"
#include "MailDialogView.h"
#include "PopupViewController.h"
#include "MailResourceCellInfo.h"
#include "MailResourceInfo.h"
#include "MailMonsterCellInfo.h"
#include "MailMonsterInfo.h"
#include "MailResourceHelpCellInfo.h"
#include "MailResourceHelpInfo.h"
#include "MailBatchCommand.h"
#include "MailGetRewardCommand.h"
#include "ChatServiceCocos2dx.h"
#include "YesNoDialog.h"
#include "ChatMailInfo.h"
#include "CreateChatRoomCommand.h"
#include "InviteChatRoomMemberCommand.h"
#include "KickChatRoomMemberCommand.h"
#include "ModifyChatRoomNameCommand.h"
#include "QuitChatRoomCommand.h"
#include "SendChatRoomMsgCommand.h"
#include "GetChatRoomMsgCommand.h"
#include "GetChatRoomListCommand.h"
#include "GetAllianceMembersCommand.h"
#include "GetInviteeCommand.h"
#include "ChatController.h"
#include "QuitChatRoomArrayCommand.h"
#include "GetChatRoomMemberCommand.h"
#include "WorldSearchUserCommand.h"
#include "BattleReportMailPopUpView.h"
#include "MailResourcePopUpView.h"
#include "MailResourceHelpView.h"
#include "MailReadPopUpView.h"
#include "OccupyMailPopUpView.h"
#include "ExplorePopUpView.h"
#include "MailAnnouncePopUp.h"
#include "MailMonsterListView.h"
#include "MailGiftView.h"
#include "WorldBossRewardMailView.h"
#include "DetectMailPopUpView.h"
#include "MailReadPopUpView.h"
#include "WorldBossMailView.h"
#include "MonsterAttackPopUpView.h"
#include "GetUpdateMailCommand.h"
#include "UIComponent.h"
#include "ResourceMailPopUpView.h"
#include "CCCommonUtils.h"

static const char* roman[30] = {"I","II","III","IV","V","VI","VII","VIII","IX","X","XI","XII","XIII","XIV","XV","XVI","XVII","XVIII","XIX","XX",
    "XXI","XXII","XXIII","XXIV","XXV","XXVI","XXVII","XXVIII","XXIX","XXX"
};

static MailController* _instance;

MailController::MailController():m_isTransInit(false),m_isShowWarning(false),mTransMailDialog(NULL),m_chatRoomName(""),m_inviteMemberName(""),m_inviteMemberUid(""),m_isSearchUser(false),m_curNotifyMailDataIndex(0),m_newMailUnreadNum(0){
    CanRemoveMail = false;
    m_mailInfoSendDic=CCDictionary::create();
    m_mailInfoSendDic->retain();
    m_allianceMemberInfoDic=CCDictionary::create();
    m_allianceMemberInfoDic->retain();
    m_chatRoomMemberArr=CCArray::create();
    m_chatRoomMemberArr->retain();
    m_chatRoomIdArray=CCArray::create();
    m_chatRoomIdArray->retain();
    m_tempChatRoomMsgDic=CCDictionary::create();
    m_tempChatRoomMsgDic->retain();
    m_mailDataDic=CCDictionary::create();
    m_mailDataDic->retain();
    m_isChatRoomEnable=false;
    m_isNewMailListEnable=false;
    m_isNewMailUIEnable = false;
}

MailController::~MailController() {
    CC_SAFE_RELEASE_NULL(m_allianceMemberInfoDic);
    CC_SAFE_RELEASE_NULL(m_mailInfoSendDic);
    CC_SAFE_RELEASE_NULL(m_chatRoomMemberArr);
    CC_SAFE_RELEASE_NULL(m_chatRoomIdArray);
    CC_SAFE_RELEASE_NULL(m_tempChatRoomMsgDic);
    CC_SAFE_RELEASE_NULL(m_mailDataDic);
}

MailController* MailController::getInstance(){
    if(!_instance){
        _instance = new MailController();
    }
    return _instance;
}

void MailController::addMail(cocos2d::CCDictionary *dic,bool isreadContent,bool postNotification){
    addMailToList(dic,isreadContent);
    if(isreadContent==false && postNotification == true){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
    }
}

void MailController::addMails(cocos2d::CCArray *arr,bool isreadContent){
    int length = arr->count();
    CCDictionary* info = NULL;
    for(int i = 0; i < length; i++){
        info = _dict(arr->objectAtIndex(i));
        addMailToList(info,isreadContent);
        CC_SAFE_RELEASE(info);
    }
    CC_SAFE_RELEASE(arr);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
}

void MailController::addMailFromAndroidToList(cocos2d::CCDictionary *dic,bool isreadContent){
    int type = dic->valueForKey("type")->intValue();
    MailInfo* mail =NULL;
    if(type==MAIL_RESOURCE){
        mail = createResourceCellInfo(dic);
    }else if(type==MAIL_RESOURCE_HELP){
        MailResourceHelpCellInfo* mailResourceHelpCellInfo= MailResourceHelpCellInfo::create();
        mailResourceHelpCellInfo->parse(dic);
        mail = mailResourceHelpCellInfo;
    }else if(type==MAIL_ATTACKMONSTER){
        MailMonsterCellInfo* mailMonster= MailMonsterCellInfo::create();
        mailMonster->parse(dic);
        mail = mailMonster;
    }else{
         mail = MailInfo::create();
        if(type == MAIL_BATTLE_REPORT){
            if(dic->objectForKey("attualContent"))
            {
                CCLOGFUNCF("attualContent");
                dic->setObject(dynamic_cast<CCDictionary*>(dic->objectForKey("attualContent")), "contents");
            }
        }
        mail->parse(dic);
        
    }
    
    if (type == MAIL_SYSUPDATE) {
        mail->version = dic->valueForKey("fromUid")->getCString();
    }
    mail->retain();
    mail->isReadContent =isreadContent;
    CCLOGFUNCF("UID %s",dic->valueForKey("uid")->getCString());
    GlobalData::shared()->mailList[dic->valueForKey("uid")->getCString()] = mail;
    if(isreadContent){
        CCLOGFUNCF("type : %d",mail->type);
        string id = mail->uid;
        CCDictionary* params = _dict(dic->objectForKey("detail"));
        if(params)
        {
            CCLOGFUNC("params!=NULL");
            if(mail->type == MAIL_BATTLE_REPORT){
                CCArray* dfWarEffectArr=dynamic_cast<CCArray*>(params->objectForKey("dfWarEffect"));
                CCArray* warEffArr=parseWarEffect(dfWarEffectArr);
                CCLOGFUNCF("dfWarEffectArr size :%d",warEffArr->count());
                params->setObject(warEffArr, "dfWarEffect");
                
                CCArray* atkWarEffectArr=dynamic_cast<CCArray*>(params->objectForKey("atkWarEffect"));
                CCArray* atkEffArr=parseWarEffect(atkWarEffectArr);
                CCLOGFUNCF("atkWarEffectArr size :%d",atkEffArr->count());
                params->setObject(atkEffArr, "atkWarEffect");
                
                parseBattleContentAbility(dynamic_cast<CCArray*>(params->objectForKey("defGen")));
                parseBattleContentAbility(dynamic_cast<CCArray*>(params->objectForKey("atkGen")));
                
                refreshMailContent(id, params);
            }else if(mail->type  == MAIL_GENERAL_TRAIN){
                refreshGeneralTrainConten(id, params);
            }else if(mail->type  == MAIL_DETECT_REPORT){
                parseAbility(params);
                refreshDetectContent(id, params);
            }else if(mail->type  == MAIL_ENCAMP){
                refreshOcupyContnet(id, params);
            }else if(mail->type  == MAIL_DIGONG){
                refreshDiGongContnet(id, params);
            }else if(mail->type == WORLD_NEW_EXPLORE){
                CCDictionary* trapDic=CCDictionary::create();
                CCString* trap=dynamic_cast<CCString*>(dic->objectForKey("trap"));
                if (trap) {
                    string trapStr=trap->getCString();
                    if (trapStr!="") {
                        CCLOGFUNCF("trapStr:%s",trapStr.c_str());
                        int startPos=trapStr.find_first_of("{");
                        int endPos = trapStr.find_last_of("}");
                        int len=endPos-startPos-1;
                        trapStr = CCCommonUtils::subStrByUtf8(trapStr, startPos+1, len);
                        CCLOGFUNCF("trapStr2:%s",trapStr.c_str());
                        vector<std::string> vector1;
                        CCCommonUtils::splitString(trapStr, ",", vector1);
                        int num = vector1.size();
                        for (int i = 0; i < num; i++) {
                            if (vector1[i]!="") {
                                vector<std::string> vector2;
                                CCCommonUtils::splitString(vector1[i], "|", vector2);
                                if (vector2.size() == 2) {
                                    if (vector2[0]!="") {
                                        trapDic->setObject(CCString::create(vector2[1]), vector2[0]);
                                    }
                                }
                            }
                        }
                    }
                }
                params->setObject(trapDic, "trap");
                refreshExploreContnet(id, params);
            }else if(mail->type ==MAIL_ALLIANCEINVITE){
                refreshAllianceInviteContnet(id, params);
            } else if (mail->type == MAIL_ALLIANCE_KICKOUT) {
                refreshAllianceKickOutContent(id, params);
            } else if (mail->type == MAIL_INVITE_TELEPORT) {
                refreshInviteTeleportContent(id, params);
            }
            else if(mail->type == MAIL_ALLIANCEAPPLY){
                refreshAllianceApplyContnet(id,params);
            }
            else if(mail->type == MAIL_REFUSE_ALL_APPLY) {
                refreshRefuseAllApplyContent(id, params);
            }
            else if(mail->type==MAIL_WORLD_BOSS){
                refreshWorldBossConten(id,params);
                refreshMailContent(id, dic->valueForKey("contents")->getCString());
            }
            else{
                refreshMailContent(id, dic->valueForKey("contents")->getCString());
            }
        }
        else
        {
            CCLOGFUNC("params==NULL");
            refreshMailContent(id, dic->valueForKey("contents")->getCString());
        }
    }
}


CCArray* MailController::parseWarEffect(CCArray* effectArr)
{
    CCArray* arr = CCArray::create();
    arr->retain();
    if (effectArr) {
        for (int i=0; i<effectArr->count(); i++) {
            CCString* effStr=dynamic_cast<CCString*>(effectArr->objectAtIndex(i));
            if (effStr) {
                string eff=effStr->getCString();
                if (eff!="") {
//                    CCLOGFUNCF("eff:%s",eff.c_str());
                    int startPos=eff.find_first_of("{");
                    int endPos = eff.find_last_of("}");
                    int len=endPos-startPos-1;
                    if(len<= 0)
                        continue;
                    eff = CCCommonUtils::subStrByUtf8(eff, startPos+1, len);
//                    CCLOGFUNCF("eff2:%s",eff.c_str());
                    vector<std::string> vector1;
                    CCCommonUtils::splitString(eff, ":", vector1);
                    int num = vector1.size();
//                    CCLOGFUNCF("num:%d",num);
                    if (num==2 && vector1[0]!="") {
                        CCDictionary* dic=CCDictionary::create();
                        string key = vector1[0];
                        int length = key.length();
                        if(length>=2)
                            key = CCCommonUtils::subStrByUtf8(key, 1, length-2);
//                        CCLOGFUNCF("vector1[1]:%s  key:%s",vector1[1].c_str(),key.c_str());
                        
                        dic->setObject(CCString::create(vector1[1]), key);
                        arr->addObject(dic);
                    }
                }
            }
        }
    }
    return arr;
}


void MailController::parseAbility(CCDictionary* dic)
{
    if (dic==NULL) {
        return;
    }
    CCArray* abilityArr=dynamic_cast<CCArray*>(dic->objectForKey("ability"));
   
    CCArray* ablityArray=CCArray::create();
    ablityArray->retain();
    if(abilityArr)
    {
        for (int j=0; j<abilityArr->count(); j++) {
            CCString* abilityStr=dynamic_cast<CCString*>(abilityArr->objectAtIndex(j));
            if (abilityStr) {
                string ability=abilityStr->getCString();
                if (ability!="") {
                    //                            CCLOGFUNCF("ability:%s",ability.c_str());
                    int startPos=ability.find_first_of("{");
                    int endPos = ability.find_last_of("}");
                    int len=endPos-startPos-1;
                    if(len<=0)
                        continue;
                    ability = CCCommonUtils::subStrByUtf8(ability, startPos+1, len);
                    //                            CCLOGFUNCF("ability2:%s",ability.c_str());
                    vector<std::string> vector1;
                    CCCommonUtils::splitString(ability, ",", vector1);
                    int num = vector1.size();
                    //                            CCLOGFUNCF("ability2 num:%d",num);
                    for (int k= 0; k< num; k++) {
                        if (vector1[k]!="") {
                            //                                    CCLOGFUNCF("vector1[k]:%s",vector1[k].c_str());
                            vector<std::string> vector2;
                            CCCommonUtils::splitString(vector1[k], ":", vector2);
                            if (vector2.size() == 2) {
                                if (vector2[0]!="") {
                                    CCDictionary* dic=CCDictionary::create();
                                    string key = vector2[0];
                                    int length = key.length();
                                    if(length>=2)
                                        key = CCCommonUtils::subStrByUtf8(key, 1, length-2);
                                    //                                            CCLOGFUNCF("vector2[1]:%s  key:%s",vector2[1].c_str(),key.c_str());
                                    dic->setObject(CCString::create(vector2[1]), key);
                                    ablityArray->addObject(dic);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    dic->setObject(ablityArray, "ability");
}

void MailController::parseBattleContentAbility(CCArray* genArr)
{
    CCLOGFUNC("");
    if (genArr) {
        CCLOGFUNCF("genArr count: %d",genArr->count());
        for (int i=0; i<genArr->count(); i++) {
            CCDictionary* tempDefGenDic=dynamic_cast<CCDictionary*>(genArr->objectAtIndex(i));
            if (tempDefGenDic)
            {
                parseAbility(tempDefGenDic);
            }
        }
    }
}

void MailController::addMailToList(cocos2d::CCDictionary *dic,bool isreadContent){
    int type = dic->valueForKey("type")->intValue();
    MailInfo* mail =NULL;
    if(type==MAIL_RESOURCE){
       mail = createResourceCellInfo(dic);
    }else if(type==MAIL_RESOURCE_HELP){
        MailResourceHelpCellInfo* mailResourceHelpCellInfo= MailResourceHelpCellInfo::create();
        mailResourceHelpCellInfo->parse(dic);
        mail = mailResourceHelpCellInfo;
    }else if(type==MAIL_ATTACKMONSTER){
        MailMonsterCellInfo* mailMonster= MailMonsterCellInfo::create();
        mailMonster->parse(dic);
        mail = mailMonster;
    }else{
        mail = MailInfo::create();
        mail->parse(dic);
    }
    
    mail->retain();
    mail->isReadContent =isreadContent;
    CCLOGFUNCF("UID %s",dic->valueForKey("uid")->getCString());
    GlobalData::shared()->mailList[dic->valueForKey("uid")->getCString()] = mail;
    if(isreadContent){
        CCLOGFUNCF("type : %d",type);
        string id = mail->uid;
        CCDictionary* params = _dict(dic->objectForKey("detail"));
        if(mail->type == MAIL_BATTLE_REPORT){
            refreshMailContent(id, params);
        }else if(mail->type  == MAIL_RESOURCE){
            //refreshMailContent(id, params);
        }else if(mail->type  == MAIL_RESOURCE_HELP){
            //refreshMailContent(id, params);
        }else if(mail->type  == MAIL_GENERAL_TRAIN){
            refreshGeneralTrainConten(id, params);
        }else if(mail->type  == MAIL_DETECT_REPORT){
            refreshDetectContent(id, params);
        }else if(mail->type  == MAIL_ENCAMP){
            refreshOcupyContnet(id, params);
        }else if(mail->type  == MAIL_DIGONG){
            refreshDiGongContnet(id, params);
        }else if(mail->type == WORLD_NEW_EXPLORE){
            refreshExploreContnet(id, params);
        }else if(mail->type == MAIL_ATTACKMONSTER){
//            refreshAttackMonsterContnet(id, params);
        }else if(mail->type ==MAIL_ALLIANCEINVITE){
            refreshAllianceInviteContnet(id, params);
        } else if (mail->type == MAIL_ALLIANCE_KICKOUT) {
            refreshAllianceKickOutContent(id, params);
        } else if (mail->type == MAIL_REFUSE_ALL_APPLY) {
            refreshRefuseAllApplyContent(id, params);
        } else if (mail->type == MAIL_INVITE_TELEPORT) {
            refreshInviteTeleportContent(id, params);
        }else if(mail->tabType==USERMAIL){
            //int unreadNum = mail->unreadDialogNum;
           // GlobalData::shared()->mailCountObj.perR +=unreadNum;
           // GlobalData::shared()->mailCountObj.perT +=1;
            
//            MailDialogInfo* dialogInfo = MailDialogInfo::create();
//            dialogInfo->parse(dic);
//            mail->dialogs->insertObject(dialogInfo, 0);
        }
        else{
            refreshMailContent(id, params->valueForKey("contents")->getCString());
        }
    }
    if(mail->status==0&&!isreadContent){
        if(mail->tabType==3){
            GlobalData::shared()->mailCountObj.studioT +=1;
            GlobalData::shared()->mailCountObj.studioR +=1;
        }else if(mail->type== MAIL_FRESHER||mail->type==MAIL_SYSNOTICE||mail->type==MAIL_SYSUPDATE){
        GlobalData::shared()->mailCountObj.saveR +=1;
        GlobalData::shared()->mailCountObj.saveT +=1;
      }else if(mail->tabType==USERMAIL){
        //  mail->unreadDialogNum+=1;
//          GlobalData::shared()->mailCountObj.perR +=1;
//          GlobalData::shared()->mailCountObj.perT +=1;
//          
//          MailDialogInfo* dialogInfo = MailDialogInfo::create();
//          dialogInfo->parse(dic);
//          mail->dialogs->insertObject(dialogInfo, 0);
//          CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
      }else if(mail->tabType==4){
          GlobalData::shared()->mailCountObj.fightR +=1;
          GlobalData::shared()->mailCountObj.fightT +=1;
      }else if(mail->tabType == MAILTAB5){
          GlobalData::shared()->mailCountObj.modR +=1;
          GlobalData::shared()->mailCountObj.modT +=1;
      }else{
          GlobalData::shared()->mailCountObj.sysR +=1;
          GlobalData::shared()->mailCountObj.sysT +=1;
      }
        if(mail->type==MAIL_SYSUPDATE){
            GlobalData::shared()->mailCountObj.upR +=1;
        }
    }
}
MailResourceCellInfo* MailController::createResourceCellInfo(CCDictionary* dic){
    MailResourceCellInfo* mail = MailResourceCellInfo::create();
    mail->parse(dic);
    return mail;

}
void MailController::pushMailResourceFirst(CCDictionary *dic){
    MailResourceCellInfo* mail = MailResourceCellInfo::create();
    mail->parse(dic);
    mail->retain();
    mail->isReadContent =false;
    if(mail->unread==0){
        mail->unread = 1;
    }
    if(mail->totalNum==0){
        mail->totalNum = 1;
    }
    GlobalData::shared()->mailList[dic->valueForKey("uid")->getCString()] = mail;
    
    if(mail->status==0){
        if(mail->type ==MAIL_RESOURCE){
            GlobalData::shared()->mailCountObj.sysR +=1;
            if(mail->totalNum==1){//确保登录时，没有包含
                GlobalData::shared()->mailCountObj.sysT +=1;
            }
            
//            MailDialogInfo* dialogInfo = MailDialogInfo::create();
//            dialogInfo->parse(dic);
//            mail->dialogs->insertObject(dialogInfo, 0);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_RESOURCE_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        }
    }
    
}
void MailController::pushMailResourceHelpFirst(CCDictionary *dic){
    MailResourceHelpCellInfo* mail = MailResourceHelpCellInfo::create();
    mail->parse(dic);
    mail->retain();
    mail->isReadContent =false;
    if(mail->unread==0){
        mail->unread = 1;
    }
    if(mail->totalNum==0){
        mail->totalNum = 1;
    }
    GlobalData::shared()->mailList[dic->valueForKey("uid")->getCString()] = mail;
    
    if(mail->status==0){
        if(mail->type ==MAIL_RESOURCE_HELP){
            GlobalData::shared()->mailCountObj.sysR +=1;
            if(mail->totalNum==1){//确保登录时，没有包含
                GlobalData::shared()->mailCountObj.sysT +=1;
            }
            
            //            MailDialogInfo* dialogInfo = MailDialogInfo::create();
            //            dialogInfo->parse(dic);
            //            mail->dialogs->insertObject(dialogInfo, 0);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_RESOURCE_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        }
    }
    
}
void MailController::pushMailMonsterFirst(CCDictionary *dic){
    MailMonsterCellInfo* mail = MailMonsterCellInfo::create();
    mail->parse(dic);
    mail->retain();
    mail->isReadContent =false;
    if(mail->unread==0){
        mail->unread = 1;
    }
    if(mail->totalNum==0){
        mail->totalNum = 1;
    }
    GlobalData::shared()->mailList[dic->valueForKey("uid")->getCString()] = mail;
    
    if(mail->status==0){
        if(mail->type ==MAIL_ATTACKMONSTER){
            GlobalData::shared()->mailCountObj.sysR +=1;
            if(mail->totalNum==1){//确保登录时，没有包含
                GlobalData::shared()->mailCountObj.sysT +=1;
            }
            
            //            MailDialogInfo* dialogInfo = MailDialogInfo::create();
            //            dialogInfo->parse(dic);
            //            mail->dialogs->insertObject(dialogInfo, 0);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_MONSTER_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        }
    }
    
}

void MailController::handleMailMessage(MailInfo *mailInfo, MailDialogInfo *mailDialogInfo,bool isNew,int msgType/*=CHANNEL_TYPE_USER*/)
{
    CCLOGFUNC("");
    CCArray* mailInfoArr=CCArray::create();
    ChatMailInfo* info=ChatMailInfo::create(mailInfo,mailDialogInfo,isNew,msgType);
    mailInfoArr->addObject(info);
    bool isModMsg = info->post == 200?true:false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (mailInfo->type==CHAT_ROOM) {
        CCLOGFUNC("mailInfo->type==CHAT_ROOM");
        notifyMailMsgToAndroid(mailInfoArr,mailInfo->crGroupId,mailInfo->fromName);
    }
    else
    {
        notifyMailMsgToAndroid(mailInfoArr,mailInfo->fromUid,mailInfo->fromName,isModMsg);
    }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (mailInfo->type==CHAT_ROOM) {
        CCLOGFUNC("mailInfo->type==CHAT_ROOM");
        notifyMailMsgToIOS(mailInfoArr , CHANNEL_TYPE_CHATROOM , mailInfo->crGroupId);
    }
    else
    {
        notifyMailMsgToIOS(mailInfoArr , CHANNEL_TYPE_USER , mailInfo->fromUid);
    }
    
#endif
}

void MailController::handleMailMessageForOpen(MailInfo* m_mailInfo)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    
        CCArray* mailInfoArr=CCArray::create();
        CCObject* obj;
        CCARRAY_FOREACH(m_mailInfo->dialogs, obj){
            MailDialogInfo* tempDialogInfo = dynamic_cast<MailDialogInfo*>(obj);
            ChatMailInfo* info=ChatMailInfo::create(m_mailInfo,tempDialogInfo,true);
            mailInfoArr->addObject(info);
        }
    
        CCLOGFUNCF("type %d",m_mailInfo->type);
        ChatServiceCocos2dx::setMailInfo(m_mailInfo->fromUid.c_str(),m_mailInfo->uid.c_str(),m_mailInfo->fromName.c_str(),m_mailInfo->type);

    bool isModMsg = false;
    if(m_mailInfo->type == MAIL_MOD_PERSONAL || m_mailInfo->type == MAIL_MOD_SEND)
        isModMsg = true;
    
    if(mailInfoArr->count()>20)
    {
        CCArray* mailArr=CCArray::create();
        int count=0;
        for (int i=0; i<mailInfoArr->count(); i++) {
            if(count==20)
            {
                MailController::getInstance()->m_mailInfoSendDic->setObject(mailArr, ChatServiceCocos2dx::m_curSendMailIndex);
                
                ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,m_mailInfo->fromUid,m_mailInfo->fromName,isModMsg);
                ChatServiceCocos2dx::m_curSendMailIndex++;
                mailArr->removeAllObjects();
                count=0;
            }
            mailArr->addObject(mailInfoArr->objectAtIndex(i));
            count++;
        }
        
        if(mailArr->count()>0)
        {
            MailController::getInstance()->m_mailInfoSendDic->setObject(mailArr, ChatServiceCocos2dx::m_curSendMailIndex);
            ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,m_mailInfo->fromUid,m_mailInfo->fromName,isModMsg);
            ChatServiceCocos2dx::m_curSendMailIndex++;
        }
        
    }
    else
    {
        if(mailInfoArr->count()>0)
        {
            CCLOG("mailInfoArr->count():%d",mailInfoArr->count());
            MailController::getInstance()->m_mailInfoSendDic->setObject(mailInfoArr, ChatServiceCocos2dx::m_curSendMailIndex);
            ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,m_mailInfo->fromUid,m_mailInfo->fromName,isModMsg);
            ChatServiceCocos2dx::m_curSendMailIndex++;
        }
    }

    
        CCLOG("!ChatServiceCocos2dx::isChatShowing");
//        ChatServiceCocos2dx::m_isInMailDialog=true;
    ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_USER;
//        ChatServiceCocos2dx::m_curMailUid=m_mailInfo->uid;
        ChatServiceCocos2dx::m_isNoticItemUsed=false;
        MailController::getInstance()->setOldOpenMailInfo(m_mailInfo);
        if(!ChatServiceCocos2dx::isChatShowing){
            ChatServiceCocos2dx::showChatActivityFrom2dx();
        }
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)    
    CCLOG("showReadPopUp USERMAIL");

    //邮件入口
    CCArray* mailInfoArr=CCArray::create();
    CCObject* obj;
    CCARRAY_FOREACH(m_mailInfo->dialogs, obj){
        MailDialogInfo* tempDialogInfo = dynamic_cast<MailDialogInfo*>(obj);
        ChatMailInfo* info=ChatMailInfo::create(m_mailInfo,tempDialogInfo,true);
        mailInfoArr->addObject(info);
    }
    CCLOG("showReadPopUp USERMAIL");
    
    if(mailInfoArr->count()>0)
    {
        CCLOG("mailInfoArr->count():%d",mailInfoArr->count());
        ChatServiceCocos2dx::removeMail2fromUid(m_mailInfo->fromUid.c_str());
        ChatServiceCocos2dx::handleChatPush(mailInfoArr, CHANNEL_TYPE_USER ,m_mailInfo->fromUid.c_str());
    }
    
    //邮件入口cell
    
    CCLog("%s",m_mailInfo->fromUid.c_str());
    CCLog("%s",m_mailInfo->uid.c_str());
    CCLog("%s",m_mailInfo->fromName.c_str());
    
    ChatServiceCocos2dx::setMailInfo(m_mailInfo->fromUid.c_str(),m_mailInfo->uid.c_str(),m_mailInfo->fromName.c_str(),m_mailInfo->type);
    ChatServiceCocos2dx::m_isInMailDialog=true;
    MailController::getInstance()->setOldOpenMailInfo(m_mailInfo);
//    ChatServiceCocos2dx::initMailData(mailInfoArr);
    ChatServiceCocos2dx::m_channelType = CHANNEL_TYPE_USER;
    ChatServiceCocos2dx::showChatIOSFrom2dx();
    ChatServiceCocos2dx::m_curMailFromUid=m_mailInfo->fromUid;
    
#endif

}

void MailController::pushMailDialogFirst(CCDictionary* dic){
    MailInfo* mail = MailInfo::create();
    mail->parse(dic);
    mail->retain();
    mail->isReadContent =false;
    GlobalData::shared()->mailList[dic->valueForKey("uid")->getCString()] = mail;

    if(mail->status==0){
        if(mail->tabType ==USERMAIL){
//            mail->unreadDialogNum+=1;
//            mail->dialogNum+=1;
            GlobalData::shared()->mailCountObj.perR +=1;
            GlobalData::shared()->mailCountObj.perT +=1;
            
        }else if (mail->tabType == MAILTAB5){
//            mail->unreadDialogNum+=1;
//            mail->dialogNum+=1;
            GlobalData::shared()->mailCountObj.modR +=1;
            GlobalData::shared()->mailCountObj.modT +=1;

        }
        
        MailDialogInfo* dialogInfo = MailDialogInfo::create();
        dialogInfo->parse(dic);
        mail->dialogs->insertObject(dialogInfo, 0);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        if(ChatServiceCocos2dx::enableNativeMail)
        {
            handleMailMessage(mail,dialogInfo,true);
        }
        if(!ChatServiceCocos2dx::isChatShowing)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
        }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(ChatServiceCocos2dx::enableNativeChat)
        {
            handleMailMessage(mail,dialogInfo,true);
        }
        if(!ChatServiceCocos2dx::isChatShowing_fun())
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
        }
#endif
    }

}

void MailController::exitMailDialog(MailInfo* mailInfo){
    
    if(mailInfo->unreadDialogNum)
        CCLOGFUNCF("mailInfo->unreadDialogNum %d",mailInfo->unreadDialogNum);
    if(mailInfo->unreadDialogNum>0){
        MailDialogReadCommand* cmd = new MailDialogReadCommand(mailInfo->fromUid, -1, mailInfo->uid);
        cmd->sendAndRelease();
        if(mailInfo->type == MAIL_MOD_SEND || mailInfo->type == MAIL_MOD_PERSONAL){
            GlobalData::shared()->mailCountObj.modR -= mailInfo->unreadDialogNum;
            if(GlobalData::shared()->mailCountObj.modR < 0){
                GlobalData::shared()->mailCountObj.modR = 0;
            }
        }else{
            GlobalData::shared()->mailCountObj.perR -=mailInfo->unreadDialogNum;
        }
        
        for (auto search=GlobalData::shared()->mailList.begin(); search!=GlobalData::shared()->mailList.end(); search++) {
            MailInfo* info=dynamic_cast<MailInfo*>(search->second);
            if(info!=NULL && info->fromUid==mailInfo->fromUid)
            {
                CCLOGFUNC("exitMailDialog");
                info->unreadDialogNum -=mailInfo->unreadDialogNum;
            }
        }
        
//        mailInfo->unreadDialogNum = 0;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
}


void MailController::updateMailList()
{
    CCLOGFUNC("updateMailList");
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(m_isNewMailListEnable)
        return;
#endif
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
        return;
    }
#endif
    if (m_oldOpenMailInfo) {
        exitMailDialog(m_oldOpenMailInfo);
        m_oldOpenMailInfo=NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
}

//0玩家邮件 1系统邮件 2保存邮件
bool MailController::isMailFull(int type){
    map<std::string, MailInfo*>::iterator it = it;
    int maxNum = 0;
    if(type == SAVEMAIL){
        maxNum = GlobalData::shared()->mailConfig.saveMailMax;
    }else if(type == SYSTEMMAIL){
        maxNum = GlobalData::shared()->mailConfig.sysMailMax;
    }else if(type == USERMAIL){
        maxNum = GlobalData::shared()->mailConfig.userMailMax;
    }
    
    int count = 0;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        if(it->second->save != 0){
            if(type == SAVEMAIL){
                count++;
            }
        }else if(it->second->tabType ==USERMAIL){
            if(type == SYSTEMMAIL){
                count++;
            }
        }else{
            if(type == USERMAIL){
                count++;
            }
        }
    }
    return count >= maxNum;
}

void MailController::removeMail(std::string uids){
    if(uids != ""){
        vector<std::string> vector;
        CCCommonUtils::splitString(uids, ",", vector);
        int i = 0;
        while(i < vector.size()){
            std::string uid = vector[i];
            MailInfo* mail = GlobalData::shared()->mailList.find(uid)->second;
            if(mail->save != 0){
                mail->save = 0;
            }else{
                if(mail->save == 1){
                    mail->save = 2;
                }else{
                    CC_SAFE_RELEASE(mail);
                    GlobalData::shared()->mailList.erase(uid);
                }
            }
            i++;
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
    }
}
int MailController::getCountByType(int type){
    int count = 0;
    
    map<string, MailInfo*>::iterator it;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        if(it->second->tabType==type){
            count++;
        }else
        if(it->second->type== MAIL_FRESHER||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
            if(type==SAVEMAIL){
                count++;
            }
            
        }else if(it->second->tabType==USERMAIL){
            if(type==USERMAIL){
                count++;
            }
        }else{
            if(type==SYSTEMMAIL){
                count++;
            }
        }
    }
    
    
    
    return count;
}
void MailController::removeOneMailResource(std::string mailUid,std::string dialogUid,std::string type){
    map<string, MailInfo*>::iterator it;
    it = GlobalData::shared()->mailList.find(mailUid);
    if (GlobalData::shared()->mailList.end() != it) {
        MailResourceCellInfo* info = dynamic_cast<MailResourceCellInfo*>(it->second);
        CCObject* obj;
        CCARRAY_FOREACH(info->collect, obj){
            MailResourceInfo* dictDialog = dynamic_cast<MailResourceInfo*>(obj);
            if(dictDialog->uid==dialogUid){
                info->collect->removeObject(dictDialog);
                info->totalNum-=1;
                break;
            }
        }
        if(info->collect->count()<=0){
            GlobalData::shared()->mailCountObj.sysT -=1;
        }
        removeMailFromServer(dialogUid, 1);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETE);
    }
}
void MailController::removeMailDialog(std::string mailUid,std::string dialogUid,std::string type){
    map<string, MailInfo*>::iterator it;
    it = GlobalData::shared()->mailList.find(mailUid);
    if (GlobalData::shared()->mailList.end() != it) {
        CCObject* obj;
        CCARRAY_FOREACH(it->second->dialogs, obj){
            MailDialogInfo* dictDialog = dynamic_cast<MailDialogInfo*>(obj);
            if(dictDialog->uid==dialogUid){
                it->second->dialogs->removeObject(dictDialog);
                it->second->dialogNum-=1;
                break;
            }
        }
        if(it->second->dialogs->count()<=0){
            //GlobalData::shared()->mailList.erase(it);
            GlobalData::shared()->mailCountObj.perT -=1;
        }
        removeMailFromServer(dialogUid, 1);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETE);
    }
}
void MailController::removeResourceMail(std::string mailUid,int index){
    map<string, MailInfo*>::iterator it;
    it = GlobalData::shared()->mailList.find(mailUid);
    if (GlobalData::shared()->mailList.end() == it) {
        return;
    }
    if(it->second->type==MAIL_RESOURCE_HELP){
        MailResourceHelpCellInfo* tempInfo = dynamic_cast<MailResourceHelpCellInfo*>(it->second);
        MailDialogDeleteCommand* command = new MailDialogDeleteCommand("",MAIL_RESOURCE_HELP);
        command->sendAndRelease();
        CC_SAFE_RELEASE(tempInfo);
        GlobalData::shared()->mailCountObj.sysT -=1;
        GlobalData::shared()->mailCountObj.sysR -=tempInfo->unread;
    }else if(it->second->type==MAIL_RESOURCE){
        MailResourceCellInfo* tempInfo = dynamic_cast<MailResourceCellInfo*>(it->second);
        MailDialogDeleteCommand* command = new MailDialogDeleteCommand("",MAIL_RESOURCE);
        command->sendAndRelease();
        CC_SAFE_RELEASE(tempInfo);
        GlobalData::shared()->mailCountObj.sysT -=1;
        GlobalData::shared()->mailCountObj.sysR -=tempInfo->unread;
    }else if(it->second->type==MAIL_ATTACKMONSTER){
        MailMonsterCellInfo* tempInfo = dynamic_cast<MailMonsterCellInfo*>(it->second);
        MailDialogDeleteCommand* command = new MailDialogDeleteCommand("",MAIL_ATTACKMONSTER);
        command->sendAndRelease();
        CC_SAFE_RELEASE(tempInfo);
        GlobalData::shared()->mailCountObj.sysT -=1;
        GlobalData::shared()->mailCountObj.sysR -=tempInfo->unread;
    }

    GlobalData::shared()->mailList.erase(it);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETE, CCInteger::create(index));
}

void MailController::removeChatRoomDialog(string groupId,int index)
{
    auto it = GlobalData::shared()->mailList.find(groupId);
    if (GlobalData::shared()->mailList.end() == it) {
        return;
    }
    GlobalData::shared()->mailCountObj.perT -=1;
    GlobalData::shared()->mailCountObj.perR -=it->second->unreadDialogNum;
    GlobalData::shared()->mailList.erase(it);
    CC_SAFE_RELEASE_NULL(it->second);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETE, CCInteger::create(index));
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::deleteChatRoom(groupId);
#endif
}

void MailController::removeSlectChatRoom(string groupId)
{
    auto it = GlobalData::shared()->mailList.find(groupId);
    if (GlobalData::shared()->mailList.end() == it) {
        return;
    }
    GlobalData::shared()->mailCountObj.perT -=1;
    GlobalData::shared()->mailCountObj.perR -=it->second->unreadDialogNum;
    if (dynamic_cast<MailInfo*>(it->second) != nullptr)
    {
        dynamic_cast<MailInfo*>(it->second)->release();
        it->second = nullptr;
    }
    GlobalData::shared()->mailList.erase(it);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::deleteChatRoom(groupId);
#endif
}

void MailController::removeDialogMail(std::string fromUid, std::string mailUid,int index){
    map<string, MailInfo*>::iterator it;
    it = GlobalData::shared()->mailList.find(mailUid);
    if (GlobalData::shared()->mailList.end() == it) {
        return;
    }
    
    MailInfo* info = it->second;
    MailDialogDeleteCommand* command = new MailDialogDeleteCommand(info->fromUid, -1, info->uid);
    command->sendAndRelease();
    CC_SAFE_RELEASE(info);
    if(info->tabType == MAILTAB5){
        GlobalData::shared()->mailCountObj.modT -=1;
        GlobalData::shared()->mailCountObj.modR -=it->second->unreadDialogNum;
    }else{
        GlobalData::shared()->mailCountObj.perT -=1;
        GlobalData::shared()->mailCountObj.perR -=it->second->unreadDialogNum;
        
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if((info->tabType == USERMAIL) && ChatServiceCocos2dx::enableNativeMail)
        {
            CCLOGFUNC("delete mail");
            ChatServiceCocos2dx::deleteUserMail(fromUid);
        }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        
        if((info->tabType == USERMAIL) && ChatServiceCocos2dx::enableNativeChat)
        {
            CCLOGFUNC("delete mail");
            ChatServiceCocos2dx::deleteUserMail(fromUid);
        }
        
#endif
        
    }
    GlobalData::shared()->mailList.erase(it);
 
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETE, CCInteger::create(index));
}

void MailController::openMailDialogViewFirst(string fromName,std::string fromUid, string modLanguage, int type){
    CCLOG("openMailDialogViewFirst");
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::isChatDialogNeedBack && ChatServiceCocos2dx::enableNativeMail)
    {
        CCLOG("ChatServiceCocos2dx::isChatDialogNeedBack");
        ChatServiceCocos2dx::isChatDialogNeedBack=false;
    }

#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(ChatServiceCocos2dx::isChatDialogNeedBack && ChatServiceCocos2dx::enableNativeChat)
    {
        CCLOG("ChatServiceCocos2dx::isChatDialogNeedBack");
        ChatServiceCocos2dx::isChatDialogNeedBack=false;
    }
#endif
    
    map<string, MailInfo*>::iterator it;
    bool isChat = false;
    MailInfo* info = NULL;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        if((((it->second->type==1 || it->second->type==0) && type == MAIL_SELF_SEND)
            || ((it->second->type == MAIL_MOD_SEND || it->second->type == MAIL_MOD_PERSONAL) && (type == MAIL_MOD_PERSONAL || type == MAIL_MOD_SEND)))
           &&it->second->fromUid==fromUid){
            CCLOGFUNCF("it->second->type %d",it->second->type);
            isChat = true;
            info = it->second;
            break;
        }
    }
    if(!isChat){
        CCDictionary* params = CCDictionary::create();
        params->setObject(CCString::create(CC_ITOA(type)), "type");
        double t = GlobalData::shared()->getWorldTime();
        t *= 1000;
        t = GlobalData::shared()->renewTime(t);
        params->setObject(CCString::create(CC_ITOA(t)), "createTime");
        params->setObject(CCString::create(""), "rewardId");
        params->setObject(CCString::create(fromName), "fromName");
        params->setObject(CCString::create(fromUid), "fromUid");
        params->setObject(CCString::create(modLanguage), "modLanguage");
        params->setObject(CCString::create(CC_ITOA(0)), "save");
        params->setObject(CCString::create(CC_ITOA(0)), "rewardStatus");
        params->setObject(CCString::create(""), "contents");
        params->setObject(CCString::create(CC_ITOA(0)), "itemIdFlag");
        params->setObject(CCString::create(fromUid), "uid");
        params->setObject(CCString::create(CC_ITOA(1)), "status");

        
        info = MailInfo::create();
        info->retain();
        info->parse(params);
        GlobalData::shared()->mailList[params->valueForKey("uid")->getCString()] = info;
    }
    
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::enableNativeMail && GlobalData::shared()->android_native_chat == 1){
        handleMailMessageForOpen(info);
    }else{
        PopupViewController::getInstance()->addPopupInView(MailDialogView::create(info));
    }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    
    if(ChatServiceCocos2dx::enableNativeChat){
        handleMailMessageForOpen(info);
    }else{
        PopupViewController::getInstance()->addPopupInView(MailDialogView::create(info));
    }

#endif
}



void MailController::removeMail(std::string uids, std::string type,int index){
    std::string mt = "";
     if(uids != ""){
        vector<std::string> vector;
        CCCommonUtils::splitString(uids, ",", vector);
         bool bcount = true;
         int count = 0;
        int i = 0;
        while(i < vector.size()){
            std::string uid = vector[i];
            map<string, MailInfo*>::iterator it;
            it = GlobalData::shared()->mailList.find(uid);
            if (GlobalData::shared()->mailList.end() == it) {
                return;
            }
            MailInfo* mail = it->second;
            if(mail->tabType==MAILTAB4){
                if(bcount){
                    bcount = false;
                    count = getCountByType(MAILTAB4);
                }
                if(mail->status==0){
                    GlobalData::shared()->mailCountObj.fightR -=1;
                }
                GlobalData::shared()->mailCountObj.fightT -=1;
            }else if(mail->tabType==MAILTAB5){
                if(bcount){
                    bcount = false;
                    count = getCountByType(MAILTAB5);
                }
                if(mail->status==0){
                    GlobalData::shared()->mailCountObj.modR -=1;
                }
                GlobalData::shared()->mailCountObj.modT -=1;
            }
            else if(mail->tabType==MAILTAB3){
                if(bcount){
                    bcount = false;
                    count = getCountByType(MAILTAB3);
                }
                if(mail->status==0){
                    GlobalData::shared()->mailCountObj.studioR -=1;
                }
                GlobalData::shared()->mailCountObj.studioT -=1;
            }else if(mail->type== MAIL_FRESHER||mail->type==MAIL_SYSNOTICE||mail->type==MAIL_SYSUPDATE){
                if(bcount){
                    bcount = false;
                    count = getCountByType(2);
                }
                if(mail->status==0){
                    GlobalData::shared()->mailCountObj.saveR -=1;
                }
                GlobalData::shared()->mailCountObj.saveT -=1;
            }else if(mail->tabType==USERMAIL){
                if(bcount){
                    bcount = false;
                    count = getCountByType(0);
                }
                if(mail->status==0){
                    GlobalData::shared()->mailCountObj.perR -=1;
                }
                GlobalData::shared()->mailCountObj.perT -=1;
            }else{
                if(bcount){
                    bcount =false;
                    count = getCountByType(1);
                }
                if(mail->status==0){
                    GlobalData::shared()->mailCountObj.sysR -=1;
                }
                GlobalData::shared()->mailCountObj.sysT -=1;
            
            }
            if(mail->type==MAIL_SYSUPDATE){
                GlobalData::shared()->mailCountObj.upR-=1;
            }
           //
            GlobalData::shared()->mailList.erase(it);
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            if(mail->type==CHAT_ROOM)
            {
                ChatServiceCocos2dx::deleteChatRoom(mail->crGroupId);
            }
            else if(mail->tabType>0)
            {
                CCLOGFUNCF("deleteMail uid:%s",mail->uid.c_str());
                ChatServiceCocos2dx::deleteMail(mail->uid ,CHANNEL_TYPE_OFFICIAL,mail->type);
            }
#endif
            CC_SAFE_RELEASE(mail);
            i++;
        }
         map<string, MailInfo*>::iterator it1;
         it1 = GlobalData::shared()->mailList.find(uids);
        removeMailFromServer(uids, count);
//         if(index>-1){
//             CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETE, CCInteger::create(index));
//         }
         CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETE, CCInteger::create(index));
    }
}

void MailController::removeAllMailFromServer(){
    MailDeleteAllCommand* cmd = new MailDeleteAllCommand();
    cmd->sendAndRelease();
}

void MailController::removeMailFromServer(std::string uids, int count){
    MailDeleteCommand* command = new MailDeleteCommand(uids, count);
    command->sendAndRelease();
}
void MailController::reloadMailMore(int type,int num, int offset){
    MailReadBatchCommand* command = new MailReadBatchCommand(type,num, offset);
    command->sendAndRelease();
}

//发送邮件
bool MailController::sendMail(std::string toName, std::string title, std::string contents, std::string allianceId,std::string mailUid,bool isflag, int type,std::string thxMail,bool showTip){
    CCLOG("toName:%s,title:%s,contents:%s,allianceId:%s,mailUid:%s,isFlag:%d,type:%d",toName.c_str(),title.c_str(),contents.c_str(),allianceId.c_str(),mailUid.c_str(),isflag,type);
    if(toName == ""){
        CCCommonUtils::flyText(_lang("105509"));
        return false;
    }
    if(contents == ""){
        CCCommonUtils::flyText(_lang("105540"));
        return false;
    }
//    if(GlobalData::shared()->playerInfo.isInSelfServer()){
        MailSendCommand* cmd = new MailSendCommand(toName, title, contents, allianceId, mailUid, isflag, type, thxMail, showTip);
        cmd->sendAndRelease();
//    }else{
//        MailCrossSendCommand* cmd = new MailCrossSendCommand(toName, title, contents,allianceId,mailUid,isflag);
//        cmd->sendAndRelease();
//    }
    return true;
}

// 原生发送邮件
bool MailController::sendMailToServer(std::string toName, std::string title, std::string contents, std::string sendLocalTime, std::string allianceId,std::string mailUid,bool isflag, int type,std::string targetUid,std::string thxMail,bool showTip){
    CCLOG("toName:%s,title:%s,contents:%s,allianceId:%s,mailUid:%s,isFlag:%d,type:%d,sendLocalTime:%s,targetUid:%s",toName.c_str(),title.c_str(),contents.c_str(),allianceId.c_str(),mailUid.c_str(),isflag,type,sendLocalTime.c_str(),targetUid.c_str());
    if(toName == ""){
        CCCommonUtils::flyText(_lang("105509"));
        return false;
    }
    if(contents == ""){
        CCCommonUtils::flyText(_lang("105540"));
        return false;
    }
    MailSendCommand* cmd = new MailSendCommand(toName, title, contents, allianceId, mailUid, isflag, type, thxMail, showTip, sendLocalTime,targetUid);
    cmd->sendAndRelease();
    return true;
}

void MailController::readMailFromServer(std::string uid, std::string type){
    MailReadCommand* cmd = new MailReadCommand(uid, type);
    cmd->sendAndRelease();
}
void MailController::notyfyReadMail(std::string uid, int type){
    MailReadStatusCommand* cmd = new MailReadStatusCommand(uid, type);
    cmd->sendAndRelease();
}

void MailController::notyfyReadChatMail(std::string fromUser,bool isMod){
    MailDialogReadCommand* cmd = new MailDialogReadCommand(fromUser, -1, "",isMod);
    cmd->sendAndRelease();
}

int MailController::getMailTabTypeByInfo(MailInfo *mail){
    int type=0;
    if(mail->tabType==MAILTAB3){
        type = 3;
    }else if(mail->type== MAIL_FRESHER||mail->type==MAIL_SYSNOTICE||mail->type==MAIL_SYSUPDATE){
        type = 2;
        
    }else if(mail->type <=MAIL_USER){
        type = 0;
    }else if(mail->type == MAIL_MOD_SEND){
        type = 4;
    }
    else{
        type = 1;
    }
    return type;
}
CCArray* MailController::getSortMailByTime(CCArray *data){

    int total = data->count();
    int i = 0;
    int j = total - 1;
    
    while(j > 0){
        i = 0;
        while(i < j){
            auto &mail1 = GlobalData::shared()->mailList[dynamic_cast<CCString*>(data->objectAtIndex(i))->getCString()];
            auto &mail2 = GlobalData::shared()->mailList[dynamic_cast<CCString*>(data->objectAtIndex(i + 1))->getCString()];
            
            bool sweepFlag = false;
            if(mail1->createTime < mail2->createTime){
                sweepFlag = true;
            }
            if(sweepFlag){
                data->swap(i, i + 1);
            }
            i++;
        }
        j--;
    }


    return data;
}
void MailController::refreshMailContent(std::string uid, CCDictionary *dict){
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        auto addGeneral = [](CCArray *arr) {
            CCLOGFUNCF("addGeneral");
            CCArray *ret = CCArray::create();
            CCObject *obj;
            if (arr!=NULL) {
                CCARRAY_FOREACH(arr, obj){
                    GeneralInfo *info = new GeneralInfo(dynamic_cast<CCDictionary*>(obj));
                    if(info)
                    {
                        ret->addObject(info);
                    }
                    CCLOGFUNC("addGeneral 2");
                    CC_SAFE_RELEASE(info);
                }
            }
             CCLOGFUNC("addGeneral 3");
            ret->retain();
            return ret;
        };
        CCLOGFUNCF("refreshMailContent TYPE:%d",it->second->type);
        if(dict->objectForKey("isResourceShieldState")){
            it->second->isResourceShieldState = dict->valueForKey("isResourceShieldState")->boolValue();
        }else{
            it->second->isResourceShieldState = false;
        }
        
        if(dict->objectForKey("atkArmyTotal")){
            it->second->atkArmyTotal = dynamic_cast<CCDictionary*>(dict->objectForKey("atkArmyTotal"));
            it->second->atkArmyTotal->retain();

        }
        if(dict->objectForKey("defArmyTotal")){
            it->second->defArmyTotal = dynamic_cast<CCDictionary*>(dict->objectForKey("defArmyTotal"));
            it->second->defArmyTotal->retain();
        }
        if(dict->objectForKey("atkAlliance")){
            it->second->atkAlliance = dynamic_cast<CCDictionary*>(dict->objectForKey("atkAlliance"));
            it->second->atkAlliance->retain();
        }
        if(dict->objectForKey("defAlliance")){
            it->second->defAlliance = dynamic_cast<CCDictionary*>(dict->objectForKey("defAlliance"));
            it->second->defAlliance->retain();
        }
        if(dict->objectForKey("atkHelpReport")){
            it->second->atkHelpReport = dynamic_cast<CCArray*>(dict->objectForKey("atkHelpReport"));
            it->second->atkHelpReport->retain();
        }
        if(dict->objectForKey("defHelpReport")){
            it->second->defHelpReport = dynamic_cast<CCArray*>(dict->objectForKey("defHelpReport"));
            it->second->defHelpReport->retain();
        }
        if(dict->objectForKey("atkHelper")){
            it->second->atkHelper =dynamic_cast<CCArray*>(dict->objectForKey("atkHelper"));
            it->second->atkHelper->retain();
        }
        if(dict->objectForKey("defHelper")){
            it->second->defHelper =dynamic_cast<CCArray*>(dict->objectForKey("defHelper"));
            it->second->defHelper->retain();
        }
        if(dict->objectForKey("defGen")){
            it->second->defGen = addGeneral(dynamic_cast<CCArray*>(dict->objectForKey("defGen")));
            it->second->atkGen = addGeneral(dynamic_cast<CCArray*>(dict->objectForKey("atkGen")));
            it->second->attReport = dynamic_cast<CCArray*>(dict->objectForKey("atkReport"));
            it->second->attReport->retain();
            it->second->defReport = dynamic_cast<CCArray*>(dict->objectForKey("defReport"));
            it->second->defReport->retain();
            if(dict->objectForKey("defProtectActivate")){
                
                it->second->isShowDefEndTxt = dict->valueForKey("defProtectActivate")->boolValue();//是否显示 “由于防守方等级差距过大，战斗提前结束”cell
            }else{
                it->second->isShowDefEndTxt = false;
            }
            
        }
        if(dict->objectForKey("defFortLost")){
            auto forts = dynamic_cast<CCArray*>(dict->objectForKey("defFortLost"));
            if(forts){
                forts->retain();
            }
            it->second->defFortLost = forts;
        }
        if(dict->objectForKey("atkUser")){
            it->second->attUser = dynamic_cast<CCDictionary*>(dict->objectForKey("atkUser"));
            it->second->attUser->retain();
        }
        if(dict->objectForKey("warPoint")){
            int pos = dict->valueForKey("warPoint")->intValue();
            it->second->warPoint = WorldController::getPointByIndex(pos);
        }
        if(dict->objectForKey("defUser")){
            it->second->defUser = dynamic_cast<CCDictionary*>(dict->objectForKey("defUser"));
            it->second->defUser->retain();
        }
        if(dict->objectForKey("reward")){
            it->second->reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
            if(it->second->reward){
                it->second->reward->retain();   
            }else{
                std::string rewardStr = dict->valueForKey("reward")->getCString();
                if(rewardStr!=""){
                    it->second->reward = RewardController::getInstance()->getBossReward(rewardStr);
                }
            }
        }
        if(dict->objectForKey("atkWarEffect")){
            it->second->atkWarEffect = dynamic_cast<CCArray*>(dict->objectForKey("atkWarEffect"));
            it->second->atkWarEffect->retain();
        }
        if(dict->objectForKey("dfWarEffect")){
            it->second->dfWarEffect = dynamic_cast<CCArray*>(dict->objectForKey("dfWarEffect"));
            it->second->dfWarEffect->retain();
        }
        if(dict->objectForKey("atkPowerLost")){
            it->second->atkPowerLost = dict->valueForKey("atkPowerLost")->intValue();
        }
        if(dict->objectForKey("defPowerLost")){
            it->second->defPowerLost = dict->valueForKey("defPowerLost")->intValue();
        }
        if(it->second->reward==NULL){
            CCArray* rewardArr = CCArray::create();
            rewardArr->retain();
            it->second->reward = rewardArr;
        }
        if(dict->objectForKey("winner")){
            it->second->winner = dict->valueForKey("winner")->getCString();
        }
        if(dict->objectForKey("level")){
            it->second->resLevel = dict->valueForKey("level")->intValue();
        }
        if(dict->objectForKey("pointId")){
            it->second->pointId = dict->valueForKey("pointId")->intValue();
        }
        if(dict->objectForKey("battleType")){
            it->second->battleType = dict->valueForKey("battleType")->intValue();
        }
        if(dict->objectForKey("reportUid")){
            it->second->reportUid = dict->valueForKey("reportUid")->getCString();
        }
        if(dict->objectForKey("defTowerKill")){
            auto towers = dynamic_cast<CCArray*>(dict->objectForKey("defTowerKill"));
            if(towers){
                towers->retain();
            }
            it->second->defTowerKill = towers;
        }
        if(dict->objectForKey("atkGenKill")){
            CCArray* arr =  dynamic_cast<CCArray*>(dict->objectForKey("atkGenKill"));
            arr->retain();
            it->second->atkGenKill = arr;
        }
        if(dict->objectForKey("defGenKill")){
            CCArray* arr =  dynamic_cast<CCArray*>(dict->objectForKey("defGenKill"));
            arr->retain();
            it->second->defGenKill = arr;
        }
        CCString *str = dynamic_cast<CCString*>(dict->objectForKey("pointType"));
        if(str && str->getCString()){
            it->second->pointType =  str->intValue();
        }
        
        if(dict->objectForKey("userKill")){
            it->second->userKill = dict->valueForKey("userKill")->intValue();
        }
        if(dict->objectForKey("allKill")){
             it->second->allKill = dict->valueForKey("allKill")->intValue();
        }
        if(dict->objectForKey("msReport")){
            it->second->msReport = dict->valueForKey("msReport")->intValue();
        }
        if(dict->objectForKey("failTimes")){
           it->second->failTimes = dict->valueForKey("failTimes")->intValue();
        }
        if(dict->objectForKey("winPercent")){
            it->second->winPercent = dict->valueForKey("winPercent")->intValue();
        }
        if(dict->objectForKey("userScore")){
            it->second->userScore = dict->valueForKey("userScore")->intValue();
        }
        if(dict->objectForKey("killRound")){
            it->second->killRound = dict->valueForKey("killRound")->intValue();
        }
        if(dict->objectForKey("monsterLevel")){
            it->second->monsterLevel = dict->valueForKey("monsterLevel")->intValue();
        }
        if(dict->objectForKey("ckfWarInfo")){
            it->second->ckfWarInfo = dynamic_cast<CCDictionary*>(dict->objectForKey("ckfWarInfo"));
            it->second->ckfWarInfo->retain();
        }
        if (dict->objectForKey("ckf")) {
            it->second->ckf = dict->valueForKey("ckf")->intValue();
        }
    }
}

void MailController::refreshMailContent(std::string uid, std::string content){
    CCLOGFUNCF("content: %s",content.c_str());
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        if(it->second->type == MAIL_DETECT){
            if(it->second->itemIdFlag == 1){ // 反侦察
                
            }else{
//                content = _lang_1("105524", content.c_str());//被{0}侦查
                vector<std::string> vector2;
                CCCommonUtils::splitString(content, "|", vector2);
                int num = vector2.size();
                switch (num) {
                    case 1:
                        content = _lang_1("105524", content.c_str());//被{0}侦查
                        break;
                    case 3:{
                        std::string name = vector2[0];//名字
                        std::string type = vector2[1];//类型
                        std::string xy = vector2[2];//坐标
                        if(type=="1"){
                            content = _lang_1("137429", name.c_str());//{0}侦查了您的城堡
                        }else if (type=="2"){
                            content = _lang_1("137431", name.c_str());//{0}侦查了您的部队
                        }else if (type=="3"){
                            content = _lang_1("137430", name.c_str());//{0}侦查了您的资源田
                        }else{
                            content = _lang_1("105524", name.c_str());//被{0}侦查
                        }

                        break;
                    }
                    default:
                        break;
                }

            }
        }else if(it->second->type == MAIL_WOUNDED){
            vector<std::string> vector;
            CCCommonUtils::splitString(content, ",", vector);
            if(vector.size()>=2){
                content = _lang_2("105551", vector[0].c_str(), vector[1].c_str());
            }
        }else if(it->second->type == MAIL_DONATE){
            it->second->fromName = _lang("105305");
            vector<string> mVStr;
            CCCommonUtils::splitString(content, "|", mVStr);
            if(mVStr.size()>4){
                std::string userName = mVStr.at(1);
                if (!mVStr.at(2).empty() && mVStr.at(2) != " ") {
                    userName = "(" + mVStr.at(2) +")" + userName;
                }
                it->second->title = _lang_1("101007",userName.c_str());
                content = _lang_3(mVStr.at(0), userName.c_str(), _lang(mVStr.at(3)).c_str(), (" $"+ mVStr.at(4)).c_str());
            }
            it->second->itemIdFlag = 0;
        }else if(it->second->type == MAIL_WORLD_BOSS && it->second->saveDict && it->second->saveDict->objectForKey("attList")){
            //得到队长的名字
            string leaderName="";
            CCArray *attList=dynamic_cast<CCArray*>(it->second->saveDict->objectForKey("attList"));
            if(attList){
                for (int i=0; i<attList->count(); i++) {
                    CCDictionary *dict=dynamic_cast<CCDictionary*>(attList->objectAtIndex(i));
                    string dName=dict->valueForKey("name")->getCString();
                    if (dict->valueForKey("leader")->intValue()==1) {
                        leaderName=dName;
                    }
                }
                content = _lang_1("137450", leaderName.c_str());
            }
        }

        it->second->contents = content;
        if(it->second->itemIdFlag == 1){
            vector<std::string> vector2;
            CCCommonUtils::splitString(content, "|", vector2);
            int num = vector2.size();
            if (num==1) {
                vector<std::string> vector3;
                CCCommonUtils::splitString(content, "@", vector3);
                num = vector3.size();
                if(num==3){
                    vector<std::string> vector4;
                    CCCommonUtils::splitString(vector3[2], ";", vector4);
                    std::string addStr = "";
                    int addNum = vector4.size();
                    for (int i=0; i<addNum; i++) {
                        vector<std::string> vector5;
                        CCCommonUtils::splitString(vector4[i], ",", vector5);
                        if (vector5.size()==2) {
                            std::string dlgStr=getAddDlgByNum(vector5[0]);
                            if(dlgStr!=""){
                                addStr+="\n";
                                //对64这种情况做单独处理
                                if(vector5[0]=="64"){
                                    std::string str64="";
                                    if (vector5[1].find("-")==vector5[1].npos) {
                                        str64+="-";
                                        str64+=vector5[1];
                                    }else{
                                        str64+="+";
                                        str64+=vector5[1].substr(1);
                                    }
                                    addStr+=_lang_1(dlgStr.c_str(), str64.c_str());
                                }//对64这种情况做单独处理
                                else{
                                    std::string str64="";
                                    if (vector5[1].find("-")==vector5[1].npos) {
                                        str64+="+";
                                        str64+=vector5[1];
                                    }else{
                                        str64+=vector5[1];
                                    }
                                    addStr+=_lang_1(dlgStr.c_str(), str64.c_str());
                                }
                                if(i==addNum-1){
                                    addStr+="\n";
                                }
                            }
                        }
                    }
                    std::string nameStr0 = content;
                    CCLog(nameStr0.c_str());
                    std::string nameStr = CCCommonUtils::getPropById(vector3[1], "name");
                    std::string nameStr2 = _lang(nameStr.c_str());
                    CCLog(nameStr2.c_str());
                    CCLog(_lang_2(vector3[0].c_str(),nameStr2.c_str(),addStr.c_str()));
                    it->second->contents = _lang_2(vector3[0].c_str(),nameStr2.c_str(),addStr.c_str());
                    return;
                }
            }
            CCLOGFUNCF("num %d",num);
            if(num>0 && (vector2[0]=="138086" || vector2[0]=="138068")){//跨服发奖邮件处理
                it->second->ckfContents =content;
                string tempV1 = "";
                string tempV2 = "";
                string tempV3 = "";
                string tempV4 = "";
                if (num>1) {
                    tempV1 = "[color=ffA22200]" + vector2[1]  + "[/color]";
                }
                if (num>2) {
                    tempV2 = "[color=ff865200]" + vector2[2]  + "[/color]";
                }
                if (num>3) {
                    tempV3 = "[color=ff0D6B1A]" + vector2[3]  + "[/color]";
                }
                if (num>4) {
                    tempV4 = "[color=ff008082]" + vector2[4]  + "[/color]";
                }
                it->second->contents = _lang_4(vector2[0].c_str(), tempV1.c_str(), tempV2.c_str(), tempV3.c_str(), tempV4.c_str());
                return ;
            }
            switch (num) {
                case 1:
                    it->second->contents = _lang(content);
                    break;
                case 2:{
                    std::string key = vector2[0];
                    std::string value = vector2[1];
                    if(key.compare("105719") == 0){
                        time_t sendT = atof(value.c_str()) / 1000;
                        value = CCCommonUtils::timeStampToDate(sendT);
                    }
                    if (key.compare("133059") == 0) {
                        int index = atoi(value.c_str());
                        if(index>0 && index<30){
                            value = roman[index-1];
                        }
                    }
                    it->second->contents = _lang_1(key.c_str(),value.c_str());
                    break;
                }
                case 3:
                    if(vector2[1]=="133019"){
                        int index = atoi(vector2[2].c_str());
                        string temp = "";
                        if(index>0 && index<30){
                            temp = roman[index-1];
                        }else{
                            temp = vector2[2];
                        }
                        string temp2 = _lang_1("133019",temp.c_str());
                        it->second->contents = _lang_1(vector2[0].c_str(),temp2.c_str());
                    }else{
                        it->second->contents = _lang_2(vector2[0].c_str(),vector2[1].c_str(),vector2[2].c_str());
                    }
                    break;
                case 4:{
                    it->second->contents = _lang_3(vector2[0].c_str(), vector2[1].c_str(), vector2[2].c_str(), vector2[3].c_str());
                    break;
                }
                case 5:
                    if (vector2[0] == "115336") {//领地使用默认名字
                        int ownerIndex = atoi(vector2[2].c_str());
                        CCPoint ownerPt = WorldController::getInstance()->getPointByIndex(ownerIndex);
                        std::string coord = " (X:";
                        coord.append(CC_ITOA(ownerPt.x));
                        coord.append(" ");
                        coord.append("Y:");
                        coord.append(CC_ITOA(ownerPt.y));
                        coord.append(") ");
                        std::string name = "";
                        name = _lang_1("115312", vector2[4].c_str());
                        it->second->contents = _lang_4(vector2[0].c_str(),name.c_str(),vector2[1].c_str(),coord.c_str(),name.c_str());
                        it->second->title = _lang_1(vector2[3].c_str(), name.c_str());
                    }else{
                        it->second->contents = _lang_4(vector2[0].c_str(), vector2[1].c_str(), vector2[2].c_str(), vector2[3].c_str(), vector2[4].c_str());
                    }
                    break;
                case 6:
                    if (vector2[0] == "115336") {//领地使用自定义名字
                        int ownerIndex = atoi(vector2[3].c_str());
                        CCPoint ownerPt = WorldController::getInstance()->getPointByIndex(ownerIndex);
                        std::string coord = " (X:";
                        coord.append(CC_ITOA(ownerPt.x));
                        coord.append(" ");
                        coord.append("Y:");
                        coord.append(CC_ITOA(ownerPt.y));
                        coord.append(") ");
                        std::string name = "";
                        if (vector2[1] == "") {
                            name = _lang_1("115312", vector2[5].c_str());
                        }
                        else
                            name = vector2[1];
                        it->second->contents = _lang_4(vector2[0].c_str(),name.c_str(),vector2[2].c_str(),coord.c_str(),name.c_str());
                        it->second->title = _lang_1(vector2[4].c_str(), name.c_str());
                    }
                    break;
                case 7:
                    if (vector2[0] == "115494") {
                        it->second->contents = LocalController::shared()->TextINIManager()->getObjectByKey(vector2[0].c_str(), "6",vector2[1].c_str(),vector2[2].c_str(),vector2[3].c_str(),vector2[4].c_str(),vector2[5].c_str(),vector2[6].c_str()).c_str();
                    }
                default:
                    break;
            }
        }
    }
}

std::string MailController::getAddDlgByNum(std::string numStr){
    if (numStr=="1010") {
        return "110054";
    }else if (numStr=="1030"){
        return "110055";
    }else if (numStr=="8"){
        return "110056";
    }else if (numStr=="50"){//50.51.52.53
        return "110058";
    }else if (numStr=="38"){
        return "110063";
    }else if (numStr=="60"){
        return "110057";
    }else if (numStr=="64"){
        return "110062";
    }else if (numStr=="66"){
        return "110060";
    }else if (numStr=="68"){
        return "110059";
    }else if (numStr=="69"){
        return "110061";
    }
    return "";
}

void MailController::refreshMailDialogContent(std::string uid, CCDictionary *dict){
    CCLOG("refreshMailDialogContent");
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("chats"));
        CCObject* obj;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CCArray* mailInfoArr=CCArray::create();
#endif
        
        CCARRAY_FOREACH(arr, obj){
            CCDictionary* dictDialog = dynamic_cast<CCDictionary*>(obj);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(ChatServiceCocos2dx::isChatShowing && ChatServiceCocos2dx::enableNativeMail)
            {
                MailDialogInfo* dialogInfo = MailDialogInfo::create();
                dialogInfo->parse(dictDialog);
                dynamic_cast<MailInfo*>(it->second)->dialogs->insertObject(dialogInfo, 0);
                ChatMailInfo* info=ChatMailInfo::create(dynamic_cast<MailInfo*>(it->second),dialogInfo,false);
                mailInfoArr->addObject(info);
            }
            else
            {
                addOneDialogToMail(it->second,dictDialog);
            }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
            if(ChatServiceCocos2dx::isChatShowing_fun() && ChatServiceCocos2dx::enableNativeChat)
            {
                
            }
            else
            {
                addOneDialogToMail(it->second,dictDialog);
            }
            
#endif
            
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(ChatServiceCocos2dx::enableNativeMail && arr->count()>0)
        {
            notifyMailMsgToAndroid(mailInfoArr,dynamic_cast<MailInfo*>(it->second)->fromUid,dynamic_cast<MailInfo*>(it->second)->fromName);
        }
#endif
        
        int dialogNum = dict->valueForKey("totalNum")->intValue();
        it->second->dialogNum = dialogNum;
        int unreadDialogNum = dict->valueForKey("unread")->intValue();
        it->second->unreadDialogNum = unreadDialogNum;

    }

}

void MailController::notifyMailMsgToAndroid(CCArray* mailInfoArr,string fromUid,string customName,bool isModMsg)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOGFUNCF("fromUid %s",fromUid.c_str());
    m_mailInfoSendDic->setObject(mailInfoArr, ChatServiceCocos2dx::m_curSendMailIndex);
    ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,fromUid,customName,isModMsg);
    ChatServiceCocos2dx::m_curSendMailIndex++;
#endif
}

void MailController::notifyMailMsgToIOS(cocos2d::CCArray *mailInfoArr, int channelType, string fromUid)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
//    CCLOGFUNCF("fromUid %s",fromUid.c_str());
//    m_mailInfoSendDic->setObject(mailInfoArr, ChatServiceCocos2dx::m_curSendMailIndex);
//    ChatServiceCocos2dx::notifyMessageIndex(ChatServiceCocos2dx::m_curSendMailIndex,fromUid);
//    ChatServiceCocos2dx::m_curSendMailIndex++;
    
    ChatServiceCocos2dx::handleChatPush(mailInfoArr, channelType , fromUid);
#endif
}

void MailController::addOneDialogToMail(MailInfo* mailInfo,CCDictionary *dict,bool isNotify){
    MailDialogInfo* dialogInfo = MailDialogInfo::create();
    dialogInfo->parse(dict);
    mailInfo->dialogs->insertObject(dialogInfo, 0);
    //mailInfo->unreadDialogNum+=1;
    CCLOGFUNC("");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(isNotify && !ChatServiceCocos2dx::isChatShowing || !ChatServiceCocos2dx::enableNativeMail){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
#else
    if((isNotify && !ChatServiceCocos2dx::isChatShowing_fun()) || !ChatServiceCocos2dx::enableNativeChat){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
#endif
    
    

}
void MailController::addOneDialogToMail(CCDictionary *dict,std::string mailUid){
    auto it = GlobalData::shared()->mailList.find(mailUid);
    if(it != GlobalData::shared()->mailList.end()){
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CCLOG("fromUid %s",dynamic_cast<MailInfo*>(it->second)->fromUid.c_str());
        if(ChatServiceCocos2dx::enableNativeMail)
        {
            MailDialogInfo* dialogInfo = MailDialogInfo::create();
            dialogInfo->parse(dict);
            dynamic_cast<MailInfo*>(it->second)->dialogs->insertObject(dialogInfo, 0);
            handleMailMessage(dynamic_cast<MailInfo*>(it->second),dialogInfo,false);
            
            if(!ChatServiceCocos2dx::isChatShowing)
            {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
            }
        }
        else
        {
            addOneDialogToMail(it->second,dict);
        }
        
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(ChatServiceCocos2dx::enableNativeChat){
            
            MailDialogInfo* dialogInfo = MailDialogInfo::create();
            dialogInfo->parse(dict);
            dynamic_cast<MailInfo*>(it->second)->dialogs->insertObject(dialogInfo, 0);
            handleMailMessage(dynamic_cast<MailInfo*>(it->second),dialogInfo,false);
            
            if(!ChatServiceCocos2dx::isChatShowing_fun())
            {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
            }
        }else{
            addOneDialogToMail(it->second,dict);
        }
        
#endif
    }
}
void MailController::addOneResourceToMail(CCDictionary *dict,MailResourceCellInfo* mailInfo){
    if(mailInfo==NULL){
        return;
    }
    MailResourceInfo* dialogInfo = MailResourceInfo::create();
    dialogInfo->parse(dict);
    mailInfo->collect->addObject(dialogInfo);
}
void MailController::addOneMonsterToMail(CCDictionary *dict,MailMonsterCellInfo* mailInfo){
    if(mailInfo==NULL){
        return;
    }
    MailMonsterInfo* dialogInfo = MailMonsterInfo::create();
    dialogInfo->parse(dict);
    mailInfo->monster->addObject(dialogInfo);
}
void MailController::addOneResourceToMail(CCDictionary *dict,std::string mailUid){
    auto it = GlobalData::shared()->mailList.find(mailUid);
    if(it != GlobalData::shared()->mailList.end()){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("collect"));
        CCDictionary* tempDic = _dict(arr->objectAtIndex(0));
        MailResourceInfo* dialogInfo = MailResourceInfo::create();
        dialogInfo->parse(tempDic);
        MailResourceCellInfo* tempInfo = dynamic_cast<MailResourceCellInfo*>(it->second);
        tempInfo->collect->insertObject(dialogInfo, 0);
        tempInfo->createTime = dialogInfo->createTime;
        CCDictionary *dict = dynamic_cast<CCDictionary*>(dialogInfo->reward->objectAtIndex(0));
        tempInfo->resourceType = dict->valueForKey("t")->intValue();
        tempInfo->resourceValue = dict->valueForKey("v")->intValue();

        tempInfo->unread+=1;
        tempInfo->totalNum+=1;
        GlobalData::shared()->mailCountObj.sysR+=1;

        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_RESOURCE_LIST_CHANGE);
    }
}
void MailController::addOneResourceHelpToMail(CCDictionary *dict,std::string mailUid){
    auto it = GlobalData::shared()->mailList.find(mailUid);
    if(it != GlobalData::shared()->mailList.end()){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("collect"));
        CCDictionary* tempDic = _dict(arr->objectAtIndex(0));
        MailResourceHelpInfo* dialogInfo = MailResourceHelpInfo::create();
        dialogInfo->parse(tempDic);
        MailResourceHelpCellInfo* tempInfo = dynamic_cast<MailResourceHelpCellInfo*>(it->second);
        tempInfo->collect->insertObject(dialogInfo, 0);
        tempInfo->createTime = dialogInfo->createTime;
        
        tempInfo->unread+=1;
        tempInfo->totalNum+=1;
        GlobalData::shared()->mailCountObj.sysR+=1;
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_RESOURCE_LIST_CHANGE);
    }
}
void MailController::addOneResourceHelpToMail(CCDictionary *dict,MailResourceHelpCellInfo* mailInfo){
    if(mailInfo==NULL){
        return;
    }
    MailResourceHelpInfo* dialogInfo = MailResourceHelpInfo::create();
    dialogInfo->parse(dict);
    mailInfo->collect->addObject(dialogInfo);
}
void MailController::addOneMonsterToMail(CCDictionary *dict,std::string mailUid){
    auto it = GlobalData::shared()->mailList.find(mailUid);
    if(it != GlobalData::shared()->mailList.end()){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("monster"));
        CCDictionary* tempDic = _dict(arr->objectAtIndex(0));
        MailMonsterInfo* dialogInfo = MailMonsterInfo::create();
        dialogInfo->parse(tempDic);
        MailMonsterCellInfo* tempInfo = dynamic_cast<MailMonsterCellInfo*>(it->second);
        tempInfo->monster->insertObject(dialogInfo, 0);
        tempInfo->createTime = dialogInfo->createTime;
        tempInfo->exp =dialogInfo->exp;
        
        tempInfo->unread+=1;
        tempInfo->totalNum+=1;
        GlobalData::shared()->mailCountObj.sysR+=1;
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_MONSTER_LIST_CHANGE);
    }
}
void MailController::addOneDialogToMailEnd(CCDictionary *dict,std::string mailUid,int msgType/*=CHANNEL_TYPE_USER*/){
    
    string uid=dict->valueForKey("uid")->getCString();
    string fromUid = dict->valueForKey("fromUid")->getCString();
    CCLOGFUNCF("NEW UID:%s", uid.c_str());
    
    CCLOGFUNC("addOneDialogToMailEnd 2");
    auto it = GlobalData::shared()->mailList.find(mailUid);
    if(it != GlobalData::shared()->mailList.end()){
            CCLOGFUNC("addOneDialogToMailEnd 2");
//        addOneDialogToMail(it->second,dict);
        MailDialogInfo* dialogInfo = MailDialogInfo::create();
        dialogInfo->parse(dict);
        it->second->dialogs->addObject(dialogInfo);
        it->second->createTime = dialogInfo->createTime;
        it->second->contents = dialogInfo->contents;
        it->second->title= dialogInfo->contents;
        it->second->type = dialogInfo->type;
        if(dialogInfo->pic!=""){
            it->second->pic=dialogInfo->pic;
        }
        if(dialogInfo->picVer!=0){
            it->second->picVer=dialogInfo->picVer;
        }
        it->second->dialogNum+=1;
        
        if(dialogInfo->type==MAIL_MOD_SEND||dialogInfo->type==MAIL_MOD_PERSONAL){
            it->second->unreadDialogNum += 1;
            GlobalData::shared()->mailCountObj.modR += 1;
        }else if(dialogInfo->type==MAIL_USER||dialogInfo->type==MAIL_Alliance_ALL || dialogInfo->type==CHAT_ROOM){
            it->second->unreadDialogNum+=1;
            GlobalData::shared()->mailCountObj.perR+=1;
        }else{
            if(dict->objectForKey("unread")){
                it->second->unreadDialogNum = dict->valueForKey("unread")->intValue();
            }
            if (dict->objectForKey("totalNum")) {
                it->second->dialogNum = dict->valueForKey("totalNum")->intValue();
            }
        }
       
        CCLOGFUNC("handleMailMessage START");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(ChatServiceCocos2dx::enableNativeMail)
        {
            handleMailMessage(dynamic_cast<MailInfo*>(it->second),dialogInfo,true,msgType);
        }
        if(!ChatServiceCocos2dx::isChatShowing)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
        }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CCLOG("fromUid %s",dynamic_cast<MailInfo*>(it->second)->fromUid.c_str());
        if(ChatServiceCocos2dx::enableNativeChat)
        {
            handleMailMessage(dynamic_cast<MailInfo*>(it->second),dialogInfo,true);
        }
        if(!ChatServiceCocos2dx::isChatShowing_fun())
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
        }

#endif
    }else{
            CCLOGFUNC("addOneDialogToMailEnd 4");
        MailInfo* info = MailInfo::create();
        info->retain();
        info->parse(dict);
        MailDialogInfo* dialogInfo = MailDialogInfo::create();
        dialogInfo->parse(dict);
        if(dialogInfo->type!=CHAT_ROOM)
            GlobalData::shared()->mailList[dict->valueForKey("uid")->getCString()] = info;
        else
        {
            info->crGroupId=mailUid;
            GlobalData::shared()->mailList[mailUid] = info;
        }
        info->dialogs->addObject(dialogInfo);
        
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CCLOGFUNC("handleMailMessage START2");
        if(ChatServiceCocos2dx::enableNativeMail)
        {
            handleMailMessage(info,dialogInfo,true,msgType);
        }
        
        if(!ChatServiceCocos2dx::isChatShowing)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
        }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        CCLOG("fromUid %s",info->fromUid.c_str());
        if(ChatServiceCocos2dx::enableNativeChat)
        {
            handleMailMessage(info,dialogInfo,true);
        }
        
        if(!ChatServiceCocos2dx::isChatShowing_fun())
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
        }
        
#endif
        
    }
}

//请求邮件历史消息
void MailController::requestMoreMail(string fromUid,string uid,int count)
{
    MailDialogReadBatchCommand* command = new MailDialogReadBatchCommand(fromUid,uid, count,20);
    command->sendAndRelease();
}


void MailController::refreshOcupyContnet(std::string uid, CCDictionary *dict){
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("arms"));
        it->second->occupyGeneral = arr;
        if(arr){
            arr->retain();
        }
        int pointId = dict->valueForKey("pointId")->intValue();
        it->second->occupyPointId = pointId;
        if (dict->objectForKey("ckf")) {
            it->second->ckf = dict->valueForKey("ckf")->intValue();
        }
        CCDictionary *dic = dynamic_cast<CCDictionary*>(dict->objectForKey("user"));
        it->second->user = dic;
        if(it->second->user){
            it->second->user->retain();
        }
        CCString *str = dynamic_cast<CCString*>(dict->objectForKey("pointType"));
        if(str && str->getCString()){
            it->second->pointType =  str->intValue();
        }
        if(dict->objectForKey("isTreasureMap")){
            it->second->isTreasureMap = dict->valueForKey("isTreasureMap")->boolValue();
        }else{
            it->second->isTreasureMap=false;
        }
    }
}

void MailController::refreshRefuseAllApplyContent(std::string uid, CCDictionary* dict) {
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        vector<std::string> vector;
        
        string content = dict->valueForKey("contents")->getCString();
        CCLOGFUNCF("content:%s",content.c_str());
        string alliancename = dict->valueForKey("alliancename")->getCString();
        it->second->alliancename = alliancename;
        
        CCCommonUtils::splitString(content, "|", vector);
        if(vector.size()>=2){
            content = _lang_1(vector[0].c_str(), alliancename.c_str());
        }
        CCLOGFUNCF("content2:%s",content.c_str());
        it->second->title = content;
        int deal = dict->valueForKey("deal")->intValue();
        it->second->deal = deal;
        int curMember = dict->valueForKey("curMember")->intValue();
        it->second->curMember = curMember;
        int maxMember = dict->valueForKey("maxMember")->intValue();
        it->second->maxMember = maxMember;
        int fightpower = dict->valueForKey("fightpower")->intValue();
        it->second->fightpower = fightpower;
        
        string learderName = dict->valueForKey("learderName")->getCString();
        it->second->learderName = learderName;
        
        
        string iconAlliance = dict->valueForKey("iconAlliance")->getCString();
        it->second->iconAlliance = iconAlliance;
        
        string allianceId = dict->valueForKey("allianceId")->getCString();
        it->second->allianceId = allianceId;
        
        it->second->allianceLang = dict->valueForKey("allianceLang")->getCString();
        it->second->fromName = dict->valueForKey("kickerName")->getCString();
        string pic = dict->valueForKey("kickerPic")->getCString();
        if (pic == "" || pic == "0") {
            pic = "g044";
        }
        it->second->pic = pic;
        it->second->picVer = dict->valueForKey("kickerPicVer")->intValue();
        string dialog = dict->valueForKey("dialog")->getCString();
        it->second->contents =_lang_1(dialog, it->second->fromName.c_str());
        CCLOGFUNCF("it->second->contents:%s",it->second->contents.c_str());
    }
}

void MailController::refreshAllianceKickOutContent(std::string uid, CCDictionary *dict) {
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        vector<std::string> vector;
        string content = dict->valueForKey("contents")->getCString();
        CCCommonUtils::splitString(content, "|", vector);
        if(vector.size()>=2){
            content = _lang_1(vector[0].c_str(), it->second->alliance.c_str());
        }
        it->second->title = content;
        int deal = dict->valueForKey("deal")->intValue();
        it->second->deal = deal;
        int curMember = dict->valueForKey("curMember")->intValue();
        it->second->curMember = curMember;
        int maxMember = dict->valueForKey("maxMember")->intValue();
        it->second->maxMember = maxMember;
        int fightpower = dict->valueForKey("fightpower")->intValue();
        it->second->fightpower = fightpower;
        
        string learderName = dict->valueForKey("learderName")->getCString();
        it->second->learderName = learderName;
        string alliancename = dict->valueForKey("alliancename")->getCString();
        it->second->alliancename = alliancename;
        
        string iconAlliance = dict->valueForKey("iconAlliance")->getCString();
        it->second->iconAlliance = iconAlliance;
        
        string allianceId = dict->valueForKey("allianceId")->getCString();
        it->second->allianceId = allianceId;
        
        it->second->allianceLang = dict->valueForKey("allianceLang")->getCString();
        it->second->fromName = dict->valueForKey("kickerName")->getCString();
        string pic = dict->valueForKey("kickerPic")->getCString();
        if (pic == "" || pic == "0") {
            pic = "g044";
        }
        it->second->pic = pic;
        it->second->picVer = dict->valueForKey("kickerPicVer")->intValue();
        string dialog = dict->valueForKey("dialog")->getCString();
        it->second->contents =_lang_1(dialog, it->second->fromName.c_str());
    }

}
void MailController::refreshAllianceInviteContnet(std::string uid, CCDictionary *dict){
    refreshMailContent(uid, dict->valueForKey("contents")->getCString());
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        int deal = dict->valueForKey("deal")->intValue();
        it->second->deal = deal;
        int curMember = dict->valueForKey("curMember")->intValue();
        it->second->curMember = curMember;
        int maxMember = dict->valueForKey("maxMember")->intValue();
        it->second->maxMember = maxMember;
        int fightpower = dict->valueForKey("fightpower")->intValue();
        it->second->fightpower = fightpower;
        
        string learderName = dict->valueForKey("learderName")->getCString();
        it->second->learderName = learderName;
        string alliancename = dict->valueForKey("alliancename")->getCString();
        it->second->alliancename = alliancename;
        
        string iconAlliance = dict->valueForKey("iconAlliance")->getCString();
        it->second->iconAlliance = iconAlliance;
        
        string allianceId = dict->valueForKey("allianceId")->getCString();
        it->second->allianceId = allianceId;
        
        it->second->allianceLang = dict->valueForKey("allianceLang")->getCString();
    }
}

void MailController::refreshInviteTeleportContent(std::string uid, cocos2d::CCDictionary *dict) {
    auto it = GlobalData::shared()->mailList.find(uid);
    if (it != GlobalData::shared()->mailList.end()) {
        if (dict->objectForKey("deal")) {
            it->second->inviteTeleDeal = dict->valueForKey("deal")->intValue();
        }
        if (dict->objectForKey("inviteeUid")) {
            it->second->inviteeUid = dict->valueForKey("inviteeUid")->getCString();
        }
        if (dict->objectForKey("inviterUid")) {
            it->second->inviterUid = dict->valueForKey("inviterUid")->getCString();
        }
        if (dict->objectForKey("inviteeName")) {
            it->second->inviteeName = dict->valueForKey("inviteeName")->getCString();
        }
        if (dict->objectForKey("inviterName")) {
            it->second->inviterName = dict->valueForKey("inviterName")->getCString();
        }
        if (dict->objectForKey("message")) {
            it->second->message = dict->valueForKey("message")->getCString();
        }
        if (dict->objectForKey("targetPoint")) {
            it->second->targetPoint = dict->valueForKey("targetPoint")->intValue();
        }
        if (dict->objectForKey("pic")) {
            it->second->pic = dict->valueForKey("pic")->getCString();
            if (it->second->pic == "" || it->second->pic == "0") {
                it->second->pic = "g044";
            }
        }
        if (dict->objectForKey("picVer")) {
            it->second->picVer = dict->valueForKey("picVer")->intValue();
        }
    }
}

void MailController::refreshAllianceApplyContnet(std::string uid, CCDictionary *dict){
    refreshMailContent(uid, dict->valueForKey("contents")->getCString());
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        int deal = dict->valueForKey("deal")->intValue();
        it->second->deal = deal;
        it->second->applicantName = dict->valueForKey("applicantName")->getCString();
        it->second->applicantPic = dict->valueForKey("applicantPic")->getCString();
        it->second->applicantPower = dict->valueForKey("applicantPower")->intValue();
        it->second->applicantId = dict->valueForKey("applicantId")->getCString();
    }
}


void MailController::refreshExploreContnet(std::string uid, CCDictionary *dict){
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        int point = dict->valueForKey("point")->intValue();
        it->second->mazeIndex = point;
        int time = dict->valueForKey("exploredTime")->intValue();
        it->second->mazeTime = time;
        it->second->mazeReward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        if(it->second->mazeReward){
            it->second->mazeReward->retain();
        }
        it->second->mazeSodiler = dynamic_cast<CCDictionary*>(dict->objectForKey("trap"));
        if(it->second->mazeSodiler){
            it->second->mazeSodiler->retain();
        }
    }
}

void MailController::refreshAttackMonsterContnet(std::string uid, CCDictionary *dict){
//    auto it = GlobalData::shared()->mailList.find(uid);
//    if(it != GlobalData::shared()->mailList.end()){
//        auto monster = _dict(dict->objectForKey("def"));
//        auto self = _dict(dict->objectForKey("att"));
//        it->second->monsterTotalHp = monster->valueForKey("mmhp")->intValue();
//        it->second->monsterCurrentHp = monster->valueForKey("mchp")->intValue();
//        it->second->monsterId = monster->valueForKey("id")->getCString();
//        it->second->exp = self->valueForKey("exp")->intValue();
//        it->second->hurt = self->valueForKey("hurt")->intValue();
//        it->second->survived = self->valueForKey("survived")->intValue();
//        it->second->dead = self->valueForKey("dead")->intValue();
//        it->second->total = self->valueForKey("total")->intValue();
//        it->second->monsterDead = monster->valueForKey("dead")->intValue();
//        int index = dict->valueForKey("xy")->intValue();
//        it->second->firstKill = false;
//        if(dict->objectForKey("firstKill")){
//            it->second->firstKill = true;
//        }
//        auto pt = WorldController::getPointByIndex(index);
//        it->second->selfX = pt.x;
//        it->second->selfY = pt.y;
//
//        it->second->monsterX = pt.x;
//        it->second->monsterY = pt.y;
//        it->second->selfPowerLost = self->valueForKey("powerLost")->intValue();
//        it->second->monsterPowerLost = monster->valueForKey("powerLost")->intValue();
//
//        if(dict->objectForKey("reportUid")){
//            it->second->reportUid = dict->valueForKey("reportUid")->getCString();
//        }
//
//        if(dict->objectForKey("rateReward")){
//            it->second->normalReward = dynamic_cast<CCArray*>(dict->objectForKey("rateReward"));
//        }
//        if(!it->second->normalReward){
//            it->second->normalReward = CCArray::create();
//        }
//        it->second->normalReward->retain();
//        if(it->second->exp > 0){
//            CCDictionary *dict = CCDictionary::create();
//            dict->setObject(CCString::create(CC_ITOA(R_EXP)), "type");
//            dict->setObject(CCString::create(CC_ITOA(it->second->exp)), "value");
//            it->second->normalReward->addObject(dict);
//        }
//        if(it->second->rewardId != "" && it->second->rewardStatus != 0){
//            vector<std::string> vector1;
//            vector<std::string> vector;
//            if(it->second->killReward == NULL){
//                it->second->killReward = CCArray::create();
//            }
//            it->second->killReward->removeAllObjects();
//            CCCommonUtils::splitString(it->second->rewardId, "|", vector);
//            int i = 0;
//            while(i < vector.size()){
//                std::string rewardStr = vector[i];
//                vector1.clear();
//                CCCommonUtils::splitString(rewardStr, ",", vector1);
//                int type = RewardController::getInstance()->getTypeByName(vector1[0]);
//                int value = atoi(vector1[1].c_str());
//                int num = atoi(vector1[2].c_str());
//                CCDictionary *dict = CCDictionary::create();
//                dict->setObject(CCString::create(CC_ITOA(type)), "type");
//                CCDictionary *valueDict = CCDictionary::create();
//                valueDict->setObject(CCString::create(CC_ITOA(value)), "itemId");
//                valueDict->setObject(CCString::create(CC_ITOA(num)), "count");
//                dict->setObject(valueDict, "value");
//                i++;
//                it->second->killReward->addObject(dict);
//            }
//            it->second->killReward->retain();
//        }else{
//            if(dict->objectForKey("reward")){
//                it->second->killReward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
//                if(it->second->killReward){
//                    it->second->killReward->retain();
//                }
//            }
//        }
//    }
}

void MailController::refreshDiGongContnet(std::string uid, CCDictionary *param){
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        CCArray *arr = dynamic_cast<CCArray*>(param->objectForKey("explore"));
        if (arr && arr->count()>0) {
            CCArray* array = CCArray::create();
            array->retain();
            for (int i=0; i<arr->count(); i++) {
                auto dict = _dict(arr->objectAtIndex(i));
                MonsterSearchResultInfo *info = MonsterSearchResultInfo::create();
                info->type = MonsterSearchResultType(dict->valueForKey("t")->intValue());
                info->time = dict->valueForKey("time")->doubleValue();
                if(dict->objectForKey("fr")){
                    info->fType = MonsterFightResultType(dict->valueForKey("fr")->intValue());
                }
                if(dict->objectForKey("exp")){
                    info->exp = dict->valueForKey("exp")->intValue();
                }
                if(dict->objectForKey("itemId")){
                    info->itemId = dict->valueForKey("itemId")->getCString();
                }
                if(dict->objectForKey("dead")){
                    info->dead = dynamic_cast<CCDictionary*>(dict->objectForKey("dead"));
                }
                if(dict->objectForKey("npcId")){
                    info->npcId = dict->valueForKey("npcId")->getCString();
                }
                if(dict->objectForKey("fid")){
                    info->fid = dict->valueForKey("fid")->getCString();
                }
                if(dict->objectForKey("resType")){
                    info->resType = dict->valueForKey("resType")->getCString();
                }
                if(dict->objectForKey("resValue")){
                    info->resValue = dict->valueForKey("resValue")->getCString();
                }
                array->addObject(info);
            }
            it->second->monsters = array;
        }
    }
}

void MailController::refreshDetectContent(std::string uid, CCDictionary *dict){
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        it->second->detectReport = dict;
        it->second->detectReport->retain();
        CCString *str = dynamic_cast<CCString*>(dict->objectForKey("pointType"));
        if(str && str->getCString()){
            it->second->pointType =  str->intValue();
        }
        if(dict->objectForKey("contents")){
            std::string contents = (dict->valueForKey("contents")->getCString());
            if(contents.compare("114005") == 0){
                it->second->pointType = MAIL_DETECT_REPORT_PROTECT;
                it->second->fromName = _lang("114102");
                it->second->title = _lang("114102");
                it->second->showContent = _lang("114005");
                it->second->contents = _lang("114005");
            }
        }
    }
}

void MailController::refreshGeneralTrainConten(std::string uid, CCDictionary *dict){
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        CCArray *generalArr = CCArray::create();
        generalArr->retain();
        CCArray *expArr = CCArray::create();
        expArr->retain();
        int exp = dict->valueForKey("expAdd")->intValue();
        expArr->addObject(CCInteger::create(exp));
        CCDictionary *generalDict = _dict(dict->objectForKey("genneral"));
        GeneralInfo *info = new GeneralInfo(generalDict);
        generalArr->addObject(info);
        CC_SAFE_RELEASE(info);
        it->second->trainGeneralExp = expArr;
        it->second->trainGenerals = generalArr;
    }
}

void MailController::refreshWorldBossConten(std::string uid, CCDictionary *dict){
    auto it = GlobalData::shared()->mailList.find(uid);
    if(it != GlobalData::shared()->mailList.end()){
        dict->retain();
        it->second->saveDict = dict;
        it->second->fromName = _lang("137451");
    }
}

void MailController::readMailContent(int type){
    
}
void MailController::addToOpMails(std::string mailUid, int mailType){
    if(!mailUid.empty()){
        auto iter = find(mOpMails.begin(), mOpMails.end(), mailUid);
        if(iter ==  mOpMails.end()){
            mOpMails.push_back(mailUid);
        }
    }else if(mailType>SYSTEMMAIL){
        std::vector<int>::iterator iter = find(mOpFolderMails.begin(), mOpFolderMails.end(), mailType);
        if(iter ==  mOpFolderMails.end()){
            mOpFolderMails.push_back(mailType);
        }
    }
}

void MailController::addFromUidsToOpMail(std::string fromUid){
    if(!fromUid.empty()){
        auto iter = find(mOpMailFromUids.begin(), mOpMailFromUids.end(), fromUid);
        if(iter ==  mOpMailFromUids.end()){
            mOpMailFromUids.push_back(fromUid);
        }
    }
}

void MailController::removeFromUidFromOpMails(std::string fromUid){
    if(!fromUid.empty()){
        auto iter = find(mOpMailFromUids.begin(), mOpMailFromUids.end(), fromUid);
        if(iter !=  mOpMailFromUids.end()){
            mOpMailFromUids.erase(iter);
        }
    }
}


void MailController::removeFromOpMails(std::string mailUid, int mailType){
    if(!mailUid.empty()){
        auto iter = find(mOpMails.begin(), mOpMails.end(), mailUid);
        if(iter !=  mOpMails.end()){
            mOpMails.erase(iter);
        }
    }else if(mailType>SYSTEMMAIL){
        std::vector<int>::iterator iter = find(mOpFolderMails.begin(), mOpFolderMails.end(), mailType);
        if(iter !=  mOpFolderMails.end()){
            mOpFolderMails.erase(iter);
        }
    }
}
void MailController::clearOpMails(){
    mOpFolderMails.clear();
    mOpMails.clear();
    mOpMailFromUids.clear();
}
bool MailController::canOp(){
    return mOpMails.size()>0 || mOpFolderMails.size()>0;
}
bool MailController::canOpReward(){
    auto iter = mOpMails.begin();
    while (iter!=mOpMails.end()) {
        auto it = GlobalData::shared()->mailList.find((*iter));
        if (it != GlobalData::shared()->mailList.end()) {
            MailInfo* mail = it->second;
            if(!mail || mail->rewardId.empty() || ( !mail->rewardId.empty() && mail->rewardStatus==1)){
                mOpMails.erase(iter);
                continue;
            }
        }
        ++iter;
    }
    return canOp();
}
bool MailController::isInOpMails(std::string mailUid,int mailType){
    if(mailType>SYSTEMMAIL){
        auto iter1 = find(mOpFolderMails.begin(), mOpFolderMails.end(), mailType);
        if(iter1 != mOpFolderMails.end()){
            return true;
        }
    }
    auto iter = find(mOpMails.begin(), mOpMails.end(), mailUid);
    if(iter!= mOpMails.end()){
        return true;
    }
    return false;
}

void MailController::deleteChatRoomById(string groupId)
{
    if (groupId=="") {
        return;
    }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::enableNativeMail)
    {
        CCLOGFUNC("delete mail");
        auto search = GlobalData::shared()->mailList.find(groupId);
        bool isExistMail=(search != GlobalData::shared()->mailList.end());
        if (isExistMail) {
            MailInfo* mailInfo=dynamic_cast<MailInfo*>(search->second);
            if (mailInfo->type==CHAT_ROOM) {
                removeSlectChatRoom(mailInfo->crGroupId);
            }
        }
    }
#endif
}

void MailController::removeAllOpMails(){
    
    std::string uids = "";
    if(mOpMails.size()>0){
        auto iter = mOpMails.begin();
        uids = (*iter);
        deleteChatRoomById(*iter);
        ++iter;
        while (iter!=mOpMails.end()) {
            deleteChatRoomById(*iter);
            uids += "," + (*iter);
            ++iter;
        }
    }
    std::string types = "";
    if(mOpFolderMails.size()>0){
        std::vector<int>::iterator iter = mOpFolderMails.begin();
        types = CC_ITOA((*iter)-1);
        ++iter;
        while (iter!=mOpFolderMails.end()) {
            types = types + "," + CC_ITOA((*iter)-1);
            ++iter;
        }
    }
    if(canOp()){
        bool test = 0;
        if(!test){
            MailBatchDelCommand *cmd = new MailBatchDelCommand(uids,types);
            cmd->sendAndRelease();
            
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            if(ChatServiceCocos2dx::enableNativeMail)
            {
                CCLOGFUNC("delete mail");
                for (auto iter = mOpMailFromUids.begin(); iter!=mOpMailFromUids.end(); iter++) {
                    auto search = GlobalData::shared()->mailList.find(*iter);
                    bool isExistMail=(search != GlobalData::shared()->mailList.end());
                    if (isExistMail) {
                        MailInfo* mailInfo=dynamic_cast<MailInfo*>(search->second);
                        if (mailInfo->type!=CHAT_ROOM) {
                            ChatServiceCocos2dx::deleteUserMail((*iter));
                        }
                    }
                }
            }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
            if(ChatServiceCocos2dx::enableNativeChat)
            {
                CCLOGFUNC("delete mail");
                for (auto iter = mOpMailFromUids.begin(); iter!=mOpMailFromUids.end(); iter++) {
                    ChatServiceCocos2dx::deleteUserMail((*iter));
                }
            }
#endif
        }
    }
}
void MailController::endRemoveAllOpMails(CCDictionary* params){
    CCLOGFUNC("");
    int state = 0;
    if(mOpMails.size()>0){
        auto iter = mOpMails.begin();
        while (iter!=mOpMails.end()) {
            int tmp = updateMailCount((*iter));
            state = MAX(tmp, state);
            ++iter;
        }
    }
    if(params){
        CCDictElement *ele;
        CCDICT_FOREACH(params, ele){
            std::string keyStr = ele->getStrKey();
            int isR = false;
            int type = 0;
            if(keyStr.length()>1){
                keyStr = keyStr.substr(keyStr.length()-1,1);
                type = atoi(keyStr.c_str());
            }else{
                isR = true;
                type = atoi(keyStr.c_str());
            }
            
            type+=1;
            updateMailCountByType(type);
            int count = 0;
            CCString* numStr = dynamic_cast<CCString*>(ele->getObject());
            if(numStr && numStr->getCString()){
                count = atoi(numStr->getCString());
            }
            if(count>0){
                state = MAX(2, state);
            }
            if(type == MAILTAB3){
                if(isR){
                    GlobalData::shared()->mailCountObj.studioR = count;
                }else{
                    GlobalData::shared()->mailCountObj.studioT = count;
                }
            }else if(type==MAILTAB4){
                if(isR){
                    GlobalData::shared()->mailCountObj.fightR = count;
                }else{
                    GlobalData::shared()->mailCountObj.fightT = count;
                }
            }else if(type == MAILTAB5){
                if(isR){
                    GlobalData::shared()->mailCountObj.modR = count;
                }else{
                    GlobalData::shared()->mailCountObj.modT = count;
                }
            }else if(type == SAVEMAIL){
                if(isR){
                    GlobalData::shared()->mailCountObj.saveR = count;
                }else{
                    GlobalData::shared()->mailCountObj.saveT = count;
                }
            }
        }
    }
    switch (state) {
        case 2: {//有物品未领取
            CCCommonUtils::flyHint("", "", _lang("105507"));
            break;
        }
        case 1:{//有收藏邮件
            break;
        }
        default:
            break;
    }
    auto it = mOpFolderMails.begin();
    while (it!=mOpFolderMails.end()) {
        int count = getCountByType((*it));
        if(count==0){
            int type = 0;
            switch ((*it)) {
                case SAVEMAIL:
                case MAILTAB3:{
                    type=(*it);
                    break;
                }
                case MAILTAB4:{
                    type = 6;
                    break;
                }
                case MAILTAB5:{
                    type =10;
                    break;
                }
                default:
                    break;
            }
            reloadMailMore(type, 0, 20);
        }
        ++it;
    }
    MailController::getInstance()->clearOpMails();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
}
void MailController::rewardAllOpMails(){
    if(canOpReward()){
        std::string uids = "";
        if(mOpMails.size()>0){
            auto iter = mOpMails.begin();
            uids = (*iter);
            ++iter;
            while (iter!=mOpMails.end()) {
                uids += "," + (*iter);
                ++iter;
            }
        }
        std::string types = "";
        if(mOpFolderMails.size()>0){
            std::vector<int>::iterator iter = mOpFolderMails.begin();
            types = CC_ITOA((*iter)-1);
            ++iter;
            while (iter!=mOpFolderMails.end()) {
                types = types + "," + CC_ITOA((*iter)-1);
                ++iter;
            }
        }
        if(!uids.empty() || !types.empty()){
            bool test = 0;
            if(!test){
                MailBatchRewardCommand *cmd = new MailBatchRewardCommand(uids,types);
                cmd->sendAndRelease();
            }else{
                endRewardAllOpMails();
            }
        }
    }
}
void MailController::endRewardAllOpMails(){
    auto iter = mOpMails.begin();
    while (iter!=mOpMails.end()) {
        auto it = GlobalData::shared()->mailList.find((*iter));
        if (it != GlobalData::shared()->mailList.end()) {
            MailInfo* mail = it->second;
            if(mail){
                mail->rewardStatus = 1;
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MIAL_GET_REWARD_REFRESH, CCString::create((*iter)));
            }
        }
        ++iter;
    }
    auto iter1 = mOpFolderMails.begin();
    while (iter1!=mOpFolderMails.end()) {
        int type = (*iter1);
        auto it = GlobalData::shared()->mailList.begin();
        while (it != GlobalData::shared()->mailList.end()) {
            MailInfo* mail = it->second;
            if(mail && mail->tabType == type && !mail->rewardId.empty()){
                mail->rewardStatus = 1;
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MIAL_GET_REWARD_REFRESH, CCString::create((*iter)));
            }
            ++it;
        }
        ++iter1;
    }
    clearOpMails();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
}
int MailController::updateMailCount(std::string mailUid){
    CCLOGFUNC("");
    bool hasSave = false;
    bool hasReward = false;
    auto it = GlobalData::shared()->mailList.find(mailUid);
    if (it != GlobalData::shared()->mailList.end()) {
        MailInfo* mail = it->second;
        if (mail->save==1) {
            hasSave = true;
        }else{
            if (mail->rewardId=="" || (mail->rewardId!=""&&mail->rewardStatus==1)) {
                hasReward = false;
            }else{
                hasReward=true;
            }
        }
        if(hasReward==true)
            return 2;
        if(hasSave == true)
            return 1;
        
        if(mail->tabType==MAILTAB4){
            if(mail->status==0){
                GlobalData::shared()->mailCountObj.fightR -=1;
            }
            GlobalData::shared()->mailCountObj.fightT -=1;
        }else if(mail->tabType==MAILTAB5){
            if(mail->status==0){
                GlobalData::shared()->mailCountObj.modR -=1;
            }
            GlobalData::shared()->mailCountObj.modT -=1;
        }
        else if(mail->tabType==MAILTAB3){
            if(mail->status==0){
                GlobalData::shared()->mailCountObj.studioR -=1;
            }
            GlobalData::shared()->mailCountObj.studioT -=1;
        }else if(mail->type== MAIL_FRESHER||mail->type==MAIL_SYSNOTICE||mail->type==MAIL_SYSUPDATE){
            if(mail->status==0){
                GlobalData::shared()->mailCountObj.saveR -=1;
            }
            GlobalData::shared()->mailCountObj.saveT -=1;
        }else if(mail->tabType==USERMAIL){
            if(mail->status==0){
                GlobalData::shared()->mailCountObj.perR -=1;
            }
            GlobalData::shared()->mailCountObj.perT -=1;
        }else{
            if(mail->status==0){
                GlobalData::shared()->mailCountObj.sysR -=1;
            }
            GlobalData::shared()->mailCountObj.sysT -=1;
            
        }
        if(mail->type==MAIL_SYSUPDATE){
            GlobalData::shared()->mailCountObj.upR-=1;
        }
        GlobalData::shared()->mailList.erase(it);
        CC_SAFE_RELEASE(mail);
    }
    return 0;
}
int MailController::updateMailCountByType(int type){
    if(type<SAVEMAIL)
        return 0;
    bool hasSave = false;
    bool hasReward = false;
    
    vector<std::string> delMails;
    auto it = GlobalData::shared()->mailList.begin();
    while (it!=GlobalData::shared()->mailList.end()) {
        MailInfo* mail = it->second;
        bool tmp1= false;
        bool tmp2= false;
        if(mail->tabType == type){
            if (mail->save==1) {
                tmp1 = true;
            }else{
                if (mail->rewardId=="" || (mail->rewardId!=""&&mail->rewardStatus==1)) {
                    tmp2 = false;
                }else{
                    tmp2=true;
                }
            }
            if(tmp2==true){
                hasReward = true;
                ++it;
                continue;
            }
            if(tmp1 == true){
                hasSave =true;
                ++it;
                continue;
            }
            delMails.push_back(it->first);
        }
        ++it;
    }
    auto iter =  delMails.begin();
    while (iter!=delMails.end()) {
        it = GlobalData::shared()->mailList.find((*iter));
        if (GlobalData::shared()->mailList.end() == it) {
            ++iter;
            continue;
        }
        MailInfo* mail = it->second;
        GlobalData::shared()->mailList.erase(it);
        CC_SAFE_RELEASE(mail);
        ++iter;
    }
    if(hasReward==true)
        return 2;
    if(hasSave == true)
        return 1;
    return 0;
}

void MailController::initTranslateMails(){
    if(GlobalData::shared()->translation==false)
        return;
    if(m_isTransInit){
        return;
    }
    std::string uids = "";
    std::string refUIDS = "";
    auto it = GlobalData::shared()->mailList.begin();
    while (it!=GlobalData::shared()->mailList.end()) {
        MailInfo* mail = it->second;
        if(mail && (mail->tabType == USERMAIL || mail->tabType == MAILTAB5)){
            if(mail->dialogs){
                CCObject *ccObj = NULL;
                bool bInsert = false;
                CCARRAY_FOREACH(mail->dialogs, ccObj){
                    MailDialogInfo *info = dynamic_cast<MailDialogInfo*>(ccObj);
                    if(info && (info->type == MAIL_USER || info->type==MAIL_Alliance_ALL || info->type == MAIL_MOD_SEND)){
                        bInsert=true;
                        std::string appStr = info->uid;
                        uids = uids.empty()? appStr : (uids+","+appStr);
                    }
                }
                if(bInsert==true){
                    std::string appStr = mail->uid;
                    refUIDS = refUIDS.empty()?appStr:(refUIDS+","+appStr);
                }
            }
        }
        ++it;
    }
    if(!uids.empty()){
        MailDialogTranslateCommand *cmd = new MailDialogTranslateCommand(uids,refUIDS);
        cmd->sendAndRelease();
    }
    m_isTransInit=true;
}
void MailController::backInitTranslateMails(cocos2d::CCArray *arr, std::string refuids){
    if(arr!=NULL){
        std::map<std::string, CCDictionary*> mTmp;
        int num = arr->count();
        for (int i=0; i<num; i++) {
            auto dic = _dict(arr->objectAtIndex(i));
            if(dic->objectForKey("translationMsg") && dic->objectForKey("originalLang")){
                std::string aa =dic->valueForKey("translationMsg")->getCString() + std::string("_____,") + dic->valueForKey("originalLang")->getCString();
                CCLog(aa.c_str());
                mTmp[dic->valueForKey("uid")->getCString()] = dic;
            }
        }
        vector<std::string> mTmpV;
        CCCommonUtils::splitString(refuids, ",", mTmpV);
        auto it = mTmpV.begin();
        while (it!=mTmpV.end()) {
            auto iter = GlobalData::shared()->mailList.find((*it));
            if(iter!=GlobalData::shared()->mailList.end()){
                MailInfo *mail = (*iter).second;
                CCObject *ccObj = NULL;
                CCARRAY_FOREACH(mail->dialogs, ccObj){
                    MailDialogInfo *info = dynamic_cast<MailDialogInfo*>(ccObj);
                    if(info){
                        auto it1 = mTmp.find(info->uid);
                        if(it1!= mTmp.end()){
                            info->translationMsg = (*it1).second->valueForKey("translationMsg")->getCString();
                            info->originalLang = (*it1).second->valueForKey("originalLang")->getCString();
                            mTmp.erase(it1);
                        }
                    }
                }

            }
            ++it;
        }
    }
}
void MailController::showMailWarning(){
    if(m_isShowWarning==true)
        return;
    
    if(GlobalData::shared()->mailCountObj.total<300){
        return;
    }else{
        std::string showDialog = "";
        if(GlobalData::shared()->mailCountObj.total<500){
            showDialog = _lang("105724");
        }else{
            showDialog = _lang("105725");
        }
        m_isShowWarning=true;
        YesNoDialog::showYesDialog(showDialog.c_str());
    }
}

MailDialogInfo*  MailController::parseChatRoomDic(CCDictionary* dict,bool isNewMsg,bool isGetAllChatRoom,bool isPush)
{
    if(!MailController::getInstance()->getChatRoomEnable())
        return NULL;
    CCArray* memberUidArray=CCArray::create();
    
    string groupId = dict->valueForKey("roomId")->getCString();
    auto search = GlobalData::shared()->mailList.find(groupId);
    bool isExistMail=(search != GlobalData::shared()->mailList.end() && groupId != "");
    string operatorUid="";
    if(dynamic_cast<CCString*>(dict->objectForKey("senderUid"))!=NULL)
        operatorUid=dict->valueForKey("senderUid")->getCString();
    string sendName=dict->valueForKey("senderName")->getCString();
    string playerUid=GlobalData::shared()->playerInfo.uid;
    bool isOperator=operatorUid==playerUid;
    string memberName="";
    bool isInMemberArray=false;
    string msg="";
    
    CCLOGFUNCF("sendName %s  sendUid:%s   uid:%s",sendName.c_str(),operatorUid.c_str(),playerUid.c_str());
    string operatorName=_lang("105347");
    if (!isOperator) {
        operatorName=sendName;
    }
    CCLOGFUNCF("operatorName %s",operatorName.c_str());
    
    CCDictionary* body=dynamic_cast<CCDictionary*>(dict->objectForKey("body"));
    int type=-1;
    if (body) {
        if(isPush)
        {
            type=body->valueForKey("type")->intValue();
        }
        else
        {
            CCInteger* typeInt=dynamic_cast<CCInteger*>(body->objectForKey("type"));
            if (typeInt) {
                type=typeInt->getValue();
            }
            else
            {
                type=body->valueForKey("type")->intValue();
            }
        }
        if (type==CHAT || type==NAME) {
            msg=body->valueForKey("msg")->getCString();
        }
        else
        {
            CCArray* memberArray=dynamic_cast<CCArray*>(body->objectForKey("msg")) ;
            if (memberArray!=NULL && memberArray->count()>0) {
                CCLOGFUNCF("member size:%d",memberArray->count());
                CCObject* pObj=NULL;
                CCARRAY_FOREACH(memberArray, pObj)
                {
                    CCDictionary* member=dynamic_cast<CCDictionary*>(pObj);
                    if (member!=NULL) {
                        
                        bool isMember=false;
                        if (member->valueForKey("uid")) {
                            string uid=member->valueForKey("uid")->getCString();
                            if (uid!="") {
                                if (playerUid==uid) {
                                    if (!isInMemberArray) {
                                        isInMemberArray=true;
                                    }
                                    isMember=true;
                                }
                                CCLOGFUNCF("ADD MEMBER UID:%s",uid.c_str());
                                memberUidArray->addObject(CCString::create(uid));
                            }
                        }
                        
                        if (member->valueForKey("name")) {
                            string name=member->valueForKey("name")->getCString();
                            
                            CCString* uid=dynamic_cast<CCString*>(member->objectForKey("uid"));
                            CCLOGFUNCF("uid: %s",uid->getCString());
                            
                            if (sendName!=name){
                                if(isMember)
                                    name=_lang("105347");
                                CCLOGFUNCF("name %s",name.c_str());
                                if (memberName=="") {
                                    memberName.append(name);
                                }
                                else
                                {
                                    memberName.append("、").append(name);
                                }
                            }
                        }
                        
                    }
                }
            }
        }
    }
    
    string groupName = dict->valueForKey("name")->getCString();
    CCLOGFUNCF("groupName:%s",groupName.c_str());
    string content="";
    switch (type) {
        case MK:
        case INVITE:
            content=_lang_2("105337", operatorName.c_str(), memberName.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(isInMemberArray && isPush &&ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable())
            {
                ChatServiceCocos2dx::postIsChatRoomMemberFlag(groupId,true);
            }
#endif
            break;
        case KICK:
            if (isInMemberArray && !isOperator) {
                content=_lang_1("105340", operatorName.c_str());
            }
            else
            {
                content=_lang_2("105338", operatorName.c_str(), memberName.c_str());
            }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(isInMemberArray && isPush && ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable())
            {
                ChatServiceCocos2dx::postIsChatRoomMemberFlag(groupId,false);
            }
#endif
            break;
        case QUIT:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(isOperator && isPush && ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable())
            {
                ChatServiceCocos2dx::postIsChatRoomMemberFlag(groupId,false);
            }
#endif
            content=_lang_1("105345", operatorName.c_str());
            break;
        case CHAT:
            content=msg;
            break;
        case NAME:
            content=_lang_2("105346", operatorName.c_str(),msg.c_str());
            break;
        default:
            break;
    }
    
    CCDictionary* params=CCDictionary::create();
    params->setObject(CCString::create(CC_ITOA(CHAT_ROOM)), "type");
    double t = GlobalData::shared()->getWorldTime();
    t *= 1000;
    int time=dict->valueForKey("time")->intValue();
    double tempTime=(double)time*1000;
    if (tempTime<=0) {
        tempTime=GlobalData::shared()->renewTime(t);
    }
    
    string sendPic="";
    int sendPicVer=0;
    int vip=0;
    long vipEndTime=0;
    string asn="";
    string allianceId="";
    string sendLocalTime="";
    
    if(dict->objectForKey("senderPic"))
    {
        sendPic=dict->valueForKey("senderPic")->getCString();
    }
    if(dict->objectForKey("senderPicVer"))
    {
        sendPicVer=dict->valueForKey("senderPicVer")->intValue();
    }
    if(dict->objectForKey("vip"))
    {
        vip=dict->valueForKey("vip")->intValue();
    }
    if(dict->objectForKey("vipEndTime"))
    {
        vipEndTime=dict->valueForKey("vipEndTime")->longValue();
    }
    if(dict->objectForKey("asn"))
    {
        asn=dict->valueForKey("asn")->getCString();
    }
    if(dict->objectForKey("allianceId"))
    {
        allianceId=dict->valueForKey("allianceId")->getCString();
    }
    if(dict->objectForKey("sendLocalTime"))
    {
        sendLocalTime=dict->valueForKey("sendLocalTime")->getCString();
    }
    
    
    params->setObject(CCString::create(sendPic) , "pic");
    params->setObject(CCString::create(CC_ITOA(sendPicVer)) , "picVer");
    params->setObject(CCString::create(CC_ITOA(vip)) , "vip");
    params->setObject(CCString::create(CC_ITOA(vipEndTime)) , "vipEndTime");
    params->setObject(CCString::create(asn) , "asn");
    params->setObject(CCString::create(allianceId) , "allianceId");
    params->setObject(CCString::create(sendLocalTime) , "sendLocalTime");
    
    params->setObject(CCString::create(CC_ITOA(tempTime)) , "createTime");
    params->setObject(CCString::create(groupName), "oldName");
    params->setObject(CCString::create(sendName), "fromName");
    params->setObject(CCString::create(CC_ITOA(0)), "save");
    params->setObject(CCString::create(content), "contents");
    params->setObject(CCString::create(operatorUid), "fromUid");
    params->setObject(dynamic_cast<CCString*>(dict->objectForKey("seqId")) , "seqId");
    params->setObject(dynamic_cast<CCString*>(dict->objectForKey("lastUpdateTime")) , "lastUpdateTime");
    
    if (type==CHAT) {
        params->setObject(CCInteger::create(0), "tipFlag");
    }
    else
    {
        params->setObject(CCInteger::create(1), "tipFlag");
    }
    
    MailInfo* mailInfo=MailInfo::create();
    
    if(!isGetAllChatRoom)
    {
        if(!isExistMail){
            CCLOGFUNCF("1111111");
            mailInfo->parse(params);
            mailInfo->retain();
            mailInfo->crGroupId=groupId;
//            memberUidArray->retain();
//            mailInfo->crMemberUidArray=memberUidArray;
        }
        else
        {
            CCLOGFUNCF("222222");
            mailInfo=dynamic_cast<MailInfo*>(search->second);
            if (mailInfo!=NULL) {
                if(type==INVITE)
                {
                    mailInfo->crGroupId = groupId;
//                    memberUidArray->retain();
//                    mailInfo->crMemberUidArray = memberUidArray;
                }
//                else
//                {
//                    if (mailInfo->crMemberUidArray) {
//                        CCArray* tempMemberUidArray=CCArray::create();
//                        for(int i=0;i<mailInfo->crMemberUidArray->count();i++)
//                        {
//                            string uid=dynamic_cast<CCString*>(mailInfo->crMemberUidArray->objectAtIndex(i))->getCString();
//                            bool isKicked=false;
//                            for(int k=0;k<memberUidArray->count();k++)
//                            {
//                                string uid2=dynamic_cast<CCString*>(memberUidArray->objectAtIndex(k))->getCString();
//                                CCLOGFUNCF("uid:%s   uid2:%s",uid.c_str(),uid2.c_str());
//                                if(strcmp(uid.c_str(), uid2.c_str())==0)
//                                {
//                                    CCLOGFUNC("uid!=uid2");
//                                    isKicked=true;
//                                    break;
//                                }
//                            }
//                            if(!isKicked)
//                                tempMemberUidArray->addObject(dynamic_cast<CCString*>(mailInfo->crMemberUidArray->objectAtIndex(i)));
//                        }
//                        mailInfo->crMemberUidArray->removeAllObjects();
//                        mailInfo->crMemberUidArray->addObjectsFromArray(tempMemberUidArray);
//                        CCLOGFUNCF("mailInfo->crMemberUidArray %d",mailInfo->crMemberUidArray->count());
//                    }
//                }
            }
        }
    }
    else
    {
        if (isExistMail) {
            mailInfo=dynamic_cast<MailInfo*>(search->second);
            if(isNewMsg)
                mailInfo->parse(params);
        }
    }
    
    if(memberUidArray->count()>0 && isNewMsg)
    {
        CCLOGFUNC("parse group member");
        string memberUids="";
        for (int j=0; j<memberUidArray->count(); j++) {
            CCString* uid=dynamic_cast<CCString*>(memberUidArray->objectAtIndex(j));
            if (uid) {
                string uidStr=uid->getCString();
                if (memberUids!="" && uidStr!="") {
                    memberUids.append("_").append(uidStr);
                }
                else
                {
                    memberUids.append(uidStr);
                }
            }
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(ChatServiceCocos2dx::enableNativeMail && MailController::getInstance()->getChatRoomEnable() && (((type==MK||type==INVITE||type==KICK)&&  memberUids!="")||(type==QUIT && operatorUid!="")))
        {
            CCLOGFUNCF("groupId %s, MEMBERUIDS:%s",groupId.c_str(),memberUids.c_str());
            switch (type) {
                case MK:
                    ChatServiceCocos2dx::setChannelMemberArray(CHANNEL_TYPE_CHATROOM,groupId,memberUids,groupName);
                    break;
                case INVITE:
                    if(isInMemberArray)
                        MailController::getInstance()->getChatRoomMember(groupId);
                    else
                        ChatServiceCocos2dx::updateChannelMemberArray(CHANNEL_TYPE_CHATROOM,groupId,memberUids,true);
                    break;
                case KICK:
                    ChatServiceCocos2dx::updateChannelMemberArray(CHANNEL_TYPE_CHATROOM,groupId,memberUids,false);
                    break;
                case QUIT:
                    ChatServiceCocos2dx::updateChannelMemberArray(CHANNEL_TYPE_CHATROOM,groupId,operatorUid,false);
                    break;
                default:
                    break;
            }
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(ChatServiceCocos2dx::enableChatRoom && (((type==MK||type==INVITE||type==KICK)&&  memberUids!="")||(type==QUIT && operatorUid!="")))
        {
            switch (type) {
                    //创建
                case MK:
                    ChatServiceCocos2dx::createChatChannelRoom(groupId,memberUids);
                    break;
                case INVITE:
                    if(isInMemberArray)
                        MailController::getInstance()->getChatRoomMember(groupId);
                    else
                        ChatServiceCocos2dx::updateChannelMemberArray(3,groupId,memberUids,0);
                    break;
                case KICK:
                        ChatServiceCocos2dx::updateChannelMemberArray(3,groupId,memberUids,1);
                    break;
                case QUIT:
                        ChatServiceCocos2dx::updateChannelMemberArray(3,groupId,operatorUid,2);
                    break;
                default:
                    break;
            }
        }
#endif
    }
    
    
    MailDialogInfo* dialogInfo = MailDialogInfo::create();
    dialogInfo->parse(params);
    if (isPush) {
        if(type!=MK)
        {
            if(type==NAME)
                mailInfo->fromName=msg;
            mailInfo->createTime = dialogInfo->createTime;
            mailInfo->contents = dialogInfo->contents;
            mailInfo->title= dialogInfo->contents;
            mailInfo->type = dialogInfo->type;
            if(dialogInfo->pic!=""){
                mailInfo->pic=dialogInfo->pic;
            }
            if(dialogInfo->picVer!=0){
                mailInfo->picVer=dialogInfo->picVer;
            }
            
            GlobalData::shared()->mailCountObj.perR +=1;
            GlobalData::shared()->mailCountObj.perT +=1;
            mailInfo->unreadDialogNum++;
            mailInfo->isReadContent=false;
            
//            CCArray* arr=dynamic_cast<CCArray*>(MailController::getTempChatRoomMsgDic()->objectForKey(groupId));
//            if (arr!=NULL && arr->count()>0) {
//                GlobalData::shared()->mailCountObj.perR +=1;
//                GlobalData::shared()->mailCountObj.perT +=1;
//                mailInfo->unreadDialogNum++;
//                mailInfo->dialogNum+=1;
//            }
        }
        
        if(mailInfo->dialogs!=NULL)
            mailInfo->dialogs->addObject(dialogInfo);
    }
    else
    {
        if(mailInfo->dialogs!=NULL)
            mailInfo->dialogs->insertObject(dialogInfo, 0);
    }
    GlobalData::shared()->mailList[groupId]=mailInfo;

    return dialogInfo;
}

CCDictionary* MailController::parseChatRoomJsonMsg(Json* jsonParent,Json* jsonBody)
{
    CCDictionary* dic=CCDictionary::create();
    CCDictionary* body=CCDictionary::create();
    int type=Json_getInt(jsonBody, "type", -1);
    body->setObject(CCInteger::create(type), "type");
    if (type==4 || type==5) {
        body->setObject(CCString::create(Json_getString(jsonBody, "msg", "")), "msg");
    }
    else
    {
        Json* jsonMsg=Json_getItem(jsonBody, "msg");
        if (jsonMsg->type == Json_Array) {
            CCArray* msgArray=CCArray::create();
            Json* child=jsonMsg->child;
            while(child!=NULL)
            {
                CCDictionary* msgDic=CCDictionary::create();
                msgDic->setObject(CCString::create(Json_getString(child, "name", "")), "name");
                msgDic->setObject(CCString::create(Json_getString(child, "uid", "")), "uid");
                msgDic->setObject(CCString::create(Json_getString(child, "pic", "")), "pic");
                msgDic->setObject(CCInteger::create(Json_getInt(child, "picVer", -1)), "picVer");
                msgArray->addObject(msgDic);
                child=child->next;
            }
            body->setObject(msgArray, "msg");
        }
    }
    
    dic->setObject(body, "body");
    Json* obj=Json_getItem(jsonParent, "time");
    string time="";
    if(obj->type==Json_String)
        time=obj->valueString;
    CCLOGFUNCF("time %s",time.c_str());
    dic->setObject(CCString::create(time), "time");
    dic->setObject(CCString::create(Json_getString(jsonParent, "senderName", "")), "senderName");
    dic->setObject(CCString::create(Json_getString(jsonParent, "senderUid", "")), "senderUid");
    dic->setObject(CCString::create(Json_getString(jsonParent, "seqId", "")), "seqId");
    dic->setObject(CCString::create(Json_getString(jsonParent, "lastUpdateTime", "")), "lastUpdateTime");
    dic->setObject(CCString::create(Json_getString(jsonParent, "name", "")), "name");
    dic->setObject(CCString::create(Json_getString(jsonParent, "roomId", "")), "roomId");
    dic->setObject(CCString::create(Json_getString(jsonParent, "senderPic", "")), "senderPic");
    dic->setObject(CCString::create(Json_getString(jsonParent, "senderPicVer", "")), "senderPicVer");
    dic->setObject(CCString::create(Json_getString(jsonParent, "vip", "")), "vip");
    dic->setObject(CCString::create(Json_getString(jsonParent, "vipEndTime", "")), "vipEndTime");
    dic->setObject(CCString::create(Json_getString(jsonParent, "asn", "")), "asn");
    dic->setObject(CCString::create(Json_getString(jsonParent, "allianceId", "")), "allianceId");
    return dic;
}

void MailController::createChatRoom(std::string memberName,CCArray* memberUidArray, std::string roomName,std::string content)
{
    CreateChatRoomCommand* cmd=new CreateChatRoomCommand(memberName,memberUidArray,roomName,content);
    cmd->sendAndRelease();
}

void MailController::inviteChatRoomMembers(std::string memberName,CCArray* memberUidArray,std::string groupId)
{
    CCLOGFUNC("");
    InviteChatRoomMemberCommand* cmd=new InviteChatRoomMemberCommand(memberName,memberUidArray,groupId);
    cmd->sendAndRelease();
}

void MailController::kickChatRoomMembers(std::string memberName,CCArray* memberUidArray,std::string groupId)
{
    CCLOGFUNCF("groupId %s memberUidArray size:%d",groupId.c_str(),memberUidArray->count());
    KickChatRoomMemberCommand* cmd=new KickChatRoomMemberCommand(memberName,memberUidArray,groupId);
    cmd->sendAndRelease();
}

void MailController::modifyChatRoomName(std::string name,std::string groupId)
{
    ModifyChatRoomNameCommand* cmd=new ModifyChatRoomNameCommand(name,groupId);
    cmd->sendAndRelease();
}
/**聊天室发送消息*/
void MailController::sendChatRoomMsg(std::string msg, std::string groupId, std::string sendLocalTime)
{
    SendChatRoomMsgCommand* cmd=new SendChatRoomMsgCommand(msg, groupId, sendLocalTime);
    cmd->sendAndRelease();
}

void MailController::quitChatRoom(std::string groupId)
{
     QuitChatRoomCommand* cmd=new QuitChatRoomCommand(groupId);
    cmd->sendAndRelease();
}

void MailController::getChatRoomMsgRecord(bool isShowing,std::string groupId)
{
    auto search = GlobalData::shared()->mailList.find(groupId);
    bool isExistMail=(search != GlobalData::shared()->mailList.end() && groupId != "");
    CCArray* dialogInfoArr=NULL;
    if(isExistMail)
        dialogInfoArr=(dynamic_cast<MailInfo*>(search->second))->dialogs;
    if (dialogInfoArr==NULL || dialogInfoArr->count()<=0) {
        return;
    }
    
    CCLOGFUNCF("dialogInfoArr->count():%d",dialogInfoArr->count());
    MailDialogInfo* mailDialog=dynamic_cast<MailDialogInfo*>(dialogInfoArr->objectAtIndex(0));
    if (mailDialog) {
        int endSeq=mailDialog->sequenceId-1;
        int startSeq=(endSeq-(20-dialogInfoArr->count()));
        startSeq=startSeq<1?1:(endSeq-(20-dialogInfoArr->count()));
        int count=endSeq-startSeq+1;
        CCLOGFUNCF("startSeq %d count %d",startSeq,count);
        if (count>0) {
            GetChatRoomMsgCommand* cmd=new GetChatRoomMsgCommand(isShowing,startSeq,count,groupId);
            cmd->sendAndRelease();
        }
    }
}

void MailController::requestChatRoomMsgRecord(bool isShowing,int start,int count,std::string groupId)
{
    if (count>0) {
        GetChatRoomMsgCommand* cmd=new GetChatRoomMsgCommand(isShowing,start,count,groupId);
        cmd->sendAndRelease();
    }
}

void MailController::initChatRoomList()
{
    GetChatRoomListCommand * cmd=new GetChatRoomListCommand();
    cmd->sendAndRelease();
}

void MailController::getAllianceMember()
{
    CCLOGFUNC("");
    if(m_isChatRoomEnable&& GlobalData::shared()->playerInfo.isInAlliance()){
        GetInviteeCommand * command = new GetInviteeCommand();
        command->putParam("allianceId", CCString::create(GlobalData::shared()->playerInfo.getAllianceId()));
        command->sendAndRelease();
    }
}

void MailController::createChatRoom(std::string content)
{
    CCLOGFUNC("");
    if (m_inviteMemberName=="" || m_inviteMemberUid=="") {
         CCLOGFUNC("createChatRoom return");
        return;
    }
    vector<string> strVec1;
    vector<string> strVec2;
    vector<string> strVec3;
    CCArray* memberUidArray=CCArray::create();
    memberUidArray->retain();
    
    CCCommonUtils::splitString(m_inviteMemberUid, "|", strVec1);
    CCLOGFUNCF("strVec1.size() :%d",strVec1.size());
    for (int i = 0; i<strVec1.size(); i++)
    {
        CCLOGFUNCF("strVec1.at(i) :%s",strVec1.at(i).c_str());
        memberUidArray->addObject(CCString::create(strVec1.at(i).c_str()));
    }
    
    MailController::getInstance()->createChatRoom(m_inviteMemberName,memberUidArray, m_chatRoomName, content);
}

void MailController::setChatRoomInfo(string roomName, string memberName, string memberUid)
{
    m_chatRoomName=roomName;
    m_inviteMemberName=memberName;
    m_inviteMemberUid=memberUid;
}

void MailController::quitAllChatRoom()
{
    if(m_chatRoomIdArray!=NULL && m_chatRoomIdArray->count()>0)
    {
        QuitChatRoomArrayCommand* cmd=new QuitChatRoomArrayCommand(m_chatRoomIdArray);
        cmd->sendAndRelease();
    }
}

void MailController::getChatRoomMember(string groupId)
{
    GetChatRoomMemberCommand* cmd=new GetChatRoomMemberCommand(groupId);
    cmd->sendAndRelease();
}

void MailController::addLocalTranslate(MailDialogInfo *cInfo){
    if(!cInfo)
        return;
    if(!cInfo->translationMsg.empty()){
        return;
    }
    if(mTransMailDialog){
        mTransQueue.push_back(cInfo);
        return;
    }
    mTransMailDialog = cInfo;
    localTranslate();
}
void MailController::localTranslate(){
    if(!mTransMailDialog)
        return;
    ChatController::getInstance()->callXCApiForGoogleTranslate();
    
    CCHttpRequest* request = new CCHttpRequest();
    string url =  CCString::createWithFormat("https://translate.google.com/translate_a/single")->getCString();
    CCLOG("change URL: %s",url.c_str());
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::Type::POST);
    std::string orgMsg = mTransMailDialog->contents;
    const char* postData = CCString::createWithFormat("client=t&sl=auto&tl=%s&dt=t&ie=UTF-8&oe=UTF-8&source=btn&q=%s", LocalController::shared()->getLanguageFileName().c_str(),orgMsg.c_str())->getCString();
    request->setRequestData(postData ,strlen(postData));
    request->setResponseCallback(this, httpresponse_selector(MailController::onMailLocalTranslate));
    request->setTag("mail_translate_request");
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
}
void MailController::onMailLocalTranslate(CCHttpClient *client, CCHttpResponse *response){
    if(!mTransMailDialog)
        return;
    
    if (!response){
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag())){
        CCLOG("%s completed", response->getHttpRequest()->getTag());
        if (strcmp("mail_translate_request",response->getHttpRequest()->getTag())!=0) {
            // return;
        }
    }
    int m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    CCLog("translate result: %s, %d", serverInfo.c_str(), serverInfo.length());
    std::string finalStr = "";
    std::string orgLan = "";
    ChatController::getTranslateInfo(serverInfo,orgLan,finalStr);
    
    mTransMailDialog->originalLang = orgLan;
    mTransMailDialog->translationMsg = finalStr;
    mTransMailDialog->isShowOrginal=false;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_PERSON_CHAT_CHANGE);
    
    if(mTransQueue.size()>0){
        mTransMailDialog = mTransQueue.at(0);
        mTransQueue.erase(mTransQueue.begin());
        localTranslate();
    }else{
        mTransMailDialog =NULL;
    }
}

void MailController::searchPlayer(string name)
{
    m_isSearchUser=true;
    WorldSearchUserCommand* cmd = new WorldSearchUserCommand(name,1);
    cmd->sendAndRelease();
}

void MailController::showMailPopupFromAnroid(MailInfo* m_mailInfo)
{
    bool issaveRead = false;
    if(m_mailInfo==NULL){
        return;
    }
    CCLOGFUNCF("type:%d  tabType:%d title:%s",m_mailInfo->type,m_mailInfo->tabType,m_mailInfo->title.c_str());

    m_mailInfo->status = READ;
    UIComponent::getInstance()->loadMailResource();
    PopupBaseView* popUpView = NULL;
    if(m_mailInfo->type == MAIL_BATTLE_REPORT){
        popUpView = BattleReportMailPopUpView::create(m_mailInfo);
    }
    else if(m_mailInfo->type == MAIL_RESOURCE){
        popUpView = MailResourcePopUpView::create(dynamic_cast<MailResourceCellInfo*>(m_mailInfo));
    }else if(m_mailInfo->type == MAIL_RESOURCE_HELP){
        popUpView = MailResourceHelpView::create(dynamic_cast<MailResourceHelpCellInfo*>(m_mailInfo));
    }
    else if(m_mailInfo->type == MAIL_DETECT_REPORT){
        if(m_mailInfo->pointType == MAIL_DETECT_REPORT_PROTECT){
            popUpView = MailReadPopUpView::create(*m_mailInfo);
        }else{
            popUpView = DetectMailPopUpView::create(m_mailInfo);
        }
    }else if(m_mailInfo->type==MAIL_DETECT){
        if(m_mailInfo->itemIdFlag == 1){ //反侦察
            popUpView = MailReadPopUpView::create(*m_mailInfo);
        }else{
            popUpView = DetectMailPopUpView::create(m_mailInfo);
        }
    }
    else if(m_mailInfo->type == MAIL_ENCAMP){
        popUpView = OccupyMailPopUpView::create(m_mailInfo);
    }
    else if(m_mailInfo->type == WORLD_NEW_EXPLORE){
        popUpView = ExplorePopUpView::create(m_mailInfo);
    }else if(m_mailInfo->type==MAIL_SYSUPDATE){
        popUpView = MailAnnouncePopUp::create(m_mailInfo);
    }else if(m_mailInfo->type == MAIL_ATTACKMONSTER){
        popUpView = MailMonsterListView::create(dynamic_cast<MailMonsterCellInfo*>(m_mailInfo));
    }else if(m_mailInfo->type == MAIL_GIFT){
        popUpView = MailGiftReadPopUpView::create(*m_mailInfo);
    }else if(m_mailInfo->type == MAIL_WORLD_BOSS){
        if(m_mailInfo->isWorldBossKillRewardMail){
            CCLOGFUNC("isWorldBossKillRewardMail");
            popUpView =WorldBossRewardMailView::create(*m_mailInfo);
        }
        else
        {
            popUpView =WorldBossMailView::create(*m_mailInfo);
        }
    }else{
        popUpView = MailReadPopUpView::create(*m_mailInfo);
    }
    
    if(popUpView)
    {
        popUpView->setHDPanelFlag(true);
        popUpView->setIsSystemMail(true);
        PopupViewController::getInstance()->addPopupInView(popUpView,true,false,true);
    }
}

void MailController::deleteMailFromAndroid(int tabType,int type,std::string mailUid,std::string fromUid){
    CCLOGFUNC("");
    if(tabType==0 && type!=CHAT_ROOM){
        MailDialogDeleteCommand* command = new MailDialogDeleteCommand(fromUid, -1, mailUid);
        command->sendAndRelease();
    }else if((type==MAIL_RESOURCE_HELP && !getIsNewMailUIEnable())|| type==MAIL_RESOURCE || type==MAIL_ATTACKMONSTER){
        MailDialogDeleteCommand* command = new MailDialogDeleteCommand("",type);
        command->sendAndRelease();
    }else{
        removeMailFromServer(mailUid, 1);
    }
    
    auto it = GlobalData::shared()->mailList.find(mailUid);
    if (GlobalData::shared()->mailList.end() != it) {
        GlobalData::shared()->mailList.erase(it);
    }
}

void MailController::deleteMailBySelectFromAndroid(std::string mailUidStr,std::string types)
{
    vector<std::string> vector;
    CCCommonUtils::splitString(mailUidStr, ",", vector);
    int i = 0;
    
    CCLOGFUNCF("mailUidStr %s  types:%s",mailUidStr.c_str(),types.c_str());
    MailBatchDelCommand *cmd = new MailBatchDelCommand(mailUidStr,types);
    cmd->sendAndRelease();
    
    while(i < vector.size()){
        std::string uid = vector[i];
        auto it=GlobalData::shared()->mailList.find(uid);
        if (  it!=GlobalData::shared()->mailList.end()) {
            GlobalData::shared()->mailList.erase(it);
        }
        i++;
    }
}

void MailController::getUpdateMail(std::string modifyTime)
{
    CCLOGFUNCF("getUpdateMail : %s",modifyTime.c_str());
    GetUpdateMailCommand *cmd = new GetUpdateMailCommand(modifyTime);
    cmd->sendAndRelease();
}

string MailController::getPointByOccupyIdx(int index)
{
//     CCLOGFUNCF("index : %d",index);
    string cord="";
    CCPoint point=WorldController::getInstance()->getPointByIndex((int)index);
    cord.append(CC_ITOA(point.x)).append("_").append(CC_ITOA(point.y));
    return cord;
}

void MailController::openMailReadPop(string mailId)
{
    if (GlobalData::shared()->mailList.find(mailId) != GlobalData::shared()->mailList.end())
    {
        MailInfo* m_mailInfo = GlobalData::shared()->mailList[mailId];
        
        if(m_mailInfo==NULL){
            return;
        }
        bool issaveRead = false;
        if(m_mailInfo->status==0)
        {
            issaveRead = true;
            m_mailInfo->status = READ;
            if (m_mailInfo->tabType != 1) {
                MailController::getInstance()->notyfyReadMail(m_mailInfo->uid, m_mailInfo->type);
                if(m_mailInfo->tabType==MAILTAB4){
                    GlobalData::shared()->mailCountObj.fightR -=1;
                }else if(m_mailInfo->tabType==MAILTAB5){
                    GlobalData::shared()->mailCountObj.modR -=1;
                }else if(m_mailInfo->tabType==MAILTAB3){
                    GlobalData::shared()->mailCountObj.studioR -=1;
                }else if(m_mailInfo->type== MAIL_FRESHER||m_mailInfo->type==MAIL_SYSNOTICE||m_mailInfo->type==MAIL_SYSUPDATE){
                    GlobalData::shared()->mailCountObj.saveR -=1;
                }else if(m_mailInfo->tabType==USERMAIL){
                    GlobalData::shared()->mailCountObj.perR -=1;
                }else{
                    GlobalData::shared()->mailCountObj.sysR -=1;
                }
            }
            else {
                if(m_mailInfo->type== MAIL_FRESHER||m_mailInfo->type==MAIL_SYSNOTICE||m_mailInfo->type==MAIL_SYSUPDATE
                   ||m_mailInfo->tabType==USERMAIL||m_mailInfo->type==MAIL_RESOURCE||m_mailInfo->type==MAIL_ATTACKMONSTER
                ||m_mailInfo->type==MAIL_RESOURCE_HELP|| m_mailInfo->type==MAIL_MOD_PERSONAL|| m_mailInfo->type==MAIL_MOD_SEND
                         ){
                }else{
                    GlobalData::shared()->mailCountObj.sysR -=1;
                    MailController::getInstance()->notyfyReadMail(m_mailInfo->uid, m_mailInfo->type);
                }
            }
            
            if(m_mailInfo->type==MAIL_SYSUPDATE){
                GlobalData::shared()->mailCountObj.upR-=1;
            }
        }
        
        UIComponent::getInstance()->loadMailResource();
        if(m_mailInfo->tabType == 1)
        {
            if(m_mailInfo->type == MAIL_BATTLE_REPORT){
                PopupViewController::getInstance()->addPopupInViewWithAnim(BattleReportMailPopUpView::create(m_mailInfo));
            }
            else if(m_mailInfo->type == MAIL_RESOURCE){
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailResourcePopUpView::create(dynamic_cast<MailResourceCellInfo*>(m_mailInfo)));
            }else if(m_mailInfo->type == MAIL_RESOURCE_HELP){
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailResourceHelpView::create(dynamic_cast<MailResourceHelpCellInfo*>(m_mailInfo)));
            }
            else if(m_mailInfo->type == MAIL_DETECT_REPORT){
                if(m_mailInfo->pointType == MAIL_DETECT_REPORT_PROTECT){
                    PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo), true);
                }else{
                    PopupViewController::getInstance()->addPopupInViewWithAnim(DetectMailPopUpView::create(m_mailInfo));
                }
            }else if(m_mailInfo->type==MAIL_DETECT){
                if(m_mailInfo->itemIdFlag == 1){ //反侦察
                    PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo), true);
                }else{
                    PopupViewController::getInstance()->addPopupInViewWithAnim(DetectMailPopUpView::create(m_mailInfo));
                }
            }
            else if(m_mailInfo->type == MAIL_ENCAMP){
                PopupViewController::getInstance()->addPopupInViewWithAnim(OccupyMailPopUpView::create(m_mailInfo));
            }
            else if(m_mailInfo->type == WORLD_NEW_EXPLORE){
                PopupViewController::getInstance()->addPopupInViewWithAnim(ExplorePopUpView::create(m_mailInfo));
            }else if(m_mailInfo->type==MAIL_SYSUPDATE){
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailAnnouncePopUp::create(m_mailInfo));
            }else if(m_mailInfo->type == MAIL_ATTACKMONSTER){
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailMonsterListView::create(dynamic_cast<MailMonsterCellInfo*>(m_mailInfo)));
            }else if(m_mailInfo->type == MAIL_GIFT){
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailGiftReadPopUpView::create(*m_mailInfo));
            }else if(m_mailInfo->type == MAIL_WORLD_BOSS){
                PopupViewController::getInstance()->addPopupInViewWithAnim(WorldBossRewardMailView::create(*m_mailInfo), true);
            }else{
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo), true);
            }
        }
        else
        {
            if(m_mailInfo->type == MAIL_BATTLE_REPORT){
                PopupViewController::getInstance()->addPopupInViewWithAnim(BattleReportMailPopUpView::create(m_mailInfo));
            }
            else if(m_mailInfo->type == MAIL_WORLD_BOSS){
                PopupViewController::getInstance()->addPopupInViewWithAnim(WorldBossMailView::create(*m_mailInfo));
            }
            else if(m_mailInfo->type == MAIL_RESOURCE){
                PopupViewController::getInstance()->addPopupInViewWithAnim(ResourceMailPopUpView::create(m_mailInfo));
            }
            else if(m_mailInfo->type == MAIL_DETECT_REPORT){
                if( m_mailInfo->pointType == MAIL_DETECT_REPORT_PROTECT){
                    PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo), true);
                }else{
                    PopupViewController::getInstance()->addPopupInViewWithAnim(DetectMailPopUpView::create(m_mailInfo));
                }
            }else if(m_mailInfo->type==MAIL_DETECT){
                if(m_mailInfo->itemIdFlag == 1){
                    PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo));
                }else{
                    PopupViewController::getInstance()->addPopupInViewWithAnim(DetectMailPopUpView::create(m_mailInfo));
                }
            }
            else if(m_mailInfo->type == MAIL_ENCAMP){
                PopupViewController::getInstance()->addPopupInViewWithAnim(OccupyMailPopUpView::create(m_mailInfo));
            }
            else if(m_mailInfo->type == WORLD_NEW_EXPLORE){
                PopupViewController::getInstance()->addPopupInViewWithAnim(ExplorePopUpView::create(m_mailInfo));
            }else if(m_mailInfo->type==MAIL_SYSUPDATE){
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailAnnouncePopUp::create(m_mailInfo));
            }else if(m_mailInfo->type == MAIL_ATTACKMONSTER){
                PopupViewController::getInstance()->addPopupInViewWithAnim(MonsterAttackPopUpView::create(m_mailInfo));
            }else if(m_mailInfo->type == MAIL_MOD_SEND || m_mailInfo->type == MAIL_MOD_PERSONAL){
                int type = MAIL_MOD_PERSONAL;
                if(GlobalData::shared()->playerInfo.gmFlag == 2 || GlobalData::shared()->playerInfo.gmFlag == 5){
                    type = MAIL_MOD_SEND;
                }
                MailController::getInstance()->openMailDialogViewFirst(m_mailInfo->fromName, m_mailInfo->fromUid, "",type);
            }else{
                PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*m_mailInfo));
            }
        }
        
        if(issaveRead){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
        }
        
    }
}

void MailController::showMailTips(string mailId)
{
    string title = "";
    string description = "";
    string lastMailId = "";
    if(atoi(mailId.c_str()) == MAIL_ATTACKMONSTER)
    {
        MailInfo* m_mailInfo = NULL;
        map<string, MailInfo*>::iterator it;
        for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
            if(it->second->type==MAIL_ATTACKMONSTER){
                m_mailInfo = it->second;
                break;
            }
        }
        if (m_mailInfo == NULL) {
            return;
        }
        
        title = _lang("105519");
        lastMailId = m_mailInfo->uid;
        MailMonsterCellInfo* monstInfo = dynamic_cast<MailMonsterCellInfo*>(m_mailInfo);
        if (monstInfo) {
            MailMonsterInfo* monsterInfo1 = dynamic_cast<MailMonsterInfo*>(monstInfo->monster->objectAtIndex(0));
            std::string name = CCCommonUtils::getNameById(monsterInfo1->monsterId);
            string level = CCCommonUtils::getPropById(monsterInfo1->monsterId,"level");
            name+=" Lv.";
            name+=level;
            string monsterResoult = "";
            if(monsterInfo1->monsterResult==1){//首杀奖励
                monsterResoult = _lang("103758");
            }else if(monsterInfo1->monsterResult==2){//战斗失败
                monsterResoult = _lang("105118");
            }else if(monsterInfo1->monsterResult == 4){//战斗无效
                monsterResoult = _lang("103786");
            }else{
                monsterResoult = _lang("105117");//战斗胜利
            }
            description = name + "                     "+monsterResoult;
        }
    }
    else
    {
        lastMailId = mailId;
        MailInfo* m_mailInfo = GlobalData::shared()->mailList[mailId];
        
        title = m_mailInfo->fromName;
        bool isbigLoss = false;
        int m_kill = 0;
        int m_loss = 0;
        if(m_mailInfo->atkGen == NULL){
            isbigLoss = true;
        }else {
            string KillAndLoss = calculateKillandLoss(m_mailInfo);
            vector<string> tmpVec;
            CCCommonUtils::splitString(KillAndLoss, "_", tmpVec);
            if (tmpVec.size()>=2) {
                m_kill = atoi(tmpVec[0].c_str());
                m_loss = atoi(tmpVec[1].c_str());
            }
        }
        
        bool isAtt = false;
        std::string attUid="";
        std::string attName="";
        std::string defName="";
        if(m_mailInfo->attUser && m_mailInfo->attUser->valueForKey("uid")){
            attUid = m_mailInfo->attUser->valueForKey("uid")->getCString();
        }
        if(m_mailInfo->attUser && m_mailInfo->attUser->valueForKey("name")){
            attName = m_mailInfo->attUser->valueForKey("name")->getCString();
        }
        if(m_mailInfo->defUser && m_mailInfo->defUser->valueForKey("name")){
            defName = m_mailInfo->defUser->valueForKey("name")->getCString();
        }
        
        if(m_mailInfo->atkHelper!=NULL){
            cocos2d::CCObject* obj;
            CCARRAY_FOREACH(m_mailInfo->atkHelper, obj){
                string helpUid = dynamic_cast<CCString*>(obj)->getCString();
                if(helpUid==GlobalData::shared()->playerInfo.uid){
                    isAtt = true;
                    break;
                }
            }
        }
        
        if(GlobalData::shared()->playerInfo.uid==attUid){
            isAtt = true;
        }
        int reportState;
        if(m_mailInfo->winner == ""){
            reportState= DRAW;
        }
        if(m_mailInfo->winner == attUid){
            if(isAtt){
                reportState= WIN;
            }else{
                reportState=LOOSE;
            }
        }else{
            if(isAtt){
                reportState= LOOSE;
            }else{
                reportState=WIN;
            }
        }
        
        if(isAtt){
            m_mailInfo->title = _lang_2("105547",_lang("102187").c_str(),defName.c_str());
        }else{
            if(m_mailInfo->battleType!=3){
                m_mailInfo->title = _lang_2("105547",attName.c_str(),_lang("102187").c_str());
            }else{
                m_mailInfo->title = _lang_2("105547",attName.c_str(),_lang("108678").c_str());
            }
        }
        title = m_mailInfo->title;
        if(reportState==WIN)
        {
            if(m_mailInfo->battleType==3){
                if(isAtt){
                    switch (m_mailInfo->pointType) {
                        case Throne:{
                            title = _lang("105700");
                            break;
                        }
                        case Trebuchet:{
                            title = _lang("105704");
                            break;
                        }
                        default:{
                            title = _lang("105578");
                            break;
                        }
                    }
                }else{
                    switch (m_mailInfo->pointType) {
                        case Throne:{
                            title = _lang("105702");
                            break;
                        }
                        case Trebuchet:{
                            title = _lang("105706");
                            break;
                        }
                        default:{
                            title = _lang("105579");
                            break;
                        }
                    }
                }
                description = _lang("108554");
                description.append(CC_CMDITOA(m_kill));
                description.append(" ");
                description.append(_lang("105019"));
                string temp =CC_CMDITOA(m_loss);
                temp.append(" ...");
                description.append(temp);
                
            }else if(m_mailInfo->battleType==6){
                title = _lang("133053");
                description = _lang("105579");
            }else if (m_mailInfo->battleType==7){//联盟领地战斗报告
                if(isAtt){
                    if (m_mailInfo->pointType == Tile_allianceArea) {
                        title = _lang("115337");
                    }
                    else if (m_mailInfo->pointType == tile_banner) {
                        title = _lang("115540");
                    }
                    description = _lang_2("115341", CC_ITOA(m_kill), CC_ITOA(m_loss));
                }else{
                    if (m_mailInfo->pointType == Tile_allianceArea) {
                        title = _lang("115339");
                    }
                    else if (m_mailInfo->pointType == tile_banner) {
                        title = _lang("115542");
                    }
                    description = _lang_2("115341", CC_ITOA(m_kill), CC_ITOA(m_loss));
                }
            }else if(m_mailInfo->msReport==1){
                title = _lang("133053");
                description = _lang("133083");
            }
        }else{
            if(m_mailInfo->battleType==3){
                if(isbigLoss){
                    if(isAtt){
                        switch (m_mailInfo->pointType) {
                            case Throne:{
                                title = _lang("105701");
                                break;
                            }
                            case Trebuchet:{
                                title = _lang("105705");
                                break;
                            }
                            default:{
                                title =_lang("105583");
                                break;
                            }
                        }
                    }else{
                        switch (m_mailInfo->pointType) {//守城大败
                            case Throne:{
                                title = _lang("105703");
                                break;
                            }
                            case Trebuchet:{
                                title = _lang("105707");
                                break;
                            }
                            default:{
                                title =_lang("105581");
                                break;
                            }
                        }
                    }
                    description =_lang("105535");
                    title = m_mailInfo->fromName;
                }
                else{
                    if(isAtt){
                        switch (m_mailInfo->pointType) {
                            case Throne:{
                                title = _lang("105701");
                                break;
                            }
                            case Trebuchet:{
                                title = _lang("105705");
                                break;
                            }
                            default:{
                                title = _lang("105582");
                                break;
                            }
                        }
                    }else{
                        switch (m_mailInfo->pointType) { // 守城失败
                            case Throne:{
                                title = _lang("105703");
                                break;
                            }
                            case Trebuchet:{
                                title = _lang("105707");
                                break;
                            }
                            default:{
                                title = _lang("105580");
                                break;
                            }
                        }
                    }
                    description = _lang("108554");
                    description.append(CC_CMDITOA(m_kill));
                    description.append(" ");
                    description.append(_lang("105019"));
                    string temp =CC_CMDITOA(m_loss);
                    temp.append(" ...");
                    description.append(temp);
                }
            }else if(m_mailInfo->battleType==6){
                title = _lang("133053");
                description = _lang("105580");
            }else if (m_mailInfo->battleType==7){
                if(isAtt){
                    if (m_mailInfo->pointType == Tile_allianceArea) {
                        title = _lang("115338");
                    }
                    else if (m_mailInfo->pointType == tile_banner) {
                        title = _lang("115541");
                    }
                    description = _lang_2("115341", CC_ITOA(m_kill), CC_ITOA(m_loss));
                }else{
                    if (m_mailInfo->pointType == Tile_allianceArea) {
                        title = _lang("115340");
                    }
                    else if (m_mailInfo->pointType == tile_banner) {
                        title = _lang("115543");
                    }
                    string description = _lang_2("115341", CC_ITOA(m_kill), CC_ITOA(m_loss));
                }
            }else if(m_mailInfo->msReport==1){
                title = _lang("133053");
                description = _lang("133083");
            }
        }
    }
    
    if (title!="" && description!="") {
        CCLOG("title:%s,   description:%s", title.c_str(), description.c_str());
        CCCommonUtils::flyHint("icon_suxing.png", title, description, 3, 0, false, 0.7, lastMailId);
    }
}

string MailController::calculateKillandLoss(MailInfo* m_mailInfo)
{
   
    if(m_mailInfo==NULL)
        return "";
     bool isAtt = false;
    std::string attUid="";
    if (m_mailInfo->attUser!=NULL && m_mailInfo->attUser->objectForKey("uid")!=NULL) {
        attUid = m_mailInfo->attUser->valueForKey("uid")->getCString();
    }
    if(GlobalData::shared()->playerInfo.uid==attUid){
        isAtt = true;
    }else{
        if(m_mailInfo->atkHelper!=NULL){
            cocos2d::CCObject* obj;
            CCARRAY_FOREACH(m_mailInfo->atkHelper, obj){
                CCString* help = dynamic_cast<CCString*>(obj);
                if(help)
                {
                    string helpUid =help ->getCString();
                    if(helpUid==GlobalData::shared()->playerInfo.uid){
                        isAtt = true;
                        break;
                    }
                }
            }
        }
    }
    CCDictionary *attUserInfo;
    if (isAtt) {
        attUserInfo = m_mailInfo->attUser;
    }else{
        attUserInfo = m_mailInfo->defUser;
    }
    std::string npc="";
    if(attUserInfo!=NULL && attUserInfo->objectForKey("npcId"))
        npc = attUserInfo->valueForKey("npcId")->getCString();
    int dead = 0;
    int num = 0;
    int hurt = 0;
    int kill = 0;
    int total = 0;
    if(npc!=""){
        if(m_mailInfo->defReport){
            int count = m_mailInfo->defReport->count();
            for (int i=0; i<count; i++) {
                CCDictionary* dic = _dict(m_mailInfo->defReport->objectAtIndex(i));
                if(dic){
                    if(dic->objectForKey("kill"))
                        kill +=dic->valueForKey("kill")->intValue();
                    if(dic->objectForKey("dead"))
                        dead +=dic->valueForKey("dead")->intValue();
                    if(dic->objectForKey("hurt"))
                        hurt +=dic->valueForKey("hurt")->intValue();
                    if(dic->objectForKey("num"))
                        num +=dic->valueForKey("num")->intValue();
                }
            }
        }
        total = dead+num+hurt;
        if(total<=0) total = 1;
    }else{
        if(isAtt==true){
            if(m_mailInfo->atkArmyTotal){
                CCDictionary* dic = _dict(m_mailInfo->atkArmyTotal);
                if (dic) {
                    if(dic->objectForKey("kill"))
                        kill +=dic->valueForKey("kill")->intValue();
                    if(dic->objectForKey("dead"))
                        dead +=dic->valueForKey("dead")->intValue();
                    if(dic->objectForKey("hurt"))
                        hurt +=dic->valueForKey("hurt")->intValue();
                    if(dic->objectForKey("num"))
                        num +=dic->valueForKey("num")->intValue();
                }
            }
            CCArray *atkGenKilltArr = dynamic_cast<CCArray*>(m_mailInfo->atkGenKill);
            CCObject *atkGenKilltobj;
            if(atkGenKilltArr){
                CCARRAY_FOREACH(atkGenKilltArr, atkGenKilltobj){
                    CCString* genKill = dynamic_cast<CCString*>(atkGenKilltobj);
                    if(genKill)
                        kill+=genKill->intValue();
                }
            }
        }else{
            if(m_mailInfo->defArmyTotal){
                if(m_mailInfo->defArmyTotal->objectForKey("kill"))
                    kill +=m_mailInfo->defArmyTotal->valueForKey("kill")->intValue();
                if(m_mailInfo->defArmyTotal->objectForKey("dead"))
                    dead +=m_mailInfo->defArmyTotal->valueForKey("dead")->intValue();
                if(m_mailInfo->defArmyTotal->objectForKey("hurt"))
                    hurt +=m_mailInfo->defArmyTotal->valueForKey("hurt")->intValue();
                if(m_mailInfo->defArmyTotal->objectForKey("num"))
                    num +=m_mailInfo->defArmyTotal->valueForKey("num")->intValue();
            }
            CCArray *defGenKilltArr = dynamic_cast<CCArray*>(m_mailInfo->defGenKill);
            CCObject *defGenKilltobj;
            if(defGenKilltArr){
                CCARRAY_FOREACH(defGenKilltArr, defGenKilltobj){
                    CCString* genKill = dynamic_cast<CCString*>(defGenKilltobj);
                    if(genKill)
                        kill+=genKill->intValue();
                }
            }
            if(m_mailInfo->defTowerKill!=NULL){
                
                for (int i=0; i<m_mailInfo->defTowerKill->count(); i++) {
                    auto trapDic = _dict(m_mailInfo->defTowerKill->objectAtIndex(i));
                    if(trapDic && trapDic->objectForKey("kill")){
                        kill += trapDic->valueForKey("kill")->intValue();
                    }
                }
            }
            if(m_mailInfo->defFortLost!=NULL){
                int total = 0;
                for (int i=0; i<m_mailInfo->defFortLost->count(); i++) {
                    auto trapDic = _dict(m_mailInfo->defFortLost->objectAtIndex(i));
                    if(trapDic){
                        if(trapDic->objectForKey("dead"))
                            total += trapDic->valueForKey("dead")->intValue();
                        if(trapDic->objectForKey("kill"))
                            kill+=trapDic->valueForKey("kill")->intValue();
                    }
                }
            }
        }
    }
    total = dead+num+hurt;
    
    string ret = "";
    return ret+CC_ITOA(kill)+"_"+CC_ITOA(dead);
}